#include "scd.h"
#include "scd_datatypetemplates.h"
#include "scd_ied.h"
#include "scd_Communication.h"

#include <QtCore>
#include <QtXml>


SCD* SCD::instance_ = NULL;


SCD::SCD()
{
	m_mapIedType['C'] = "1";
	m_mapIedType['P'] = "2";
	m_mapIedType['M'] = "3";
	m_mapIedType['I'] = "4";
	m_mapIedType['D'] = "5";
	m_mapIedType['R'] = "6";

	m_mapIedType['c'] = "1";
	m_mapIedType['p'] = "2";
	m_mapIedType['m'] = "3";
	m_mapIedType['i'] = "4";
	m_mapIedType['d'] = "5";
	m_mapIedType['r'] = "6";
    instance_ = NULL;
}

SCD::~SCD()
{

}


SCD* SCD::instance()
{
    if(NULL == SCD::instance_)
    {
        SCD::instance_ = new SCD();
    }

    return SCD::instance_;
}

void SCD::close_singleton()
{
    if(instance_ != NULL)
    {
        instance_->clearAllData();
        delete instance_;
        instance_ = NULL;
    }
}

DataTypeTemplates* SCD::getDataTypeTemplates()
{
    return this->dataTypeTemplates_;
}

bool SCD::init(string &filename)
{
    QFile file(filename.c_str());
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        doc.setContent(&file);
        QDomElement root = doc.documentElement();

        QDomNode node = root.firstChild();
        QString tagName;

        while(!node.isNull())
        {
            QDomElement e = node.toElement();
            tagName = e.tagName();

            if(tagName == QString("Header"))
            {
                this->initHeader(e);
            }
            else if(tagName == QString("Communication"))
			{
				this->initCommunication(e);
			}
			else if(tagName == QString("IED"))
			{
				this->initIED(e);            
			}
			else if(tagName == QString("DataTypeTemplates"))
			{
				this->initDataTypeTemplates(e);
			}

            node = node.nextSibling();
        } 
    }
    else
    {
		listErrorInfo.push_back("scd file not find"); 
        return false;
    }

    file.close();
	return true;
}

//数据集
void SCD::initDataSetToAddress()
{
    #define KEY_NAME    ("%1/%2$%3")

    IED* pIed = NULL;
    QString name;
    map<string,IED*>::iterator it;

    map<string,vector<string> > tmp;

    for(it=mpIED_.begin(); it != mpIED_.end();++it)
    {

        name = QString::fromStdString(it->first);

        pIed = it->second;
        tmp = pIed->initDataSetToAddress();

        this->mpDataSetToDa_.insert(tmp.begin(),tmp.end());
    }
}

//我改1 DOI，由于SCD的问题，DOI解析定义的类型
void SCD::initDoiToAddress()
{ 
    IED* pIed = NULL; 
    map<string,IED*>::iterator it;

    map<QString,vector<string> > tmp;

    for(it=mpIED_.begin(); it != mpIED_.end();++it)
    {  
        pIed = it->second;
        tmp = pIed->initDoiToAddress();

        this->mpDoiToDa_.insert(tmp.begin(),tmp.end()); 
    }
}

void SCD::clearAllData()
{
    mpSubstation_.clear();
    delete communication_;
    communication_ = NULL;
    mpIED_.clear();
    delete dataTypeTemplates_;
    dataTypeTemplates_ = NULL;
    mpDataSetToDa_.clear();
    mpDoiToDa_.clear();
    listErrorInfo.clear();
}

void SCD::getAllRptCtrlBlk(QList<QString> &listRptCtrlBlk)
{  
	IED* pIed = NULL;  
	map<string,IED*>::iterator it;  

	for(it=mpIED_.begin(); it != mpIED_.end();++it)
	{ 
		pIed = it->second; 
		QString rptName("%1%2/%3.RP.%4");
		pIed->getAllRptCtrlBlk(listRptCtrlBlk,rptName); 
	}
}

void SCD::getAllIedAddress(map<string, stIedData> &mapIedData)
{
	this->communication_->getAllIedAddress(mapIedData);
}

void SCD::GetAllIedDesc(map<string, stIedData> &mapIedData)
{
	map<string, IED*>::iterator it;

	for (it = mpIED_.begin(); it != mpIED_.end(); ++it)
	{
		string iedName = it->first;
		if (mapIedData.find(iedName) == mapIedData.end())
		{
			stIedData iedData;
			iedData.IpA_ = "192.168.0.0";
			iedData.PortA_ = 102;
			iedData.IpB_ = "192.168.0.1";
			iedData.PortB_ = 102;
			iedData.Desc_ = it->second->desc_.toStdString();
			iedData.Config_ = it->second->configVersion_.toStdString();
			iedData.Manu_ = it->second->manufacturer_.toStdString();
			iedData.Model_ = it->second->type_.toStdString();
			if (m_mapIedType.find(iedName.at(0)) == m_mapIedType.end())
			{ 
				mapIedData[iedName].Type_ = "6";
			}
			else
			{ 
				mapIedData[iedName].Type_ = m_mapIedType[iedName.at(0)];
			} 
			mapIedData[iedName] = iedData;
		}
		else
		{
			mapIedData[iedName].Desc_ = it->second->desc_.toStdString();
			mapIedData[iedName].Config_ = it->second->configVersion_.toStdString();
			mapIedData[iedName].Manu_ = it->second->manufacturer_.toStdString();
			mapIedData[iedName].Model_ = it->second->type_.toStdString();

			if (m_mapIedType.find(iedName.at(0)) == m_mapIedType.end())
			{
				mapIedData[iedName].Type_ = "6";
			}
			else
			{
				mapIedData[iedName].Type_ = m_mapIedType[iedName.at(0)];
			} 
		}
	}
}

void SCD::SetNetworkPoint(map<string, list<stPointData>> &mapPointData)
{
	map<string, IED*>::iterator it;

	for (it = mpIED_.begin(); it != mpIED_.end(); ++it)
	{
		stPointData pointData; 
		pointData.RedisAddr_ = QString(it->first.c_str() + QString("_00057")).toStdString();
		pointData.Desc_ = QString::fromLocal8Bit("A网状态").toStdString();
		pointData.DoName_ = "Alm4";
		pointData.Name_ = QString(it->first.c_str() + QString("LD0/GGIO0$ST$Alm4$stVal")).toStdString();
		pointData.Type_ = "BOOLEAN";

		mapPointData[it->first].push_back(pointData);

		pointData.RedisAddr_ = QString(it->first.c_str() + QString("_00061")).toStdString();
		pointData.Desc_ = QString::fromLocal8Bit("B网状态").toStdString();
		pointData.DoName_ = "Alm5";
		pointData.Name_ = QString(it->first.c_str() + QString("LD0/GGIO0$ST$Alm5$stVal")).toStdString();

		mapPointData[it->first].push_back(pointData);
	}
}

void SCD::getErrorList(list<string> &listError)
{ 
	listError = listErrorInfo;
}

map<string,vector<string> >& SCD::getDataSetToAddress()
{
    return this->mpDataSetToDa_;
}

map<QString,vector<string> >& SCD::getDoiToAddress()
{
    return this->mpDoiToDa_;
}


#if 0


 std::map<std::string,std::vector<std::string> >&
         SCD::init(std::string filename)
 {
    QString name(filename.c_str());
    QFile file(name);

    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        doc.setContent(&file);
        QDomElement root = doc.documentElement();

        QDomNode node = root.firstChild();
        QString tagName;

        while(!node.isNull())
        {
            QDomElement e = node.toElement();
            tagName = e.tagName();

            if(tagName == QString("Header"))
            {
                 qDebug()<<"begin handler Header... \n ";

                this->initHeader(e);

                qDebug()<<"end handler Header... \n ";
            }
            else
                if(tagName == QString("Communication"))
                {

                    qDebug()<<"begin handler initCommunication... \n ";

                    this->initCommunication(e);

                   qDebug()<<"end handler initCommunication... \n ";
                }
                else
                    if(tagName == QString("IED"))
                    {

                        qDebug()<<"begin handler initIED... \n ";

                        this->initIED(e);

                       qDebug()<<"end handler initIED... \n ";

                    }
                    else
                        if(tagName == QString("DataTypeTemplates"))
                        {

                            qDebug()<<"begin handler initDataTypeTemplates... \n ";

                           this->initDataTypeTemplates(e);

                           qDebug()<<"end handler initDataTypeTemplates... \n ";
                        }

            node = node.nextSibling();
        }

    }
    else
    {
        qDebug()<<"scd file is  open faild.\n ";
    }

   return mpDataSetToDa_;
 }
#endif



 void SCD::initHeader(QDomElement& /*e*/)
 {

 }
 void SCD::initSubstation(QDomElement& /*e*/)
 {

 }
 void SCD::initCommunication(QDomElement& e)
 {
     communication_ = new stCommunication();

     communication_->desc_ = e.attribute("desc","");

     QDomNode n = e.firstChild();
     QDomElement tmp;
     QString name;
     while(!n.isNull())
     {
         tmp = n.toElement();
         name = tmp.tagName();

         if(name == "SubNetwork")
         {
             communication_->initSubNetwork(tmp); 
         }

         n = n.nextSibling();
     }

 }

 void SCD::initIED(QDomElement& e)
 {
    stIED* ied = new stIED();

    ied->desc_ = e.attribute("desc","");
    ied->name_ = e.attribute("name","");
    ied->type_ = e.attribute("type","");
    ied->manufacturer_ = e.attribute("manufacturer","");
    ied->configVersion_ = e.attribute("configVersion","");
    ied->originalSclVersion_ = e.attribute("orignalSclVersion","");
    ied->originalSclRevision_ = e.attribute("originalSclRevision","");
    ied->engRight_ = e.attribute("engRight","");
    ied->owner_ = e.attribute("owner","");

    this->mpIED_.insert(map<string,stIED*>
                        ::value_type(ied->name_.toStdString(),ied));


    QDomNode n = e.firstChild();
    QDomElement tmp;
    QString name;
    while(!n.isNull())
    {
        tmp = n.toElement();
        name = tmp.tagName();

        if(name == "Services")
        {
            ied->initServices(tmp);
        }
        else if(name == "AccessPoint")
        {
            ied->initAccessPoint(tmp);
        }

        n = n.nextSibling();
    }


 }

 void SCD::initDataTypeTemplates(QDomElement& e)
 {
    dataTypeTemplates_ = new DataTypeTemplates();

    QDomNode n = e.firstChild();
    QDomElement tmp;
    QString name;
    while(!n.isNull())
    {
        tmp = n.toElement();
        name = tmp.tagName();

        if(name == "LNodeType")
        {
            dataTypeTemplates_->initLNodeType(tmp);
        }
        else if(name == "DOType")
        {
            dataTypeTemplates_->initDOType(tmp);
        }
        else if(name == "DAType")
		{
			dataTypeTemplates_->initDAType(tmp);
		}
		else if(name == "EnumType")
		{
			dataTypeTemplates_->initEnumType(tmp);
		}

        n = n.nextSibling();
    }

 }

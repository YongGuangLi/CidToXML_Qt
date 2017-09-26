#include "cidtoxml.h"
#include "scd.h"
#include "scd_communication.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDomDocument>
#include <QDomElement>
#include <QTextCodec>
#include <QMap>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <string>
#include <windows.h>
#include <fstream>


CidToData::CidToData()
{

}

CidToData::~CidToData()
{

}


int CidToData::ConvertScd2Data(std::string csCidFile,std::string csInitFile, std::list<std::string> &lstErrors)
{     
	if(!SCD::instance()->init(csCidFile))
	{ 
		SCD::instance()->getErrorList(lstErrors);
		return -1;
	} 
	 
	if (!InitCfgFile(csInitFile))
	{  
        lstErrors.push_back("ini file load failed\n");
		return -1;
    }
    mapIedData_.clear();
	 
	SCD::instance()->getAllIedAddress(this->mapIedData_);
	SCD::instance()->GetAllIedDesc(this->mapIedData_);

    SCD::instance()->initDataSetToAddress();

    mapPointData_.clear();
    map<string,vector<string>>::iterator it = SCD::instance()->getDataSetToAddress().begin();
    int iCnt = 0;
    int iNum = 0;
    for( ; it != SCD::instance()->getDataSetToAddress().end(); ++it)
    {
        vector<string>::iterator it1 = it->second.begin();

        for( ; it1 != it->second.end(); ++it1)
        {
            iNum++;
            QStringList stringList = QString::fromStdString(*it1).split(":");
            if(mapPointData_.find(stringList.at(0).toStdString()) == mapPointData_.end()) 
            {
                iCnt = 1;                 //ied中发布地址重新计数
            }

            stPointData pointData;
            pointData.ID_ = iNum;
            pointData.RedisAddr_ = QString(stringList.at(0) + "_"+ QString::number(iCnt)).toStdString();
            pointData.Desc_ = stringList.at(1).toStdString();
            pointData.DoName_ = stringList.at(2).toStdString();
            pointData.Name_ = stringList.at(3).toStdString();
            pointData.Type_ = stringList.at(4).toStdString();

            if(mapPointData_.find(stringList.at(0).toStdString()) != mapPointData_.end())
            {
                mapPointData_[stringList.at(0).toStdString()].push_back(pointData);
            }
            else
            {
                list<stPointData> listPointData;
                listPointData.push_back(pointData);
                mapPointData_[stringList.at(0).toStdString()] = listPointData;
            }
            iCnt++;
        }
    } 
 
	SCD::instance()->SetNetworkPoint(mapPointData_);

    SCD::instance()->getErrorList(lstErrors);
    SCD::instance()->close_singleton();
	return 0;
}


std::map<std::string, stIedData> CidToData::GetIedData()
{
	return mapIedData_;
}


std::map<std::string, list<stPointData>> CidToData::GetPointData()
{
    return mapPointData_;
}


bool CidToData::InitCfgFile(std::string csInitFile)
{
	QFile file(QString::fromLocal8Bit(csInitFile.c_str()));

	if(file.open(QIODevice::ReadOnly))
	{
		QSettings *settings = new QSettings(QString::fromLocal8Bit(csInitFile.c_str()),QSettings::IniFormat,NULL); 
		if (settings != NULL)
		{ 
			settings->beginGroup("FC");
			QStringList FCkeys = settings->allKeys();
			for (int i = 0 ; i < FCkeys.size(); ++i)
			{
				if (settings->value(FCkeys.at(i)).toString() == "1")
				{
					mapFilterFC_[FCkeys.at(i).toStdString()] = settings->value(FCkeys.at(i)).toString().toStdString();
				} 
			} 
			settings->endGroup();

			settings->beginGroup("TYPE");
			QStringList TYPEkeys = settings->allKeys();
			for (int i = 0 ; i < TYPEkeys.size(); ++i)
			{ 
				mapFilterType_[TYPEkeys.at(i).toStdString()] = settings->value(TYPEkeys.at(i)).toString().toStdString(); 
			} 
			settings->endGroup();

			return true;
		}
		file.close();
	} 
	else
	{ 
		return false;
	}
}

  
extern "C" __declspec(dllexport) ICidToData* CreateModule(void* pIService)
{
	CidToData* pModule = new CidToData();
	return pModule;
}

extern "C" __declspec(dllexport) void DeleteModule(ICidToData* pModule)
{
	if(pModule == NULL)
		return ;
	delete (CidToData*)pModule;
}


 

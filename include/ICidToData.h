#pragma once
#include <string>
#include <map>
#include <list>


typedef struct
{
	std::string IpA_;
	std::string IpB_;
	int PortA_;
	int PortB_;
	std::string Type_;     //IED����
	std::string Desc_;     //IED����
	std::string Config_;   //�汾
	std::string Manu_;     //������
	std::string Model_;    //�ͺ�

}stIedData;


typedef struct
{
	int ID_;
	std::string	RedisAddr_;
	std::string Name_;
	std::string Desc_;     //�߼��ڵ�����/���ݶ�������
	std::string Type_;    
    std::string DoName_;    
	std::string iedName_;
}stPointData;

class ICidToData
{
public:
	virtual int ConvertScd2Data(std::string csCidFile,std::string csInitFile, std::list<std::string> &lstErrors) = 0;
	virtual std::map<std::string,stIedData> GetIedData() = 0;
    virtual std::map<std::string,std::list<stPointData>> GetPointData() = 0;
};


#ifdef CIDTOXML_EXPORTS
#	define CIDTOXML_API __declspec(dllexport)
#else
#	define CIDTOXML_API __declspec(dllimport)
#endif

extern "C"
{
	CIDTOXML_API ICidToData* CreateModule(void* pIService);
	CIDTOXML_API  void DeleteModule(ICidToData* pModule);
}

 


 


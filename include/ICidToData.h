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
	std::string Type_;     //IED类型
	std::string Desc_;     //IED描述
	std::string Config_;   //版本
	std::string Manu_;     //制造商
	std::string Model_;    //型号

}stIedData;


typedef struct
{
	int ID_;
	std::string	RedisAddr_;
	std::string Name_;
	std::string Desc_;     //逻辑节点描述/数据对象描述
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

 


 


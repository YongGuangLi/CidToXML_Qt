#ifndef CIDTOXML_H
#define CIDTOXML_H

#ifdef CIDTOXML_LIB
# define CIDTOXML_EXPORT __declspec(dllexport)
#else
# define CIDTOXML_EXPORT __declspec(dllimport)
#endif

#include "../include/ICidToData.h"
#include <string>
#include <map>
#include <list>
#include <iostream>

class QDomDocument;
class QDomElement;
class QString;

using namespace std;

class  CidToData : public ICidToData
{
public: 
	CidToData();
	~CidToData(); 
	//加载cid文件
	int ConvertScd2Data(std::string csCidFile,std::string csInitFile, std::list<std::string> &lstErrors);
	map<string, stIedData> GetIedData();
    map<string, list<stPointData>> GetPointData();
private:

	//读取配置文件
	bool InitCfgFile(std::string csInitFile);
	
	//按照功能约束过滤,在配置文件中设置
	/*
		MX=0
		ST=1
		CO=2 
		EX=2
		SV=2
		CF=2
		DC=2
	*/
	map<string,string> mapFilterFC_;
	
	map<string,string> mapFilterType_;

	map<string,stIedData> mapIedData_;

    map<string, list<stPointData>> mapPointData_;
};
 
#endif // CIDTOXML_H

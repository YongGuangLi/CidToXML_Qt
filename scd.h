#ifndef SCD_H
#define SCD_H



#include <string>
#include <vector>
#include <map>
#include <QList>
#include "cidtoxml.h"

class QDomElement;


struct Header;
struct Substation;
struct Communication;
struct IED;
struct DataTypeTemplates;

using namespace std;

class SCD
{
public: 
    static SCD* instance();
    static void close_singleton();
    bool init(string &filename);
    void initDataSetToAddress();
	void initDoiToAddress();
    void clearAllData();
    map<string,vector<string> >& getDataSetToAddress();
	map<QString,vector<string> >& getDoiToAddress();

    DataTypeTemplates* getDataTypeTemplates();

	void getAllRptCtrlBlk(QList<QString> &listRptCtrlBlk);
	void getAllIedAddress(map<string, stIedData> &mapIedData);
	void GetAllIedDesc(map<string, stIedData> &mapIedData);
	void SetNetworkPoint(map<string, list<stPointData>> &mapPointData);

	void getErrorList(list<string> &listError);
private:
    Header *header_;
    map<string,Substation*> mpSubstation_;
    Communication *communication_;
    map<string,IED*> mpIED_;
    DataTypeTemplates *dataTypeTemplates_;

    bool bInit_;

    map<string,vector<string> > mpDataSetToDa_;
	map<QString,vector<string> > mpDoiToDa_;
	map<char, string> m_mapIedType;
    string filename_;

    static SCD *instance_; 
private:
	
    void initHeader(QDomElement& e);
    void initSubstation(QDomElement& e);
    void initCommunication(QDomElement& e);
    void initIED(QDomElement& e);
    void initDataTypeTemplates(QDomElement& e);

    SCD();
    ~SCD();

};

#endif // SCD_H

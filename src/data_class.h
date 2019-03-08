#ifndef DATACLASS_H
#define DATACLASS_H
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include "ogrsf_frmts.h"
using std::string;
using std::vector;

const int fieldsCountPOZEMLIMOTI = 21;
const int fieldsCountMESTNOSTI = 4;
const int fieldsCountPRAVA = 15;
const int fieldsCountPERSONS = 13;
const int fieldsCountAPARTS = 13;
const int fieldsCountSGRADI = 16;
const int fieldsCountOGRIMO = 7;

struct structReference
{
	double X = 0;
	double Y = 0;
	std::string coordtype = "";
 };

struct structCharArray
{
	vector<char>  pChars;
//	char* pChars;
	int sizeCharArray = 0;
	int currentChar = 0;
};

struct structConturAttr
{
	int typeContur;
	double pointInConturX;
	double pointInConturY;
};

struct structNadpisiAtrr
{
	int type; // ��� �� ������ �� ������������� � ���������� � 1;   featureBufferT.set_Value(T_TypeKlasificator, System.Convert.ToInt16(splitstringspace[1]));
	string code; //  � �������� ����� �� ������;   featureBufferT.set_Value(T_CodeText, System.Convert.ToInt32(splitstringspace[2]));
	double pointX; // �� ���������� �� ������;   double pointTextX = System.Convert.ToDouble(splitstringspace[4]) + referenceY;
	double pointY; // �� ���������� �� ������;   double pointTextY = System.Convert.ToDouble(splitstringspace[3]) + referenceX;
	int sizeFont; //  �������� �� ������� � ��������� �� ��������  //featureBufferT.set_Value(T_SizeFont, System.Convert.ToInt32(splitstringspace[5]));
	string begDate; //  � ���� �� ��������� ����� �� ������ ��� �����  //featureBufferT.set_Value(T_CreateDate, splitstringspace[6]);
	string endDate; //  � ���� �� ��������������� �� ��������� ������������ ������  //featureBufferT.set_Value(T_EndDate, splitstringspace[7]);
	double rotAngle; //  � ���� �� ��������� �� ������(100 ���� �� ������������)  //featureBufferT.set_Value(T_RotAngle, System.Convert.ToDouble(splitstringspace[8]));
	string alignment;// ��������� ��� �� ������������   //featureBufferT.set_Value(T_Alignment, splitstringspace[9]);
	string textNadpis;
};

struct structCadasterLayers
{
	std::map<int, std::string> cadasterLines;
	std::map<std::string, std::string> cadasterSgradi;
	std::map<std::string, structConturAttr> conturSgradiAttr;
	std::map<std::string, std::string> cadasterImoti;
	std::map<std::string, structConturAttr> conturImotiAttr;
	std::map<std::string, structNadpisiAtrr> nadpisiAttr;
	std::map<std::string, std::string> cadasterGranici;
	std::map<std::string, structConturAttr> conturGraniciAttr;

};

struct structLayerShemi
{
	std::map<std::string, std::string> shemiAparts;
	std::map<std::string, structConturAttr> conturShemiAttr;
};

struct structField
{
	string name;
	OGRFieldType ogrType;
	int lentgh;
	int flag; //1 - key, 2 -  ���� �� ������ � ������������, ��������� � ����� �� ������ name, 3 - ���� �� ������ � ������� � ��� �� ���� name �� ���� � ���
	string relTable; // name relation table, "name" is the key in this table
};

struct structFieldShape
{
	//structFieldShape() : name(), ogrType(), lentgh(), table() {}
	//structFieldShape(std::string newName, OGRFieldType newogrType, int newlentgh, string newtable)
	//	: name(newName), ogrType(newogrType), lentgh (newlentgh), table (newtable) {}
	string name;
	OGRFieldType ogrType;
	int lentgh;
	string table;
};

struct structDataPOZEMLIMOTI
{
////F name	type	Len	dec	flag	table	��� �� ������
	string IDENT;	//C	20	0	1		������������� �� ����
	int VIDT;		//S	1	0	2		��� ���������
	int VIDTOLD;		//S	1	0	2		��� ���������(����� ������������)
	int VIDS;		//S	2	0	2		��� �����������
	int NTP;		//S	4	0	2		����� �� ������ ��������
	int NTPOLD;		//S	4	0	2		����� �� ������ ��������(����� ������������)
	int MESTNOST;	//S	4	0	3	MESTNOSTI	��� �� ��������
	string PARTIDA;	//C	20	0			����� �� ������� �� ��
	int ADDRCODE;	//L	10	0	3	ADDRESS	����� �� �����
	string NOMER1;	//C	10	0			���� ������������� �����
	string KVARTAL;	//C	10	0			����� �� ������� �� ������������ ����
	string PARCEL; 	//C	10	0			����� �� ������  �� ������������ ���� � ������ �����
	double GODCAD;	//N	4	0			������ �� ������ ����������� ����
	double GODREG;	//N	4	0			������ �� ������ ������������ ����
	int CODZAP;		//S	4	0	3	ZAPOVEDI	��� �� ���������� ������� �� ����������
	int ZACON;		//S	2	0	2		��� �� �����, �� ����� �� ���������� ���������
	int KAT;			//S	2	0			������������� ��������� �� ������
	int NVAST;		//S	1	0	2		����� �� �������������� - �� ������������� �� �������� �� ��������������
	int VAVOD;		//B	1	0			���� �� ����� ��� ��������
	string BEG_DATE;	//D	10	0			���� �� �����������
	string END_DATE;	//D	10	0			���� �� �������������
};

struct structDataSGRADI
{
	////F name	type	Len	dec	flag	table	��� �� ������
	string IDENT;	//C	20	0	1		������������� �� ������
	int VIDS;		//S	2	0	2		��� �����������
	int CONST;		//S	2	0	2		����������� �� ��������
	int VFUNC;		//S	3	0	2		������������ �������������� �� ��������
	int VFUNCOLD;	//S	3	0	2		������������ �������������� �� �������� �(����� ������������)
	int GOD;		//S	4	0			������ �� ����������� �� ��������
	string PARTIDA;	//C	20	0			����� �� ������� �� ��
	string NOMER1;	//C	10	0			���� ������������� �����
	int ��;			//S	3	0			���� ����� �� ��������
	int ��1;		//S	3	0			���� ������������ �����
	int BRPOM;		//S	3	0			���� ������������� ������
	int ADDRCODE;	//L	10	0	3	ADDRESS	����� � ���
	int LEGAL;		//S	1	0			���� �� ��������� : 0 - ���� �����,	1 - ���������, 2 � �������
	string LEGALDOC;//C	20	0			�������� �� ����������� �� ���������
	string BEG_DATE;//D	10	0			���� �� �����������
	string END_DATE;//D	10	0			���� �� �������������
};

struct structDataPRAVA
{
	////F name	type	Len	dec	flag	table	��� �� ������
	string IDENT;	//C	20	0	1		������������� �� �������� ����, ������ ��� ������������� ����� � ������
	int VIDS;		//S	2	0	2		��� �����������
	string PERSON;	//C	13	0	3	PERSONS	��� / ������� �� �������� �� �������
	int DOCCOD;		//L	10	0	3	DOCS	��� �� ��������
	int DOCID1;	    //S	3	0			������� ���� : �������� �� ���������, -1 ��� ��������, -2 ��� ���� � ��.�.
	double DOCID2;	//N	8	3			������� ���� : ����������, ����� �� �������� ��� ������ � ��.�.����(� ���������� �� DOCID1)
	double PLDOC;	//N	11	3			���� �� ��������
	int PTYPE;		//S	2	0	2		����� �� �����������
	int PRAVOVID;	//S	2	0	2		��� �� ���� �����
	string SROK;	//D	10	0			������ ���� �� ������� ������ ������� � ������
	string DOCIDENT;	//C	30	0			����� �� ������ �� �������� - �������� �����, �������� ������� � ������, ��.����.����� � �.�.
	int DOP;		//B	1	0			���� �� ����������������� ��������(�����������); � � ������������, F � ������� ��������
	string BEG_DATE;	//D	10	0			���� �� ����������� �� �������
	string END_DATE;	//D	10	0			���� �� ������������ �� �������
	string END_TIME;	//T	5	0			�����(������) �� ������������ �� �������
};

struct structDataPERSONS
{
	////F name	type	Len	dec	flag	table	��� �� ������
	string PERSON;	// C	13	0	1		��� / ������� �� ����������(�� �� ������� ����� � ���� �� �������)
	int SUBTYPE;	// S	1	0	2		��� �� �������, ��� �� ������������
	string NAME;	// C	45	0			��� �� ����������
	string NSTATE;	// C	2	0	2		����� �� ����������� - ������ ��� �� ������
	int ADDRCODE;	// L	10	0	3	ADDRESS	����� �� ����������� - ��� �� �����
	string ADDR;	// C	50	0			����� ����� �� ����������� - ��������
	string ADDRET;	// C	4	0			����� �� ����������� � ����
	string ADDRAP;	// C	4	0			����� �� ����������� � ����������
	int FLAG;	    // B	1	0			���� �� ������� ����������
	string SPERSON;	// C	10	0			��� �� �������� ����(������ / ������� ��� ��������� ���� �� ��)
	string FIRMREG;	// C	50	0			����� �� ������� ����������� �� �����
	string BEG_DATE;// D	10	0			���� �� �����������
	string END_DATE;// D	10	0			���� �� �������������
};

struct structDataAPARTS
{
	////F name	type	Len	dec	flag	table	��� �� ������
	string IDENT;   //	C	20	0	1		������������� �� ������������� �����
	string REM;	    //  C	20	0			�������� �� ������
	int PREDN;	    //  S	3	0	2		�������������� �� ������������� �����
	int VIDS;	    //  S	2	0	2		��� �����������
	string PARTIDA;	//  C	20	0			����� �� ������� �� ��
	int ADDRCODE;	//  L	10	0	3	ADDRESS	����� � ���
	string ADDRET;	//  C	4	0			����� � ����
	string ADDRAP;  //	C	4	0			����� � ����������
	double PLDOC;	//  N	11	3			���� �� ��������
	int BRET;	    //  S	2	0			���� ����� �� ������
	string DOPS;	//  C	80	0			��� � ���� �� ����������� ��������� � ������ �����, ����� �� ���������� ���� �� ������(�������� �����)
	string BEG_DATE;//  D	10	0			���� �� �����������
	string END_DATE;//  D	10	0			���� �� �������������
};

struct structDataOGRPIMO
{
	////F name	type	Len	dec	flag	table	��� �� ������
	string IDENT;	//F IDENT    C  20 0 3 POZEMLIMOTI
	int TYPE;     //S   3 0 2
	int DOCCOD;   //L  10 0 3 DOCS
	int DOCCOD1;  //L  10 0 3 DOCS
	int STATUS;   //S   1 0 2
	string BEG_DATE;	//D	10	0			���� �� �����������
	string END_DATE;	//D	10	0			���� �� �������������
};

struct structDataMESTNOSTI
{
	////F name	type	Len	dec	flag	table	��� �� ������
	int MESTNOST;	//S	4	0	3	MESTNOSTI	��� �� ��������
	string NAME;	//C	20	0			����� �� ������� �� ��
	string BEG_DATE;	//D	10	0			���� �� �����������
	string END_DATE;	//D	10	0			���� �� �������������
};

struct structAttrTables
{
	std::map<std::string, std::vector<structField>> tableFields;
	std::map<std::string, std::multimap<std::string, std::string>> tableAttributes;
	std::map<std::string, structDataPOZEMLIMOTI> tableAttributesPOZEMLIMOTI;
	std::map<std::string, structDataSGRADI> tableAttributesSGRADI;
	std::map<int, structDataMESTNOSTI> tableAttributesMESTNOSTI;
	std::multimap<std::string, structDataPRAVA> tableAttributesPRAVA;
	std::map<std::string, structDataPERSONS> tableAttributesPERSONS;
	std::map<std::string, structDataAPARTS> tableAttributesAPARTS;
	std::multimap<std::string, structDataOGRPIMO> tableAttributesOGRPIMO;
};

struct structNomTABLEVIDS
{
	int VIDS;		//S	2	0	2		��� �����������	F VIDS S 2 0 2
	string VIDS_txt; // C 30 0 2
};

struct structNomTABLEVIDT
{
	int VIDT;		//S	2	0	2		��� �����������	F VIDS S 2 0 2
	string VIDT_txt; // C 30 0 2
};
struct structNomTABLENTP
{
	int NTP;		//S	2	0	2		��� �����������	F VIDS S 2 0 2
	string NTP_txt; // C 30 0 2
};

struct structNomTABLEVFUNC
{
	int VFUNC;		//S	2	0	2		��� �����������	F VIDS S 2 0 2
	string VFUNC_txt; // C 30 0 2
};
struct structNomTABLEPREDN
{
	int PREDN;		//S	2	0	2		��� �����������	F VIDS S 2 0 2
	string PREDN_txt; // C 30 0 2
};
struct structNomTABLEOGRPIMO_TYPE
{
	int TYPE;		//S	2	0	2		��� �����������	F VIDS S 2 0 2
	string TYPE_txt; // C 30 0 2
};

struct structNomTABLENADPISI
{
	int TYPE;		//S	2	0	2		��� �����������	F VIDS S 2 0 2
	string TYPE_txt; // C 30 0 2
};

struct structNomenclatureTables
{
	std::map<std::string, std::vector<structField>> tableFields;
	std::map<std::string, std::map<std::string, std::string>> tableNomenclatur�s;
	std::map<int, structNomTABLEVIDS> tableNomenclatur�sVIDS;
	std::map<int, structNomTABLEVIDT> tableNomenclatur�sVIDT;
	std::map<int, structNomTABLENTP> tableNomenclatur�sNTP;
	std::map<int, structNomTABLEVFUNC> tableNomenclatur�sVFUNC;
	std::map<int, structNomTABLEPREDN> tableNomenclatur�sPREDN;
	std::map<int, structNomTABLEOGRPIMO_TYPE> tableNomenclatur�sOGRPIMO_TYPE;
	std::map<int, structNomTABLENADPISI> tableNomenclatur�sNADPISI;
};

structCharArray readFile(const char* filename);

void populateLayerShemi(structCharArray &charArrayFromFile, structLayerShemi &layerShemi);

void populateHeader(structCharArray &charArrayFromFile, structReference  &reference, std::string nameShapeFile);


void populateCadasterLayers(structCharArray &charArrayFromFile, structCadasterLayers  &cadasterLayers);

void populateAttributeTable(structCharArray &charArrayFromFile, structAttrTables &attrTables, string tableName);


int populateNomenclatureTables(structNomenclatureTables &nomTables);

void populateCADEntyties(structCharArray &,
	structReference &,
	structCadasterLayers &,
	structLayerShemi &,
	structAttrTables &,
	std::string
	);


#endif
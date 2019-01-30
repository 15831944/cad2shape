#include "stdafx.h"
#include <iostream>  
#include <fstream>  
#include <string>
#include <cstring>
#include <stdlib.h>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <vector>
#include <iterator>
#include <map>
#include <sstream>
#include <chrono> 

#include "ogrsf_frmts.h"
#include "data_class.h"
#include "shapefileImoti.h"
#include "shapefileSgradi.h"
#include "shapefilePrava.h"
#include "shapefileShemi.h"
#include "shapefileOgrpimo.h"
#include "shapefileNadpisi.h"
#include "helper.h"
#include "helper_template.cpp"

structCharArray readFile(const char* filename)
{

	structCharArray temp_struct;
	// open the file:
	std::streampos fileSize;
	std::ifstream file(filename, std::ios::binary);

	// get its size:
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// read the data:
	std::vector<char> fileData(fileSize);
	file.read((char*)&fileData[0], fileSize);
	temp_struct.pChars = fileData;
	temp_struct.sizeCharArray = fileSize;
	return temp_struct;
}

void populateLayerShemi(structCharArray &charArrayFromFile, structLayerShemi &layerShemi){

	int codeLine;
	string identSgrada = "";
	double  etaj = 0;
	int typeGranica;
	double pointInConturX;
	double pointInConturY;

	string s = "";
	string sgradiCodeContur = "";
	string myInputstring;
	string identApart = "";
	string allCoordLine = "";
	string allCoordPolygon = "";
	string allCodeLines = "";
	string previousFirstSymbol = "";

	vector <string> splitstringspace;
	vector <string> splitstringsemicolon;
	vector <string>  splitstringspacelinescode;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
	std::map<int, std::string> apartLines;


	myInputstring = ReadLineFromBytes(charArrayFromFile);


	while ((charArrayFromFile.currentChar < charArrayFromFile.sizeCharArray) && (myInputstring != "END_LAYER"))
	{
		if (myInputstring != "")
		{
			splitstringspace = split(myInputstring, delimiterCharSpace);
			if (splitstringspace[0] == "ET")
			{
				previousFirstSymbol = "ET";
				identSgrada = trim(splitstringspace[1]);
				etaj = atod(splitstringspace[2]);
			}
			else if (splitstringspace[0] == "L")
			{
				if (previousFirstSymbol == "L")
				{
					apartLines[codeLine] = allCoordLine;
					allCoordLine = "";
				}
				previousFirstSymbol = "L";
				codeLine = atoi(splitstringspace[2]);

			}
			else if (IsLineNumber(myInputstring) && (previousFirstSymbol == "L"))
			{
				allCoordLine = allCoordLine + myInputstring;
			}

			else if (splitstringspace[0] == "C")
			{
				if (previousFirstSymbol == "L")
				{
					apartLines[codeLine] = allCoordLine;
					allCoordLine = "";
				}
				if (previousFirstSymbol == "C")
				{
					splitstringspacelinescode = split(allCodeLines, delimiterCharSpace);

					for (int i = 0; i < splitstringspacelinescode.size(); i++)
					{
						int lineCode = atoi(splitstringspacelinescode[i]);
						if (apartLines.find(lineCode) != apartLines.end())
						{
							s = ConcatLinesCoordinates(allCoordPolygon, apartLines[lineCode]);
							allCoordPolygon = s;
						}
					}
					layerShemi.shemiAparts[identApart] = allCoordPolygon;
					layerShemi.conturShemiAttr[identApart].typeContur = typeGranica;
					layerShemi.conturShemiAttr[identApart].pointInConturX = pointInConturX;
					layerShemi.conturShemiAttr[identApart].pointInConturY = pointInConturY;
					allCoordPolygon = "";
					allCodeLines = "";
				}

				previousFirstSymbol = "C";
				typeGranica = atoi(splitstringspace[1]);
				identApart = trim(splitstringspace[2]);
				pointInConturX = atod(splitstringspace[4]);
				pointInConturY = atod(splitstringspace[3]);
			}
			else if (IsLineNumber(myInputstring) && (previousFirstSymbol == "C"))
			{
				allCodeLines = allCodeLines + " " + myInputstring;
			}

			else if (myInputstring == "END_ET")
			{
				previousFirstSymbol = "END_ET";
				allCodeLines = trim(allCodeLines);
				splitstringspacelinescode = split(allCodeLines, delimiterCharSpace);

				for (int i = 0; i < splitstringspacelinescode.size(); i++)
				{
					int lineCode = atoi(splitstringspacelinescode[i]);
					if (apartLines.find(lineCode) != apartLines.end())
					{
						s = ConcatLinesCoordinates(allCoordPolygon, apartLines[lineCode]);
						allCoordPolygon = s;
					}
				}

				layerShemi.shemiAparts[identApart] = allCoordPolygon;
				layerShemi.conturShemiAttr[identApart].typeContur = typeGranica;
				layerShemi.conturShemiAttr[identApart].pointInConturX = pointInConturX;
				layerShemi.conturShemiAttr[identApart].pointInConturY = pointInConturY;
				allCoordPolygon = "";
				allCodeLines = "";
				apartLines.clear();
			}
		}
		myInputstring = ReadLineFromBytes(charArrayFromFile);
	}
}

void populateHeader(structCharArray &charArrayFromFile, structReference  &reference, std::string nameShapeFile){

	string myInputstring;
	vector <string> splitstringspace;
	vector <string> splitstringcomma;
	char delimiterCharSpace = ' ';
	char delimiterCharComma = ',';

	std::ofstream myfile;
	myfile.open(nameShapeFile + "_info.txt");

	myInputstring = ReadLineFromBytes(charArrayFromFile);

	while ((charArrayFromFile.currentChar < charArrayFromFile.sizeCharArray) && (myInputstring != "END_HEADER"))
	{
		//	out << myInputstring << endl;
		if (myInputstring != "")
		{
			splitstringspace = split(myInputstring, delimiterCharSpace);

			if (splitstringspace[0] == "REFERENCE")
			{
				reference.X = atod(splitstringspace[1]);
				reference.Y = atod(splitstringspace[2]);
			}
			if (splitstringspace[0] == "COORDTYPE")
			{
				splitstringcomma = split(splitstringspace[1], delimiterCharComma);
				reference.coordtype = splitstringcomma[0];
			}

		}
		myfile << myInputstring << "\n";
		myInputstring = ReadLineFromBytes(charArrayFromFile);
	}
	myfile.close();
}


void populateCadasterLayers(structCharArray &charArrayFromFile, structCadasterLayers  &cadasterLayers){
	int codeLine;
	string identSgrada = "";
	double  etaj = 0;
	int typeGranica;
	double pointInConturX;
	double pointInConturY;
	string s = "";
	string codeContur = "";
	string myInputstring;
	string identApart = "";
	string allCoordLine = "";
	string allCoordPolygon = "";
	string allCodeLines = "";
	string previousFirstSymbol = "";

	vector <string> splitstringspace;
	vector <string> splitstringspacenadpis;
	vector <string> splitstringsemicolon;
	vector <string>  splitstringspacelinescode;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
	char trimCharQUOTE = '\"';


	myInputstring = ReadLineFromBytes(charArrayFromFile);


	while ((charArrayFromFile.currentChar < charArrayFromFile.sizeCharArray) && (myInputstring != "END_LAYER"))
	{
		//	out << myInputstring << endl;
		if (myInputstring != "")
		{
			splitstringspace = split(myInputstring, delimiterCharSpace);

			if (splitstringspace[0] == "L")
			{
				if (previousFirstSymbol == "L")
				{
					cadasterLayers.cadasterLines[codeLine] = allCoordLine;
					allCoordLine = "";
				}
				previousFirstSymbol = "L";
				codeLine = atoi(splitstringspace[2]);
			}
			else if (IsLineNumber(myInputstring) && (previousFirstSymbol == "L"))
			{
				allCoordLine = allCoordLine + myInputstring;
			}

			else if (splitstringspace[0] == "C")
			{
				if (previousFirstSymbol == "L")
				{
					cadasterLayers.cadasterLines[codeLine] = allCoordLine;
					allCoordLine = "";
				}
				if (previousFirstSymbol == "C")
				{
					allCodeLines = trim(allCodeLines);
					splitstringspacelinescode = split(allCodeLines, delimiterCharSpace);

					for (int i = 0; i < splitstringspacelinescode.size(); i++)
					{
						int lineCode = atoi(splitstringspacelinescode[i]);
						if (cadasterLayers.cadasterLines.find(lineCode) != cadasterLayers.cadasterLines.end())
						{
							s = ConcatLinesCoordinates(allCoordPolygon, cadasterLayers.cadasterLines[lineCode]);
							allCoordPolygon = s;
						}
					}
					//					cout << codeContur << endl;
					if (typeGranica == 1)
					{
						cadasterLayers.cadasterSgradi[codeContur] = allCoordPolygon;
						cadasterLayers.conturSgradiAttr[codeContur].typeContur = typeGranica;
						cadasterLayers.conturSgradiAttr[codeContur].pointInConturX = pointInConturX;
						cadasterLayers.conturSgradiAttr[codeContur].pointInConturY = pointInConturY;
					}
					else if (typeGranica == 2)
					{
						cadasterLayers.cadasterImoti[codeContur] = allCoordPolygon;
						cadasterLayers.conturImotiAttr[codeContur].typeContur = typeGranica;
						cadasterLayers.conturImotiAttr[codeContur].pointInConturX = pointInConturX;
						cadasterLayers.conturImotiAttr[codeContur].pointInConturY = pointInConturY;
					}
					else
					{
						cadasterLayers.cadasterGranici[codeContur] = allCoordPolygon;
						cadasterLayers.conturGraniciAttr[codeContur].typeContur = typeGranica;
						cadasterLayers.conturGraniciAttr[codeContur].pointInConturX = pointInConturX;
						cadasterLayers.conturGraniciAttr[codeContur].pointInConturY = pointInConturY;
					}

					allCoordPolygon = "";
					allCodeLines = "";
				}

				previousFirstSymbol = "C";
				typeGranica = atoi(splitstringspace[1]);
				codeContur = trim(splitstringspace[2]);
				pointInConturX = atod(splitstringspace[4]);
				pointInConturY = atod(splitstringspace[3]);

			}
			else if (IsLineNumber(myInputstring) && (previousFirstSymbol == "C"))
			{
				allCodeLines = allCodeLines + " " + myInputstring;
			}

			else if (splitstringspace[0] == "T")
			{
				if (previousFirstSymbol == "C")
				{
					allCodeLines = trim(allCodeLines);
					splitstringspacelinescode = split(allCodeLines, delimiterCharSpace);

					for (int i = 0; i < splitstringspacelinescode.size(); i++)
					{
						int lineCode = atoi(splitstringspacelinescode[i]);
						if (cadasterLayers.cadasterLines.find(lineCode) != cadasterLayers.cadasterLines.end())
						{
							s = ConcatLinesCoordinates(allCoordPolygon, cadasterLayers.cadasterLines[lineCode]);
							allCoordPolygon = s;
						}
					}
					if (typeGranica == 1)
					{
						cadasterLayers.cadasterSgradi[codeContur] = allCoordPolygon;
						cadasterLayers.conturSgradiAttr[codeContur].typeContur = typeGranica;
						cadasterLayers.conturSgradiAttr[codeContur].pointInConturX = pointInConturX;
						cadasterLayers.conturSgradiAttr[codeContur].pointInConturY = pointInConturY;
					}
					else if (typeGranica == 2)
					{
						cadasterLayers.cadasterImoti[codeContur] = allCoordPolygon;
						cadasterLayers.conturImotiAttr[codeContur].typeContur = typeGranica;
						cadasterLayers.conturImotiAttr[codeContur].pointInConturX = pointInConturX;
						cadasterLayers.conturImotiAttr[codeContur].pointInConturY = pointInConturY;
					}
					else
					{
						cadasterLayers.cadasterGranici[codeContur] = allCoordPolygon;
						cadasterLayers.conturGraniciAttr[codeContur].typeContur = typeGranica;
						cadasterLayers.conturGraniciAttr[codeContur].pointInConturX = pointInConturX;
						cadasterLayers.conturGraniciAttr[codeContur].pointInConturY = pointInConturY;
					}

					allCoordPolygon = "";
					allCodeLines = "";
				}

				splitstringspacenadpis = splitstringspace;
				previousFirstSymbol = "T";

			}
			else if (previousFirstSymbol == "T")
			{
				structNadpisiAtrr t;
				t.type = atoi(splitstringspacenadpis[1]); //    featureBufferT.set_Value(T_TypeKlasificator, System.Convert.ToInt16(splitstringspace[1]));
				t.code = trim(splitstringspacenadpis[2]); //    featureBufferT.set_Value(T_CodeText, System.Convert.ToInt32(splitstringspace[2]));
				t.pointX = atod(splitstringspacenadpis[4]);
				t.pointY = atod(splitstringspacenadpis[3]);
				t.sizeFont = atoi(splitstringspacenadpis[5]);//    //featureBufferT.set_Value(T_SizeFont, System.Convert.ToInt32(splitstringspace[5]));
				t.begDate = trim(splitstringspacenadpis[6]);//    //featureBufferT.set_Value(T_CreateDate, splitstringspace[6]);
				t.endDate = trim(splitstringspacenadpis[7]);//    //featureBufferT.set_Value(T_EndDate, splitstringspace[7]);
				t.rotAngle = atod(splitstringspacenadpis[8]); //    //featureBufferT.set_Value(T_RotAngle, System.Convert.ToDouble(splitstringspace[8]));
				t.alignment = trim(splitstringspacenadpis[9]);//    //featureBufferT.set_Value(T_Alignment, splitstringspace[9]);
				t.textNadpis = trim(myInputstring, trimCharQUOTE);
				cadasterLayers.nadpisiAttr[t.code] = t;
			}
		}
		myInputstring = ReadLineFromBytes(charArrayFromFile);
	}
}

void populateAttributeTable(structCharArray &charArrayFromFile, structAttrTables &attrTables, string tableName)
{
	string myInputstring;
	string tableFields;
	vector <string> splitstringspace;
	vector <string> splitstringsemicolon;
	vector <string>  splitstringcomma;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
	char delimiterCharComma = ',';
	char trimCharD = 'D';
	char trimCharQUOTE = '\"';
	int fieldsCount = 0;

	myInputstring = ReadLineFromBytes(charArrayFromFile);

	while ((charArrayFromFile.currentChar < charArrayFromFile.sizeCharArray) && (myInputstring != "END_TABLE"))
	{
		if (myInputstring != "")
		{
			splitstringspace = split(myInputstring, delimiterCharSpace);

			if (splitstringspace[0] == "D")
			{
				myInputstring = trim(myInputstring, trimCharD);
				splitstringcomma = split(myInputstring, delimiterCharComma);
				string fieldIDENT = trim(splitstringcomma[0], trimCharQUOTE);
				if (tableName == "POZEMLIMOTI")
				{
					splitstringcomma = splitDataRow(myInputstring, fieldsCountPOZEMLIMOTI);
					structDataPOZEMLIMOTI t;
					t.IDENT = trim(splitstringcomma[0], trimCharQUOTE);	//C	20	0	1		������������� �� ����
					t.VIDT = atoi(splitstringcomma[1]);		//S	1	0	2		��� ���������
					t.VIDTOLD = atoi(splitstringcomma[2]);		//S	1	0	2		��� ���������(����� ������������)
					t.VIDS = atoi(splitstringcomma[3]);		//S	2	0	2		��� �����������
					t.NTP = atoi(splitstringcomma[4]);		//S	4	0	2		����� �� ������ ��������
					t.NTPOLD = atoi(splitstringcomma[5]);		//S	4	0	2		����� �� ������ ��������(����� ������������)
					t.MESTNOST = atoi(splitstringcomma[6]);	//S	4	0	3	MESTNOSTI	��� �� ��������
					t.PARTIDA = trim(splitstringcomma[7], trimCharQUOTE);	//C	20	0			����� �� ������� �� ��
					t.ADDRCODE = atoi(splitstringcomma[8]);	//L	10	0	3	ADDRESS	����� �� �����
					t.NOMER1 = trim(splitstringcomma[9], trimCharQUOTE);	//C	10	0			���� ������������� �����
					t.KVARTAL = trim(splitstringcomma[10], trimCharQUOTE);	//C	10	0			����� �� ������� �� ������������ ����
					t.PARCEL = trim(splitstringcomma[11], trimCharQUOTE); 	//C	10	0			����� �� ������  �� ������������ ���� � ������ �����
					t.GODCAD = atod(splitstringcomma[12]);	//N	4	0			������ �� ������ ����������� ����
					t.GODREG = atod(splitstringcomma[13]);	//N	4	0			������ �� ������ ������������ ����
					t.CODZAP = atoi(splitstringcomma[14]);		//S	4	0	3	ZAPOVEDI	��� �� ���������� ������� �� ����������
					t.ZACON = atoi(splitstringcomma[15]);		//S	2	0	2		��� �� �����, �� ����� �� ���������� ���������
					t.KAT = atoi(splitstringcomma[16]);			//S	2	0			������������� ��������� �� ������
					t.NVAST = atoi(splitstringcomma[17]);		//S	1	0	2		����� �� �������������� - �� ������������� �� �������� �� ��������������
					t.VAVOD = atoi(splitstringcomma[18]);		//B	1	0			���� �� ����� ��� ��������
					t.BEG_DATE = trim(splitstringcomma[19], trimCharQUOTE);	//D	10	0			���� �� �����������
					t.END_DATE = trim(splitstringcomma[20], trimCharQUOTE);	//D	10	0			���� �� �������������

					attrTables.tableAttributesPOZEMLIMOTI[t.IDENT] = t;
				}
				else if (tableName == "SGRADI")
				{
					splitstringcomma = splitDataRow(myInputstring, fieldsCountSGRADI);
					structDataSGRADI t;

					////F name	type	Len	dec	flag	table	��� �� ������
					t.IDENT = trim(splitstringcomma[0], trimCharQUOTE);	//C	20	0	1		������������� �� ������
					t.VIDS = atoi(splitstringcomma[1]);		//S	2	0	2		��� �����������
					t.CONST = atoi(splitstringcomma[2]);	//S	2	0	2		����������� �� ��������
					t.VFUNC = atoi(splitstringcomma[3]);	//S	3	0	2		������������ �������������� �� ��������
					t.VFUNCOLD = atoi(splitstringcomma[4]);	//S	3	0	2		������������ �������������� �� �������� �(����� ������������)
					t.GOD = atoi(splitstringcomma[5]);		//S	4	0			������ �� ����������� �� ��������
					t.PARTIDA = trim(splitstringcomma[6], trimCharQUOTE);	//C	20	0			����� �� ������� �� ��
					t.NOMER1 = trim(splitstringcomma[7], trimCharQUOTE);	//C	10	0			���� ������������� �����
					t.�� = atoi(splitstringcomma[8]);	//S	3	0			���� ����� �� ��������
					t.��1 = atoi(splitstringcomma[9]);	//S	3	0			���� ������������ �����
					t.BRPOM = atoi(splitstringcomma[10]);	//S	3	0			���� ������������� ������
					t.ADDRCODE = atoi(splitstringcomma[11]);	//L	10	0	3	ADDRESS	����� � ���
					t.LEGAL = atoi(splitstringcomma[12]);	//S	1	0			���� �� ��������� : 0 - ���� �����,	1 - ���������, 2 � �������
					t.LEGALDOC = trim(splitstringcomma[13], trimCharQUOTE);	//C	20	0			�������� �� ����������� �� ���������
					t.BEG_DATE = trim(splitstringcomma[14], trimCharQUOTE); //D	10	0			���� �� �����������
					t.END_DATE = trim(splitstringcomma[15], trimCharQUOTE);	//D	10	0			���� �� �������������
					attrTables.tableAttributesSGRADI[t.IDENT] = t;
				}
				else if (tableName == "PRAVA")
				{
					splitstringcomma = splitDataRow(myInputstring, fieldsCountPRAVA);
					structDataPRAVA t;

					t.IDENT = trim(splitstringcomma[0], trimCharQUOTE);	//C	20	0	1		������������� �� �������� ����, ������ ��� ������������� ����� � ������
					t.VIDS = atoi(splitstringcomma[1]);		//S	2	0	2		��� �����������
					t.PERSON = trim(splitstringcomma[2], trimCharQUOTE);	//C	13	0	3	PERSONS	��� / ������� �� �������� �� �������
					t.DOCCOD = atoi(splitstringcomma[3]);		//L	10	0	3	DOCS	��� �� ��������
					t.DOCID1 = atoi(splitstringcomma[4]);	    //S	3	0			������� ���� : �������� �� ���������, -1 ��� ��������, -2 ��� ���� � ��.�.
					t.DOCID2 = atod(splitstringcomma[5]);	//N	8	3			������� ���� : ����������, ����� �� �������� ��� ������ � ��.�.����(� ���������� �� DOCID1)
					t.PLDOC = atod(splitstringcomma[6]);	//N	11	3			���� �� ��������
					t.PTYPE = atoi(splitstringcomma[7]);		//S	2	0	2		����� �� �����������
					t.PRAVOVID = atoi(splitstringcomma[8]);	//S	2	0	2		��� �� ���� �����
					t.SROK = trim(splitstringcomma[9], trimCharQUOTE);	//D	10	0			������ ���� �� ������� ������ ������� � ������
					t.DOCIDENT = trim(splitstringcomma[10], trimCharQUOTE);	//C	30	0			����� �� ������ �� �������� - �������� �����, �������� ������� � ������, ��.����.����� � �.�.
					t.DOP = atoi(splitstringcomma[11]);		//B	1	0			���� �� ����������������� ��������(�����������); � � ������������, F � ������� ��������
					t.BEG_DATE = trim(splitstringcomma[12], trimCharQUOTE);	//D	10	0			���� �� ����������� �� �������
					t.END_DATE = trim(splitstringcomma[13], trimCharQUOTE);	//D	10	0			���� �� ������������ �� �������
					t.END_TIME = trim(splitstringcomma[14], trimCharQUOTE);	//T	5	0			�����(������) �� ������������ �� �������

					attrTables.tableAttributesPRAVA.insert(pair<string, structDataPRAVA>(fieldIDENT, t));
				}
				else if (tableName == "OGRPIMO")
				{
					splitstringcomma = splitDataRow(myInputstring, fieldsCountOGRIMO);
					structDataOGRPIMO t;

					////F name	type	Len	dec	flag	table	��� �� ������
					t.IDENT = trim(splitstringcomma[0], trimCharQUOTE); 	//F IDENT    C  20 0 3 POZEMLIMOTI
					t.TYPE = atoi(splitstringcomma[1]);      //S   3 0 2
					t.DOCCOD = atoi(splitstringcomma[2]);    //L  10 0 3 DOCS
					t.DOCCOD1 = atoi(splitstringcomma[3]);   //L  10 0 3 DOCS
					t.STATUS = atoi(splitstringcomma[4]);    //S   1 0 2
					t.BEG_DATE = trim(splitstringcomma[5], trimCharQUOTE);	//D	10	0			���� �� ����������� �� �������
					t.END_DATE = trim(splitstringcomma[6], trimCharQUOTE);	//D	10	0			���� �� ������������ �� �������

					attrTables.tableAttributesOGRPIMO.insert(pair<string, structDataOGRPIMO>(t.IDENT, t));
				}
				else if (tableName == "PERSONS")
				{
					splitstringcomma = splitDataRow(myInputstring, fieldsCountPERSONS);
					structDataPERSONS t;
					t.PERSON = trim(splitstringcomma[0], trimCharQUOTE);	// C	13	0	1		��� / ������� �� ����������(�� �� ������� ����� � ���� �� �������)
					t.SUBTYPE = atoi(splitstringcomma[1]);	// S	1	0	2		��� �� �������, ��� �� ������������
					t.NAME = trim(splitstringcomma[2], trimCharQUOTE);	// C	45	0			��� �� ����������
					t.NSTATE = trim(splitstringcomma[3], trimCharQUOTE);	// C	2	0	2		����� �� ����������� - ������ ��� �� ������
					t.ADDRCODE = atoi(splitstringcomma[4]);	// L	10	0	3	ADDRESS	����� �� ����������� - ��� �� �����
					t.ADDR = trim(splitstringcomma[5], trimCharQUOTE);	// C	50	0			����� ����� �� ����������� - ��������
					t.ADDRET = trim(splitstringcomma[6], trimCharQUOTE);	// C	4	0			����� �� ����������� � ����
					t.ADDRAP = trim(splitstringcomma[7], trimCharQUOTE);	// C	4	0			����� �� ����������� � ����������
					t.FLAG = atoi(splitstringcomma[8]);	    // B	1	0			���� �� ������� ����������
					t.SPERSON = trim(splitstringcomma[9], trimCharQUOTE);	// C	10	0			��� �� �������� ����(������ / ������� ��� ��������� ���� �� ��)
					t.FIRMREG = trim(splitstringcomma[10], trimCharQUOTE);	// C	50	0			����� �� ������� ����������� �� �����
					t.BEG_DATE = trim(splitstringcomma[11], trimCharQUOTE);// D	10	0			���� �� �����������
					t.END_DATE = trim(splitstringcomma[12], trimCharQUOTE);// D	10	0			���� �� �������������

					attrTables.tableAttributesPERSONS[t.PERSON] = t;
				}
				else if (tableName == "APARTS")
				{
					splitstringcomma = splitDataRow(myInputstring, fieldsCountAPARTS);
					structDataAPARTS t;

					t.IDENT = trim(splitstringcomma[0], trimCharQUOTE);   //	C	20	0	1		������������� �� ������������� �����
					t.REM = trim(splitstringcomma[1], trimCharQUOTE);     //  C	20	0			�������� �� ������
					t.PREDN = atoi(splitstringcomma[2]); 	    //  S	3	0	2		�������������� �� ������������� �����
					t.VIDS = atoi(splitstringcomma[3]);	    //  S	2	0	2		��� �����������
					t.PARTIDA = trim(splitstringcomma[4], trimCharQUOTE);	//  C	20	0			����� �� ������� �� ��
					t.ADDRCODE = atoi(splitstringcomma[5]);	//  L	10	0	3	ADDRESS	����� � ���
					t.ADDRET = trim(splitstringcomma[6], trimCharQUOTE); //  C	4	0			����� � ����
					t.ADDRAP = trim(splitstringcomma[7], trimCharQUOTE);  //	C	4	0			����� � ����������
					t.PLDOC = atod(splitstringcomma[8]);	//  N	11	3			���� �� ��������
					t.BRET = atoi(splitstringcomma[9]);	    //  S	2	0			���� ����� �� ������
					t.DOPS = trim(splitstringcomma[10], trimCharQUOTE);	//  C	80	0			��� � ���� �� ����������� ��������� � ������ �����, ����� �� ���������� ���� �� ������(�������� �����)
					t.BEG_DATE = trim(splitstringcomma[11], trimCharQUOTE);// D	10	0			���� �� �����������
					t.END_DATE = trim(splitstringcomma[12], trimCharQUOTE);// D	10	0			���� �� �������������

					attrTables.tableAttributesAPARTS[t.IDENT] = t;
				}
				else if (tableName == "MESTNOSTI")
				{
					splitstringcomma = splitDataRow(myInputstring, fieldsCountMESTNOSTI);
					structDataMESTNOSTI t;
					t.MESTNOST = atoi(splitstringcomma[0]);	//S	4	0	3	MESTNOSTI	��� �� ��������
					t.NAME = trim(splitstringcomma[1], trimCharQUOTE);	//C	20	0			
					t.BEG_DATE = trim(splitstringcomma[2], trimCharQUOTE);	//D	10	0			���� �� �����������
					t.END_DATE = trim(splitstringcomma[3], trimCharQUOTE);	//D	10	0			���� �� �������������

					attrTables.tableAttributesMESTNOSTI[t.MESTNOST] = t;
				}
			}
		}
		myInputstring = ReadLineFromBytes(charArrayFromFile);
	}
}

int populateNomenclatureTables(structNomenclatureTables &nomTables)
{

	string myInputstring;
	string tableFields;
	vector <string> splitstringspace;
	vector <string> splitstringsemicolon;
	vector <string>  splitstringcomma;
	char delimiterCharSpace = ' ';
	char delimiterCharSemicolon = ';';
	char delimiterCharComma = ',';
	char trimCharD = 'D';
	char trimCharQUOTE = '\"';
	int fieldsCount = 0;

	std::ifstream src("nomenclatures.txt");
	if (!src)
	{
		perror("Error opening file nomenclatures.txt");
		system("pause");
		return -1;
	}
	const char * filenameNomenclature = "nomenclatures.txt";
	structCharArray charArrayFromFile = readFile(filenameNomenclature);

	while (charArrayFromFile.currentChar < charArrayFromFile.sizeCharArray)
	{
		myInputstring = ReadLineFromBytes(charArrayFromFile);
		if (myInputstring != "")
		{
			splitstringspace = split(myInputstring, delimiterCharSpace);
			if (splitstringspace[0] == "TABLE")
			{
				string tableName = splitstringspace[1];
				while ((charArrayFromFile.currentChar < charArrayFromFile.sizeCharArray) && (myInputstring != "END_TABLE"))
				{
					if (myInputstring != "")
					{
						splitstringspace = split(myInputstring, delimiterCharSpace);

						if (splitstringspace[0] == "D")
						{
							myInputstring = trim(myInputstring, trimCharD);
							splitstringsemicolon = split(myInputstring, delimiterCharSemicolon);
							string fieldIDENT = trim(splitstringsemicolon[0], trimCharQUOTE);
							if (tableName == "VIDS")
							{
								structNomTABLEVIDS t;
								t.VIDS = atoi(splitstringsemicolon[0]);
								t.VIDS_txt = trim(splitstringsemicolon[1], trimCharQUOTE);

								nomTables.tableNomenclatur�sVIDS[t.VIDS] = t;
							}
							else if (tableName == "VIDT")
							{
								structNomTABLEVIDT t;
								t.VIDT = atoi(splitstringsemicolon[0]);
								t.VIDT_txt = trim(splitstringsemicolon[1], trimCharQUOTE);

								nomTables.tableNomenclatur�sVIDT[t.VIDT] = t;
							}
							else if (tableName == "NTP")
							{
								structNomTABLENTP t;
								t.NTP = atoi(splitstringsemicolon[0]);
								t.NTP_txt = trim(splitstringsemicolon[1], trimCharQUOTE);

								nomTables.tableNomenclatur�sNTP[t.NTP] = t;
							}
							else if (tableName == "VFUNC")
							{
								structNomTABLEVFUNC t;
								t.VFUNC = atoi(splitstringsemicolon[0]);
								t.VFUNC_txt = trim(splitstringsemicolon[1], trimCharQUOTE);

								nomTables.tableNomenclatur�sVFUNC[t.VFUNC] = t;
							}
							else if (tableName == "PREDN")
							{
								structNomTABLEPREDN t;
								t.PREDN = atoi(splitstringsemicolon[0]);
								t.PREDN_txt = trim(splitstringsemicolon[1], trimCharQUOTE);

								nomTables.tableNomenclatur�sPREDN[t.PREDN] = t;
							}
							else if (tableName == "OGRPIMO_TYPE")
							{
								structNomTABLEOGRPIMO_TYPE t;
								t.TYPE = atoi(splitstringsemicolon[0]);
								t.TYPE_txt = trim(splitstringsemicolon[1], trimCharQUOTE);

								nomTables.tableNomenclatur�sOGRPIMO_TYPE[t.TYPE] = t;
							}
							else if (tableName == "NADPISI")
							{
								structNomTABLENADPISI t;
								t.TYPE = atoi(splitstringsemicolon[0]);
								t.TYPE_txt = trim(splitstringsemicolon[1], trimCharQUOTE);

								nomTables.tableNomenclatur�sNADPISI[t.TYPE] = t;
							}
						}
					}

					myInputstring = ReadLineFromBytes(charArrayFromFile);
				}


			}
		}
	}

}


void populateCADEntyties(structCharArray &charArrayFromFile,
	structReference &reference,
	structCadasterLayers &cadasterLayers,
	structLayerShemi &layerShemi,
	structAttrTables &attrTables,
	std::string nameShapeFile
	)
{
	string myInputstring;
	char delimiterCharSpace = ' ';
	vector <string> splitstringspace;


	while (charArrayFromFile.currentChar < charArrayFromFile.sizeCharArray)
	{
		myInputstring = ReadLineFromBytes(charArrayFromFile);
		//	out << myInputstring << endl;
		if (myInputstring != "")
		{
			splitstringspace = split(myInputstring, delimiterCharSpace);

			if (splitstringspace[0] == "HEADER")
			{
				populateHeader(charArrayFromFile, reference, nameShapeFile);
				//reference.X = atod(splitstringspace[1]);
				//reference.Y = atod(splitstringspace[2]);
			}
			else if ((splitstringspace[0] == "LAYER") && (splitstringspace[1] == "CADASTER"))
			{
				populateCadasterLayers(charArrayFromFile, cadasterLayers);
			}
			//else if ((splitstringspace[0] == "CONTROL") && (splitstringspace[1] == "CADASTER"))
			//{
			//  AddDocumPlostToImot(lines, ref currentLine, workspace);
			//}
			else if ((splitstringspace[0] == "LAYER") && (splitstringspace[1] == "SHEMI"))
			{
				populateLayerShemi(charArrayFromFile, layerShemi);
			}
			else if (splitstringspace[0] == "TABLE")
			{
				string tableName = splitstringspace[1];
				populateAttributeTable(charArrayFromFile, attrTables, tableName);
			}
			//else
			//{
			//  currentLine += 1;
			//}
		}
	}
}


#pragma once
#include<fstream>
#include<string>
#include<cstring>
#include<iostream>
#include<cmath>
using namespace std;
class Operation {
public:
	char tableName[30];
	char binFileName[50];
	int rowNum;
	char rowName[20][30];
	bool bool_type[20];//bool_type==0(int) bool_type==1(bool)
	int primaryKeyNum;
	int record_num;
	int infoSize;
	int recordSize;

	Operation() {
		memset(tableName, 0, sizeof(tableName));
		recordSize = 0;
	}
	void buildBinFileName();//由用户输入的表名获取二进制文件名
	void getInfo();//从二进制文件中读出预存的表头信息
	void saveInfo();//用户新建表时会输入表的信息，将信息存到二进制文件中（仅仅在用户新建表时调用）
	void cinTableName(char tablename[30]);//用户进行增删查改操作必须先输入表名
	void buildNewTable(char tablename[30], int rownum, char rowname[20][30], char booltype[20], int primarykeynum);//用户新建表时输入表的信息
	bool insertByFile(char FileName[30]);//手动插入
	bool insertByHands(char cinData[40][30]);//文件插入
	void upDate();//增删查改操作可能会使表头信息变化，需要更新（目前只写了更新记录数目）
};

int char_to_int(char source[30]);
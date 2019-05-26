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
	void buildBinFileName();//���û�����ı�����ȡ�������ļ���
	void getInfo();//�Ӷ������ļ��ж���Ԥ��ı�ͷ��Ϣ
	void saveInfo();//�û��½���ʱ����������Ϣ������Ϣ�浽�������ļ��У��������û��½���ʱ���ã�
	void cinTableName(char tablename[30]);//�û�������ɾ��Ĳ����������������
	void buildNewTable(char tablename[30], int rownum, char rowname[20][30], char booltype[20], int primarykeynum);//�û��½���ʱ��������Ϣ
	bool insertByFile(char FileName[30]);//�ֶ�����
	bool insertByHands(char cinData[40][30]);//�ļ�����
	void upDate();//��ɾ��Ĳ������ܻ�ʹ��ͷ��Ϣ�仯����Ҫ���£�Ŀǰֻд�˸��¼�¼��Ŀ��
};

int char_to_int(char source[30]);
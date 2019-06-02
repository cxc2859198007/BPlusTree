#ifndef OPERATION_H
#define OPERATION_H

#include<fstream>
#include<string>
#include<cstring>
#include<iostream>
#include<cmath>
using namespace std;
const int maxResultNum = 10000000;
const int showSize = 10000;
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

	int shownum;
    int result_nums;
    char show[showSize][20][30];

	Operation() {
		memset(tableName, 0, sizeof(tableName));
		memset(binFileName, 0, sizeof(binFileName));
		rowNum = 0;
		memset(rowName, 0, sizeof(rowName));
		memset(bool_type, false, sizeof(bool_type));
		primaryKeyNum = 0;
		record_num = 0;
		infoSize = 0;
		recordSize = 0;
	}

	void buildBinFileName();
	/*
		��;��ͨ������tableName[30]��ô洢�˱���Ϣ�Ķ������ļ����ݿ��ַ
		�������ͣ���
		����ֵ����
	*/

	void getInfo();
	/*
		��;���Ӷ������ļ��ж�ȡ��ͷ��Ϣ
		�������ͣ���
		����ֵ����
	*/

	void saveInfo();
	/*
		��;���û��½���ʱ�������ͷ��Ϣ������Ϣ�浽�������ļ��У��������û��½���ʱ���ã�
		�������ͣ���
		����ֵ����
	*/

	void cinTableName(char tablename[30]);
	/*
		��;���û��½�����ȡ�ɱ�ʱ���ã�ȷ��tableName[30]��֮�����ɾ��Ķ����������
		�������ͣ���
		����ֵ����
	*/

	void buildNewTable(char tablename[30], int rownum, char rowname[20][30], int booltype[20], int primarykeynum);
	/*
		��;���û��½���ʱ��������Ϣ��ͬʱ����B+��
		�������ͣ����� ������ ������ ��������(int or char[30]) ������
		����ֵ����
	*/

	void chooseOldTable(char tablename[30]);
	/*
		��;���û�ѡȡһ���������ݿ����
		�������ͣ�����
		����ֵ����
	*/

	bool insertByFile(char FileName[50]);
	/*
		��;����txt���ȡ���ݣ��������ݿ���
		�������ͣ�txt�ļ���ַ
		����ֵ�����txt�ļ��޷���ȡ������false�����򷵻�true
	*/

	bool insertByHands(char cinData[20][30]);
	/*
		��;���û��ֶ��������ݣ��������ݿ���
		�������ͣ�ÿһ�����Ե�ֵ
		����ֵ������޷����룬����false�����򷵻�true
	*/

	void upDateInfo();
	/*
		��;�����±�ͷ��Ϣ����ɾ��ı��¼������
		�������ͣ���
		����ֵ����
	*/

	void showRecord(long* adrs, int adr_num);
	/*
		��;�������¼
		�������ͣ�ades��ż�¼�ڶ������ļ��е�λ�� adr_numΪ��¼����
		����ֵ����
	*/

	long getFirstAdr(int row);
	/*
		��;��
		�������ͣ�
		����ֵ��
	*/

	int search_adr(int row, char target[30], long*& result_adr, int rel = 1);
	/*
		��;������row������ֵ����target�����ļ�¼λ�ô����result_adr��
		�������ͣ�rel=1�жϵ��� rel=2�ж�С�� rel=3�ж�С�ڵ��� rel=4�жϴ��� rel=5�жϴ��ڵ���rel=6�ж��ַ�������ƥ��
				  row=0 ��ѯ�ӵ�target����¼��ʼ��1000����¼
		����ֵ�����ؼ�¼����
	*/

    bool search(int row, char target[30],int rel = 1);
	/*
		��;�����ҵ�row����������target�����ļ�¼
		�������ͣ�relͬsearch_adr��һ��
		����ֵ�����һ�����ϵļ�¼��û�У�����false�����򣬷���true
	*/

	void deleteInFile(long* adrs, int adr_num);
	/*
		��;��ɾ�������ݿ��ļ��еļ�¼
		�������ͣ�adrsΪҪɾ���ļ�¼���ļ��е�λ�ã�adr_numΪ��¼����
		����ֵ����
	*/

	void deleteInBPlusTree(long* adrs, int adr_num);
	/*
		��;��ɾ����B+���еļ�¼
		�������ͣ�adrsΪҪɾ���ļ�¼���ļ��е�λ�ã�adr_numΪ��¼����
		����ֵ����
	*/

	bool deletee(int row, char target[30], int rel = 1);
	/*
		��;��ɾ����row����������target�����ļ�¼
		�������ͣ�relͬsearch_adr��һ��
		����ֵ�����һ������ɾ�������ļ�¼��û�У�����false�����򣬷���true
	*/

	void upDateRecord(long adr, char reviseData[30], bool type);
	/*
		��;�������ݿ��ļ���λ��Ϊadr����ֵ�޸�ΪreviseData
		�������ͣ�typeΪ��������
		����ֵ����
	*/

	bool revise(int searchRow, char target[30], char reviseData[30], int reviseRow, int rel = 1);
	/*
		��;������searchRow����������target�����ļ�¼�ĵ�reviseRow�����Ը�ΪreviseData
		�������ͣ�relͬsearch_adr��һ��
		����ֵ�����һ�����ϸ��������ļ�¼��û�У�����false�����򣬷���true
	*/

	void saveBPlusTree();

    void int_to_char(int a,char b[]);
};

int char_to_int(char source[30]);
/*
	��;�����ַ���ת��Ϊ����
	�������ͣ�Ҫת�����ַ���
	����ֵ��ת���������
*/

bool partial_search(char* source, int destLen, char* target, int targetLen);//�ַ����鲿��ƥ��
#endif

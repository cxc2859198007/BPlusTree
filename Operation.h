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

	bool insertByFile(char FileName[30]);
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
		�������ͣ�ades[1000]��ż�¼�ڶ������ļ��е�λ�� adr_numΪ��¼����
		����ֵ����
	*/

	long getFirstAdr(int row);
	/*
		��;��
		�������ͣ�
		����ֵ��
	*/

	int search_adr(int row, char target[30], long*& result_adr);
	/*
		��;������row������ֵΪtarget�ļ�¼λ�ô����result_adr��
		�������ͣ�����;
		����ֵ�����ؼ�¼����
	*/

	bool search(int row, char target[30]);
	/*
		��;�����ҵ�row������Ϊtarget�ļ�¼
		�������ͣ�����;
		����ֵ�����һ�����ϵļ�¼��û�У�����false�����򣬷���true
	*/

	void deleteInFile(long* adrs, int adr_num);
	/*
		��;��ɾ�������ݿ��ļ��еļ�¼
		�������ͣ�adrs[1000]ΪҪɾ���ļ�¼���ļ��е�λ�ã�adr_numΪ��¼����
		����ֵ����
	*/

	bool deletee(int row, char target[30]);
	/*
		��;��ɾ����row������Ϊtarget�ļ�¼
		�������ͣ�����;
		����ֵ�����һ������ɾ�������ļ�¼��û�У�����false�����򣬷���true
	*/

	void upDateRecord(long adr, char reviseData[30], bool type);
	/*
		��;�������ݿ��ļ���λ��Ϊadr����ֵ�޸�ΪreviseData
		�������ͣ�typeΪ��������
		����ֵ����
	*/
	
	bool revise(int searchRow, char target[30], char reviseData[30], int reviseRow);
	/*
		��;������searchRow������Ϊtarget�ļ�¼�ĵ�reviseRow�����Ը�ΪreviseData
		�������ͣ�����;
		����ֵ�����һ�����ϸ��������ļ�¼��û�У�����false�����򣬷���true
	*/

	void saveBPlusTree();
};

int char_to_int(char source[30]);
/*
	��;�����ַ���ת��Ϊ����
	�������ͣ�Ҫת�����ַ���
	����ֵ��ת���������
*/
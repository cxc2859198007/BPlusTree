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
		用途：通过表名tableName[30]获得存储此表信息的二进制文件数据库地址
		参数解释：无
		返回值：无
	*/
	
	void getInfo();
	/*
		用途：从二进制文件中读取表头信息
		参数解释：无
		返回值：无
	*/
	
	void saveInfo();
	/*
		用途：用户新建表时会输入表头信息，将信息存到二进制文件中（仅仅在用户新建表时调用）
		参数解释：无
		返回值：无
	*/
	
	void cinTableName(char tablename[30]);
	/*
		用途：用户新建表或读取旧表时调用，确定tableName[30]，之后的增删查改都在这个表上
		参数解释：无
		返回值：无
	*/

	void buildNewTable(char tablename[30], int rownum, char rowname[20][30], int booltype[20], int primarykeynum);
	/*
		用途：用户新建表时输入表的信息，同时建立B+树
		参数解释：表名 属性数 属性名 属性类型(int or char[30]) 主码标号
		返回值：无
	*/
	
	void chooseOldTable(char tablename[30]);
	/*
		用途：用户选取一个已有数据库操作
		参数解释：表名
		返回值：无
	*/

	bool insertByFile(char FileName[30]);
	/*
		用途：从txt里读取数据，存入数据库中
		参数解释：txt文件地址
		返回值：如果txt文件无法读取，返回false；否则返回true
	*/
	
	bool insertByHands(char cinData[20][30]);
	/*
		用途：用户手动输入数据，存入数据库中
		参数解释：每一个属性的值
		返回值：如果无法插入，返回false；否则返回true
	*/

	void upDateInfo();
	/*
		用途：更新表头信息（增删会改变记录数量）
		参数解释：无
		返回值：无
	*/

	void showRecord(long* adrs, int adr_num);
	/*
		用途：输出记录
		参数解释：ades[1000]存放记录在二进制文件中的位置 adr_num为记录数量
		返回值：无
	*/

	long getFirstAdr(int row);
	/*
		用途：
		参数解释：
		返回值：
	*/

	int search_adr(int row, char target[30], long*& result_adr);
	/*
		用途：将第row个属性值为target的记录位置存放在result_adr中
		参数解释：见用途
		返回值：返回记录数量
	*/

	bool search(int row, char target[30]);
	/*
		用途：查找第row个属性为target的记录
		参数解释：见用途
		返回值：如果一条符合的记录都没有，返回false；否则，返回true
	*/

	void deleteInFile(long* adrs, int adr_num);
	/*
		用途：删除在数据库文件中的记录
		参数解释：adrs[1000]为要删除的记录在文件中的位置，adr_num为记录数量
		返回值：无
	*/

	bool deletee(int row, char target[30]);
	/*
		用途：删除第row个属性为target的记录
		参数解释：见用途
		返回值：如果一条符合删除条件的记录都没有，返回false；否则，返回true
	*/

	void upDateRecord(long adr, char reviseData[30], bool type);
	/*
		用途：将数据库文件中位置为adr的数值修改为reviseData
		参数解释：type为数据类型
		返回值：无
	*/
	
	bool revise(int searchRow, char target[30], char reviseData[30], int reviseRow);
	/*
		用途：将第searchRow个属性为target的记录的第reviseRow个属性改为reviseData
		参数解释：见用途
		返回值：如果一条符合更改条件的记录都没有，返回false；否则，返回true
	*/

	void saveBPlusTree();
};

int char_to_int(char source[30]);
/*
	用途：将字符串转化为数字
	参数解释：要转换的字符串
	返回值：转化后的数字
*/
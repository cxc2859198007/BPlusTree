#include"Operation.h"
#include"BPlusTree.h"
using namespace std;

BPlusTree bpt;
void Operation::buildBinFileName() {
	strcpy_s(binFileName, "");
	int tableNameLen = strlen(tableName);
	strncat_s(binFileName, tableName, tableNameLen);
	strncat_s(binFileName, ".dat", 4);
}

void Operation::getInfo() {
	buildBinFileName();
	fstream f;
	f.open(binFileName, ios::in | ios::binary);
	f.seekg(0, ios::beg);
	f.read((char*)& infoSize, sizeof(int));
	f.read((char*)& recordSize, sizeof(int));
	f.read((char*)& record_num, sizeof(int));
	f.read((char*)& rowNum, sizeof(int));
	
	for (int i = 0; i < rowNum; i++) {
		f.read((char*)& rowName[i], sizeof(rowName[i]));
	}
	for (int i = 0; i < rowNum; i++) {
		f.read((char*)& bool_type[i], sizeof(bool_type[i]));
	}
	f.read((char*)& primaryKeyNum, sizeof(int));
	
	f.close();
}

void Operation::saveInfo() {
	buildBinFileName();
	recordSize += sizeof(bool);
	for (int i = 0; i < rowNum; i++) {
		if (!bool_type[i]) recordSize += sizeof(int);
		else recordSize += sizeof(char) * 30;
	}
	infoSize = 5 * sizeof(int) + rowNum * (sizeof(char[30]) + sizeof(bool));
	fstream f;
	f.open(binFileName, ios::binary | ios::out);
	f.seekp(0, ios::beg);
	f.write((char*)& infoSize, sizeof(int));
	f.write((char*)& recordSize, sizeof(int));
	f.write((char*)& record_num, sizeof(int));
	f.write((char*)& rowNum, sizeof(int));

	for (int i = 0; i < rowNum; i++) {
		f.write((char*)& rowName[i], sizeof(rowName[i]));
	}
	for (int i = 0; i < rowNum; i++) {
		f.write((char*)& bool_type[i], sizeof(bool_type[i]));
	}
	f.write((char*)& primaryKeyNum, sizeof(int));
}

void Operation::cinTableName(char tablename[30]) {
	for (int i = 0; i < 30; i++)
		tableName[i] = tablename[i];
}

void Operation::buildNewTable(char tablename[30], int rownum, char rowname[20][30], char booltype[20], int primarykeynum) {
	cinTableName(tablename);
	is_exist(bpt, tableName);
	
	record_num = 0;
	rowNum = rownum;
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < 30; j++) {
			rowName[i][j] = rowname[i][j];
		}
	}
	for (int i = 0; i < rowNum; i++) {
		bool_type[i] = booltype[i];
	}
	primaryKeyNum = primarykeynum;
	saveInfo();
}

bool Operation::insertByFile(char FileName[30]) {
	getInfo();
	ifstream fin(FileName, ios::in);
	fin.seekg(0, ios::beg);
	fstream f;
	f.open(binFileName, ios::app | ios::binary);
	if (!f) return false;
	char tempStr[20][30]; int tempInt[20];
	while (!fin.eof()) {
		int tempIntIndex = 0, tempStrIndex = 0, temPrimKey = -1;
		for (int i = 0; i < rowNum; i++) {
			if (!bool_type[i]) {
				fin >> tempInt[tempIntIndex];
				if (i + 1 == primaryKeyNum)
					temPrimKey = tempInt[tempIntIndex];
				tempIntIndex++;
			}
			else {
				fin >> tempStr[tempStrIndex];
				tempStrIndex++;
			}
		}
		if (bpt.Search_bytes(temPrimKey) != -1) continue;
		
		tempIntIndex = 0; tempStrIndex = 0;
		bool flag = true;
		long p = long(infoSize) + long(record_num) * recordSize;
		f.write((char*)& flag, sizeof(bool));
		for (int i = 0; i < rowNum; i++) {
			if (!bool_type[i]) {
				f.write((char*)& tempInt[tempIntIndex], sizeof(int));
				tempIntIndex++;
			}
			else {
				f.write((char*)& tempStr[tempStrIndex], sizeof(tempStr[tempStrIndex]));
				tempStrIndex++;
			}
			if (i == rowNum - 1) record_num++;
		}
		bpt.Insert(bpt.root, temPrimKey, p);

	}
	upDate();
	f.close(); fin.close();
	return true;
}

bool Operation::insertByHands(char cinData[40][30]) {
	getInfo();
	fstream f;
	f.open(binFileName, ios::app | ios::binary);
	if (!f) return false;
	int tempInt[20];
	char tempStr[20][30];
	int tempIntIndex = 0, tempStrIndex = 0, temPrimKey = -1;
	for (int i = 0; i < rowNum; i++) {
		if (!bool_type[i]) {
			tempInt[tempIntIndex] = char_to_int(cinData[i]);
			if (i + 1 == primaryKeyNum) {
				temPrimKey = tempInt[tempIntIndex];
			}
			tempIntIndex++;
		}
		else {
			for (int j = 0; j < strlen(cinData[i]); j++)
				tempStr[tempStrIndex][j] = cinData[i][j];
			tempStrIndex++;
		}
	}
	if (bpt.Search_bytes(temPrimKey) != -1) return false;
	tempIntIndex = 0; tempStrIndex = 0;
	bool flag = true;
	long p = long(infoSize) + long(record_num) * recordSize;
	f.write((char*)& flag, sizeof(bool));
	for (int i = 0; i < rowNum; i++) {
		if (!bool_type[i]) {
			f.write((char*)& tempInt[tempIntIndex], sizeof(int));
			tempIntIndex++;
		}
		else {
			f.write((char*)& tempStr[tempStrIndex], sizeof(tempStr[tempStrIndex]));
			tempStrIndex++;
		}
		if (i == rowNum - 1) record_num++;
	}
	bpt.Insert(bpt.root, temPrimKey, p);
	upDate();
	f.close();
	return true;
}

void Operation::upDate() {
	buildBinFileName();
	fstream update;
	update.open(binFileName, ios::binary | ios::out | ios::in);
	update.seekp(8, ios::beg);
	update.write((char*)& record_num, sizeof(int));
}

int char_to_int(char source[30]) {
	int intNum = 0, weight = 0;
	for (int i = strlen(source) - 1; i >= 0; i--) {
		intNum += (int(source[i]) - int('0')) * int(pow(10, weight));
		weight++;
	}
	return intNum;
}
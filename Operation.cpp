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

void Operation::buildNewTable(char tablename[30], int rownum, char rowname[20][30], int booltype[20], int primarykeynum) {
	cinTableName(tablename);
	record_num = 0;
	rowNum = rownum;
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < 30; j++) {
			rowName[i][j] = rowname[i][j];
		}
	}
	for (int i = 0; i < rowNum; i++) {
		if (!booltype[i]) bool_type[i] = false;
		else bool_type[i] = true;
	}
	primaryKeyNum = primarykeynum;
	saveInfo();
	getInfo();
	is_exist(bpt, tableName);
}

void Operation::chooseOldTable(char tablename[30]) {
	cinTableName(tablename);
	getInfo();
	is_exist(bpt, tableName);
}

bool Operation::insertByFile(char FileName[30]) {
	ifstream fin(FileName, ios::in);
	fin.seekg(0, ios::beg);
	fstream f;
	f.open(binFileName, ios::app | ios::binary);
	if (!f) return false;
	char tempStr[20][30]; int tempInt[20];
	while (!fin.eof()) {
		int tempIntIndex = 0, tempStrIndex = 0, temPrimKey = 0;
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
	upDateInfo();
	f.close(); fin.close();
	return true;
}

bool Operation::insertByHands(char cinData[20][30]) {
	fstream f;
	f.open(binFileName, ios::app | ios::binary);
	if (!f) return false;
	int tempInt[20];
	char tempStr[20][30];
	int tempIntIndex = 0, tempStrIndex = 0, temPrimKey = 0;
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
	upDateInfo();
	f.close();
	return true;
}

void Operation::upDateInfo() {
	buildBinFileName();
	fstream update;
	update.open(binFileName, ios::binary | ios::out | ios::in);
	update.seekp(8, ios::beg);
	update.write((char*)& record_num, sizeof(int));
}

void Operation::showRecord(long* adrs, int adr_num) {
	fstream f;
	f.open(binFileName, ios::in | ios::binary);
	for (int k = 0; k < adr_num; k++) {
		cout << "µÚ" << k << "Ìõ:  ";
		f.seekg(adrs[k], ios::beg);
		char tempStr[30]; int tempInt;
		bool flag = 0;
		f.read((char*)& flag, sizeof(bool));
		if (flag) {
			for (int i = 0; i < rowNum; i++) {
				if (!bool_type[i]) {
					f.read((char*)& tempInt, sizeof(int));
					cout << tempInt << " ";
				}
				else {
					f.read((char*)& tempStr, sizeof(tempStr));
					cout << tempStr << " ";
				}
			}
		}
		cout << endl;
	}
	f.close();
}

long Operation::getFirstAdr(int row) {
	long firstAdr = infoSize + sizeof(bool);
	for (int i = 0; i < row - 1; i++) {
		if (!bool_type[i]) firstAdr += sizeof(int);
		else firstAdr += sizeof(char[30]);
	}
	return firstAdr;
}

int Operation::search_adr(int row, char target[30], long*& result_adr) {
	long firstAdr = getFirstAdr(row);
	int result_num = 0;
	fstream f;
	f.open(binFileName, ios::in | ios::binary);
	if (!bool_type[row - 1]) {
		int intTarget = char_to_int(target);
		int tempInt; bool flag = false;
		if (row == primaryKeyNum) {
			long the_result = 0;
			the_result = bpt.Search_bytes(intTarget);
			if (the_result == -1) return 0;
			else{
				result_adr[result_num++] = the_result;
				f.close();
				return result_num;
			}
		}
		for (long i = 0; i < record_num; i++) {
			long p = i * long(recordSize) + firstAdr;
			f.seekg(i * long(recordSize) + infoSize, ios::beg);
			f.read((char*)& flag, sizeof(bool));
			f.seekg(p, ios::beg);
			f.read((char*)& tempInt, sizeof(int));
			if (tempInt == intTarget && flag) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
		}
		f.close();
		return result_num;
	}
	else {
		char temChar[30]; bool flag = false;
		int result_index = 0;
		for (long i = 0; i < record_num; i++) {
			long p = i * long(recordSize) + firstAdr;
			f.seekg(i * long(recordSize) + infoSize, ios::beg);
			f.read((char*)& flag, sizeof(bool));
			f.seekg(p, ios::beg);
			f.read((char*)& temChar, sizeof(temChar));
			if (!strcmp(target, temChar) && flag) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
		}
		f.close();
		return result_num;
	}
}

bool Operation::search(int row, char target[30]) {
	long* result_adr = new long[50000];
	int result_num = search_adr(row, target, result_adr);
	if (result_num == 0) {
		delete[]result_adr;
		return false;
	}
	else {
		showRecord(result_adr, result_num);
		delete[]result_adr;
		return true;
	}
}

void Operation::deleteInFile(long* adrs, int adr_num) {
	fstream update;
	update.open(binFileName, ios::binary | ios::out | ios::in);
	for (int i = 0; i < adr_num; i++) {
		update.seekp(adrs[i], ios::beg);
		bool flag = false;
		update.write((char*)& flag, sizeof(flag));
	}
	update.close();
}
bool Operation::deletee(int row, char target[30]) {
	long* result_adr = new long[50000];
	int result_num = search_adr(row, target, result_adr);
	if (result_num == 0) {
		delete[]result_adr;
		return false;
	}
	showRecord(result_adr, result_num);
	deleteInFile(result_adr, result_num);

	if (row == primaryKeyNum) {
		int intTarget = char_to_int(target);
		bpt.Delete(bpt.root, intTarget);
	}
	else {
		fstream f;
		f.open(binFileName, ios::in | ios::binary);
		for (int k = 0; k < result_num; k++) {
			f.seekg(result_adr[k], ios::beg);
			char tempStr[30]; int tempInt;
			bool flag = 0;
			f.read((char*)& flag, sizeof(bool));
			for (int i = 0; i < rowNum; i++) {
				if (!bool_type[i]) {
					f.read((char*)& tempInt, sizeof(int));
					if (i + 1 == primaryKeyNum) {
						bpt.Delete(bpt.root, tempInt);
						break;
					}
				}
				else f.read((char*)& tempStr, sizeof(tempStr));
			}
		}
	}
	delete[]result_adr;
	return true;
}

void Operation::upDateRecord(long adr, char reviseData[30], bool type) {
	char tempStr[30];
	for (int i = 0; i < 30; i++) tempStr[i] = '\0';
	for (int i = 0; i < strlen(reviseData); i++) tempStr[i] = reviseData[i];
	fstream update;
	update.open(binFileName, ios::binary | ios::out | ios::in);
	update.seekp(adr, ios::beg);
	if (!type) {
		int intData = char_to_int(tempStr);
		update.write((char*)& intData, sizeof(int));
	}
	else update.write((char*)& tempStr, sizeof(char) * 30);
	update.close();
}

bool Operation::revise(int searchRow, char target[30], char reviseData[30], int reviseRow) {
	long* result_adr = new long[50000];
	int result_num = search_adr(searchRow, target, result_adr);
	showRecord(result_adr, result_num);
	if (result_num == 0) {
		delete[]result_adr;
		return false;
	}
	long miss = getFirstAdr(reviseRow) - infoSize;
	for (int i = 0; i < result_num; i++) {
		result_adr[i] += miss;
		upDateRecord(result_adr[i], reviseData, bool_type[reviseRow - 1]);
	}
	delete[]result_adr;
	return true;
}

void Operation::saveBPlusTree() {
	bpt.Save(tableName);
	bpt.Print(tableName);
}

int char_to_int(char source[30]) {
	int intNum = 0, weight = 0;
	for (int i = strlen(source) - 1; i >= 0; i--) {
		intNum += (int(source[i]) - int('0')) * int(pow(10, weight));
		weight++;
	}
	return intNum;
}
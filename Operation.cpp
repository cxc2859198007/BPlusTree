#include"Operation.h"
#include"BPlusTree.h"

#include<cstring>
#include<string>
#include<string.h>
using namespace std;

BPlusTree bpt;
int wei[260];

void Operation::buildBinFileName() {
	memset(binFileName, '\0', sizeof(binFileName));
	int tableNameLen = strlen(tableName);
	Strncat(binFileName, tableName, tableNameLen);
	Strncat(binFileName, ".dat", 4);
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

bool Operation::insertByFile(char FileName[50]) {
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

	shownum = adr_num;
	for (int k = 0; k < adr_num && k < 2000; k++) {
		memset(show[k], '\0', sizeof(show[k]));
		cout << "µÚ" << k + 1 << "Ìõ¼ÇÂ¼£º";
		f.seekg(adrs[k], ios::beg);
		bool flag = 0;
		f.read((char*)& flag, sizeof(bool));
		if (flag) {
			for (int i = 0; i < rowNum; i++) {
				if (!bool_type[i]) {
					int tempInt, cnt1 = 0, cnt2 = 0;
					f.read((char*)& tempInt, sizeof(int));
					while (tempInt) {
						show[k][i][cnt1++] = tempInt % 10 + '0';
						tempInt /= 10;
					}
					cnt2 = cnt1 - 1; cnt1 = 0;
					while (cnt1 < cnt2) {
						char c = show[k][i][cnt1];
						show[k][i][cnt1] = show[k][i][cnt2];
						show[k][i][cnt2] = c;
						cnt1++; cnt2--;
					}
					cout << show[k][i] << " ";
				}
				else {
					f.read((char*)& show[k][i], sizeof(char) * 30);
					cout << show[k][i] << " ";
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

int Operation::search_adr(int row, char target[30], long*& result_adr, int rel) {
	long firstAdr = getFirstAdr(row);
	int result_num = 0;
	fstream f;
	f.open(binFileName, ios::in | ios::binary);
	if (row == 0) {
		int start = char_to_int(target);
		bool flag = false; int now = 0;
		for (long i = 0; i < record_num; i++) {
			long p = i * long(recordSize) + firstAdr;
			f.seekg(i * long(recordSize) + infoSize, ios::beg);
			f.read((char*)& flag, sizeof(bool));
			if (flag) {
				now++;
				if (start <= now) {
					result_adr[result_num++] = (i * long(recordSize) + infoSize);
					if (result_num == 1000) {
						f.close();
						return 1000;
					}
				}
			}
		}
		f.close();
		return result_num;
	}
	if (!bool_type[row - 1]) {
		int intTarget = char_to_int(target);
		int tempInt; bool flag = false;
		if (row == primaryKeyNum) {
			if (rel == 1) {
				long the_result = 0;
				the_result = bpt.Search_bytes(intTarget);
				if (the_result == -1) return 0;
				else {
					if (result_num < 2000) result_adr[result_num++] = the_result;
					f.close();
					return 1;
				}
			}

			if (rel == 2) bpt.Search_rangebytes(-2147483647, intTarget - 1, result_num, result_adr);
			else if (rel == 3) bpt.Search_rangebytes(-2147483647, intTarget, result_num, result_adr);
			else if (rel == 4) bpt.Search_rangebytes(intTarget + 1, 2147483647, result_num, result_adr);
			else if (rel == 5) bpt.Search_rangebytes(intTarget, 2147483647, result_num, result_adr);
			f.close();
			return result_num;
		}
		for (long i = 0; i < record_num; i++) {
			long p = i * long(recordSize) + firstAdr;
			f.seekg(i * long(recordSize) + infoSize, ios::beg);
			f.read((char*)& flag, sizeof(bool));
			f.seekg(p, ios::beg);
			f.read((char*)& tempInt, sizeof(int));
			if (tempInt == intTarget && flag && rel == 1 && result_num < 2000) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
			else if (tempInt < intTarget && flag && rel == 2 && result_num < 2000) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
			else if (tempInt <= intTarget && flag && rel == 3 && result_num < 2000) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
			else if (tempInt > intTarget && flag && rel == 4 && result_num < 2000) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
			else if (tempInt >= intTarget && flag && rel == 5 && result_num < 2000) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
		}
		f.close();
		return result_num;
	}
	else {
		char temChar[30]; bool flag = false;
		int result_index = 0;
		for (int i = 0; i < 260 && rel == 6; i++) wei[i] = -1;
		for (long i = 0; i < record_num; i++) {
			long p = i * long(recordSize) + firstAdr;
			f.seekg(i * long(recordSize) + infoSize, ios::beg);
			f.read((char*)& flag, sizeof(bool));
			f.seekg(p, ios::beg);
			f.read((char*)& temChar, sizeof(temChar));
			if (!strcmp(target, temChar) && flag && rel == 1 && result_num < 2000) {
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
			else if (partial_search(temChar, strlen(temChar), target, strlen(target)) && flag && rel == 6 && result_num < 2000){
				result_adr[result_num++] = (i * long(recordSize) + infoSize);
			}
		}
		f.close();
		return result_num;
	}
}

bool Operation::search(int row, char target[30], int rel) {
	long* result_adr = new long[2000];
	int result_num = search_adr(row, target, result_adr, rel);
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

void Operation::deleteInBPlusTree(long* adrs, int adr_num) {
	fstream f;
	f.open(binFileName, ios::in | ios::binary);
	for (int k = 0; k < adr_num; k++) {
		f.seekg(adrs[k], ios::beg);
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
	f.close();
}
bool Operation::deletee(int row, char target[30],int rel) {
	long* result_adr = new long[2000];
	int result_num = search_adr(row, target, result_adr, rel);

	if (result_num == 0) {
		delete[]result_adr;
		return false;
	}
	showRecord(result_adr, result_num);
	deleteInFile(result_adr, result_num);
	deleteInBPlusTree(result_adr, result_num);

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

bool Operation::revise(int searchRow, char target[30], char reviseData[30], int reviseRow, int rel) {
	long* result_adr = new long[2000];
	int result_num = search_adr(searchRow, target, result_adr, rel);
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

bool partial_search(char* source, int destLen, char* target, int targetLen) {
	int pos = 0;
	for (int i = 0; i < targetLen; i++) wei[int(target[i])] = i;
	while (pos + targetLen <= destLen) {
		bool flag = true;
		for (int i = 0; i < targetLen; i++) {
			if (source[i + pos] != target[i]) {
				flag = false;
				break;
			}
		}
		if (flag == true) {
			for (int j = 0; j < targetLen; j++) {
				wei[int(target[j])] = -1;
			}
			return true;
		}
		else {
			int space = targetLen;
			if (wei[source[space + pos]] == -1)
				pos = pos + space + 1;
			else
				pos = pos + space - wei[source[pos + space]];
		}
	}
	for (int j = 0; j < targetLen; j++) wei[int(target[j])] = -1;
	return false;
}
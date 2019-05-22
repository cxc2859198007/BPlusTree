#include"BPlusTree.h"
#include<fstream>
#include<istream>
#include<ostream>
using namespace std;
BPlusTree bpt;

class Operation {
public:
	char tableName[30];
	char FileName[30];
	char binFileName[50];
	int rowNum;
	int primaryKeyNum;
	char rowName[20][30];
	char type[20][30];
	bool bool_type[20];
	int record_num;
	long long recordSize;

	Operation() {
		memset(tableName, 0, sizeof(tableName));

		recordSize = 0;
	}
	bool buildTableByFile();
	bool buildTableByHands();
	void TraversalBin();
	void buildBinFileName();
};
Operation opt;

int main() {

	opt.buildTableByFile();
	bpt.Save(opt.tableName);
	//opt.TraversalBin();

	return 0;
}

void Operation::buildBinFileName() {
	strcpy_s(binFileName, "D:\\");
	int tableNameLen = strlen(tableName);
	strncat_s(binFileName, tableName, tableNameLen);
	strncat_s(binFileName, ".dat", 4);
}
bool Operation::buildTableByFile() {

	cin >> FileName;
	ifstream fin(FileName, ios::in); fin.seekg(0, ios::beg);

	fin >> tableName;
	is_exist(bpt, tableName);
	buildBinFileName();
	fin >> rowNum;
	for (int i = 0; i < rowNum; i++)
	{
		fin >> rowName[i];
	}
	for (int i = 0; i < rowNum; i++)
	{
		fin >> type[i];
		if (strcmp(type[i], "int") == 0)
			bool_type[i] = 0;
		else
			bool_type[i] = 1;
	}
	fin >> primaryKeyNum;
	fin >> record_num;

	fstream f;
	f.open(binFileName, ios::out | ios::binary);
	char tempStr[25]; int tempInt;
	for (int i = 0; i < rowNum; i++)
	{
		if (!bool_type[i])
			recordSize += long long(sizeof(int));
		else
			recordSize += long long(sizeof(tempStr));
	}
	for (long long j = 0; j < record_num; j++)
	{
		for (int i = 0; i < rowNum; i++)
		{
			if (!bool_type[i])
			{
				fin >> tempInt;
				f.write((char*)& tempInt, sizeof(int));
				if (i + 1 == primaryKeyNum)
					bpt.Insert(bpt.root, tempInt, j * recordSize);

			}
			else
			{
				fin >> tempStr;
				for (int i = strlen(tempStr); i < 25; i++)
					tempStr[i] = '\0';
				f.write((char*)& tempStr, sizeof(tempStr));

			}
		}
	}
	f.close();
	fin.close();
	return true;
}

bool Operation::buildTableByHands() {

	cout << "输入表名：" << endl;
	cin >> tableName;
	is_exist(bpt, tableName);
	buildBinFileName();
	cout << "输入属性数:" << endl;
	cin >> rowNum;
	cout << "输入属性名:" << endl;
	for (int i = 0; i < rowNum; i++)
	{
		cin >> rowName[i];
	}
	cout << "输入各属性类型（0代表int类型或1代表char数组类型）:" << endl;
	for (int i = 0; i < rowNum; i++)
	{
		cin >> bool_type[i];
	}
	cout << "输入主码为第几个属性:" << endl;
	cin >> primaryKeyNum;
	cout << "输入记录条数:" << endl;
	cin >> record_num;

	fstream f;
	f.open(binFileName, ios::out | ios::binary);
	if (!f)
	{
		cout << "文件打开失败,程序终止!" << endl;
		exit(0);
	}
	char tempStr[25]; int tempInt;
	cout << "分别输入各个记录:" << endl;
	for (int i = 0; i < rowNum; i++)
	{
		if (!bool_type[i])
			recordSize += long long(sizeof(int));
		else
			recordSize += long long(sizeof(tempStr));
	}
	for (long long j = 0; j < record_num; j++)
	{
		for (int i = 0; i < rowNum; i++)
		{
			if (!bool_type[i])
			{
				cin >> tempInt;
				f.write((char*)& tempInt, sizeof(int));
				if (i + 1 == primaryKeyNum)
					bpt.Insert(bpt.root, tempInt, j * recordSize);

			}
			else
			{
				cin >> tempStr;
				for (int i = strlen(tempStr); i < 25; i++)
					tempStr[i] = '\0';
				f.write((char*)& tempStr, sizeof(tempStr));
			}
		}
	}
	f.close();
	return true;
}
void Operation::TraversalBin()
{
	fstream f;
	f.open(binFileName, ios::in | ios::binary);
	char tempStr[25]; int tempInt;
	int count = 0;
	while (record_num--)
	{
		for (int i = 0; i < rowNum; i++)
		{
			if (!bool_type[i])
			{
				f.read((char*)& tempInt, sizeof(int));
				cout << tempInt << " ";
			}
			else
			{
				f.read((char*)& tempStr, sizeof(tempStr));
				cout << tempStr << " ";
			}
		}
		cout << endl;
		//cout <<++count<<endl;
	}
	f.close();
}
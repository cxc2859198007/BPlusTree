#include"BPlusTree.h"
#include"Operation.h"
#include<fstream>
#include<istream>
#include<ostream>
#include<ctime>
using namespace std;
Operation opt;
ofstream dataout;

/*
≤‚ ‘ ˝æ›£∫
Student
10
id name sex age class grade school chinese math english
0 1 1 0 1 0 1 0 0 0
1
*/

bool vis[100000000];
int RandId() {
	int id = 0;
	for (int i = 0; i < 8; i++) {
		if (i == 7) id += (rand() % 9 + 1) * int(pow(10, i));
		else id += (rand() % 10) * int(pow(10, i));
	}
	if (vis[id] == true) return RandId();
	else {
		vis[id] = true;
		return id;
	}
}
string RandName() {
	string nm = "";
	int len = rand() % 11 + 5;
	for (int i = 1; i <= len; i++) {
		nm += char(rand() % 26 + 'a');
	}
	return nm;
}
string RandSex() {
	string s1 = "male", s2 = "female";
	int a = rand() % 2;
	if (a == 0) return s1;
	else return s2;
}
string RandClass() {
	string cla = "";
	for (int i = 1; i <= 10; i++) {
		cla += char(rand() % 26 + 'a');
	}
	return cla;
}
string RandSchool() {
	string sch = "";
	int len = rand() % 16 + 5;
	for (int i = 1; i <= len; i++) {
		sch += char(rand() % 26 + 'a');
	}
	return sch;
}
void Rand() {
	int id= RandId(); dataout << id << " ";
	//cout << id << " ";
	
	string nm = RandName(); dataout << nm << " ";
	//cout << nm << " ";
	
	string sex = RandSex(); dataout << sex << " ";
	//cout << sex << " ";
	
	int age = rand() % 9 + 12; dataout << age << " ";
	//cout << age << " ";

	string cla = RandClass(); dataout << cla << " ";
	//cout << cla << " ";
	
	int gra = rand() % 21; dataout << gra << " ";
	//cout << gra << " ";
	
	string sch = RandSchool(); dataout << sch << " ";
	//cout << sch << " ";

	int chi = rand() % 101; dataout << chi << " ";
	//cout << chi << " ";

	int mat = rand() % 101; dataout << mat << " ";
	//cout << mat << " ";

	int eng = rand() % 101; dataout << eng << endl;
	//cout << eng << endl;
}
void RandData() {
	dataout.open("test_data_100w.txt", ios::out);
	for (int i = 1; i <= 1000000; i++) Rand();
	dataout.close();
}

int main() {
	
	char tbnm[30] = "Student";
	int rm = 10;
	char rnm[20][30] = { "id","name","sex","age","class","grade","school","chinese","math","english" };
	int tp[20] = { 0, 1, 1, 0, 1, 0, 1, 0, 0, 0 };
	int pkn = 1;
	char df[30] = "test_data_1w.txt";
	char tar1[30] = "female";
	char tar2[30] = "male";
	//opt.buildNewTable(tbnm, rm, rnm, tp, pkn);
	opt.chooseOldTable(tbnm);
	//opt.deletee(3, tar2);
	//opt.search(1, tar1);
	opt.search(3, tar1);
	//opt.insertByFile(df);
	//opt.search(3, tar);
	//opt.revise(3, tar2, tar1, 3);
	opt.saveBPlusTree();
	system("pause");
	return 0;
}
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<fstream>
#include"BPlusTree.h"
using namespace std;
const string filename = "d:\\data.txt";
ifstream infile;
ofstream outfile;
int test[10000];
bool vis[20000];
void Rand() {
	memset(vis, false, sizeof(vis));
	int cnt = 1;
	while (cnt <= 5000) {
		int x = rand() % 20000;
		if (vis[x] == false) {
			vis[x] = true;
			test[cnt] = x;
			cnt++;
		}
	}
	
	outfile.open(filename, ios::out);
	for (int i = 1; i <= 5000; i++) {
		outfile << test[i] << endl;
	}
	outfile.close();
}
void Read() {
	infile.open(filename, ios::in);
	int cnt = 1;
	while (!infile.eof()) {
		infile >> test[cnt++];
	}
	infile.close();
}
BPlusTree bpt;

int main() {
	srand(unsigned int(time(NULL)));
	//Rand();
	Read();
	
	bpt.Create();
	for (int i = 1; i <= 5000; i++) {
		bpt.Insert(bpt.root, test[i]);	
	}
	bpt.Print();
	return 0;
}
#pragma once
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<string.h>
#include<cstdlib>
#include<queue>
#include<cmath>
#include <string>
const int M = 8;//阶数
class Node {
public:
	int key[M + 5];
	long long bytes[M + 5];
	int num;
	bool is_leaf;
	Node* fath;
	Node* ch[M + 5];
	Node* prev;
	Node* next;
};

class BPlusTree {
public:
	Node* root;
	int Node_num;

	//void Print();
	void Save(char tableName[30]);

	Node* Node_new();//新建B+树结点
	void Create();

	Node* Search_leaf(Node* now, int target);
	long long Search_bytes(Node* root, int target);
	void Split(Node* p1);
	bool Insert(Node*& root, int target, long long byt);

	bool Left_transfer(Node* p);
	bool Right_transfer(Node* p);
	bool Merge(Node* p1, Node* p2);
	bool Delete(Node*& root, int target);

	BPlusTree() {
		root = NULL;
		Node_num = 0;
	}
	~BPlusTree() {
		delete root;
		Node_num = 0;
	}
};

void is_exist(BPlusTree& tmpbpt, char tableName[30]);
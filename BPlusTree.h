#pragma once
#include<iostream>
#include<cstdio>
const int M = 20;//½×Êý

class Node {
public:
	int key[M + 5];
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

	void Print();

	Node* Node_new();
	void Create();
	
	Node* Search_leaf(Node* now,int target);

	void Split(Node* ch1);
	bool Insert(Node*& root, int target);
	
	bool Left_transfer(Node* p);
	bool Right_transfer(Node* p);
	void Merge(Node* ch1, Node* ch2);
	bool Delete(Node* root, int target);

	BPlusTree() {
		root = NULL;
		Node_num = 0;
	}
	~BPlusTree() {
		delete root;
		Node_num = 0;
	}
};
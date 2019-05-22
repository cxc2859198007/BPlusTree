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
	long long bytes[M + 5];//bytes[i]为key[i]对应记录所在二进制文件中的位置
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
	/*
		用途：将此B+树存储下来,存储到tableName_bplustree.txt
		参数解释：tableName[30]为B+树对应的表名
		返回值：无
	*/

	Node* Node_new();
	/*
		用途：新建B+树结点
		参数解释：无
		返回值：一个空结点
	*/
	
	void Create();
	/*
		用途：创建一个空的B+树 只有一个空的根
		参数解释：无
		返回值：无
	*/
	
	Node* Search_leaf(Node* now, int target);
	/*
		用途：找到应该插入/删除/查询的叶子结点
		参数解释：now为当前结点 target为要插入/删除/查询的key
		返回值：要操作的叶子结点
	*/
	
	long long Search_bytes(int target);
	/*
		用途：查询某个key值对应的bytes值
		参数解释：target为key值
		返回值：返回key值对应的bytes值，如果不存在返回-1
	*/
	
	void Split(Node* p1);
	/*
		用途：B+树的p1结点超过M-1个key，需要分裂
		参数解释：p1为超过限制的结点
		返回值：无
	*/

	bool Insert(Node*& root, int target, long long byt);
	/*
		用途：B+树插入操作
		参数解释：root为B+树的根 target为要插入的key byt为要插入的bytes
		返回值：返回插入是否成功
	*/

	bool Left_transfer(Node* p);
	/*
		用途：向左兄弟借值
		参数解释：p的key值过少 需要借操作
		返回值：返回是否成功
	*/

	bool Right_transfer(Node* p);
	/*
		用途：向右兄弟借值
		参数解释：p的key值过少 需要借操作
		返回值：返回是否成功
	*/

	bool Merge(Node* p1, Node* p2);
	/*
		用途：合并操作
		参数解释：p1和p2合并
		返回值：返回是否成功
	*/

	bool Delete(Node*& root, int target);
	/*
		用途：删除操作
		参数解释：root为B+树的树根 target为要删除的key值
		返回值：返回是否成功
	*/

	BPlusTree() {
		root = NULL;
		Node_num = 0;
	}
	~BPlusTree() {
		delete root;
		Node_num = 0;
	}
};

bool is_exist(BPlusTree& tmpbpt, char tableName[30]);
/*
	用途：建立一棵B+树，如果建过此表直接读取以前建立的树，如果没有生成一棵空树
	参数解释：tmpbpt为建立的B+树 tableName为此树对应的表名
	返回值：返回以前是否建过tableName
*/
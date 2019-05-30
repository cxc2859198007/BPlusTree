#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<string.h>
#include<cstdlib>
#include<queue>
#include<cmath>
#include <string>
using namespace std;
const int M = 20;//����
class Node {
public:
	int key[M + 5];
	long bytes[M + 5];//bytes[i]Ϊkey[i]��Ӧ��¼���ڶ������ļ��е�λ��
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


	void Print(char tableName[30]);
	/*
		��;������B+���ṹ�洢����,�洢��tableName_bplustree.txt
		�������ͣ�tableName[30]ΪB+����Ӧ�ı���
		����ֵ����
	*/

	void Save(char tableName[30]);
	/*
		��;������B+��key��bytes�洢����,�洢��tableName_bplustree_data.txt
		�������ͣ�tableName[30]ΪB+����Ӧ�ı���
		����ֵ����
	*/

	Node* Node_new();
	/*
		��;���½�B+�����
		�������ͣ���
		����ֵ��һ���ս��
	*/

	void Create();
	/*
		��;������һ���յ�B+�� ֻ��һ���յĸ�
		�������ͣ���
		����ֵ����
	*/

	Node* Search_leaf(Node* now, int target);
	/*
		��;���ҵ�Ӧ�ò���/ɾ��/��ѯ��Ҷ�ӽ��
		�������ͣ�nowΪ��ǰ��� targetΪҪ����/ɾ��/��ѯ��key
		����ֵ��Ҫ������Ҷ�ӽ��
	*/

	long Search_bytes(int target);
	/*
		��;����ѯĳ��keyֵ��Ӧ��bytesֵ
		�������ͣ�targetΪkeyֵ
		����ֵ������keyֵ��Ӧ��bytesֵ����������ڷ���-1
	*/

	void Search_rangebytes(int l, int r, int& result_num, long*& result_adr);
	/*
		��;����ѯkeyֵ��[l,r]��Χ�ڵ����ж�Ӧbytesֵ
		�������ͣ�l,r��Χ����result_num����¼������results_adr������
		����ֵ����
	*/
	
	void Split(Node* p1);
	/*
		��;��B+����p1��㳬��M-1��key����Ҫ����
		�������ͣ�p1Ϊ�������ƵĽ��
		����ֵ����
	*/

	bool Insert(Node*& root, int target, long byt);
	/*
		��;��B+���������
		�������ͣ�rootΪB+���ĸ� targetΪҪ�����key bytΪҪ�����bytes
		����ֵ�����ز����Ƿ�ɹ�
	*/

	bool Left_transfer(Node* p);
	/*
		��;�������ֵܽ�ֵ
		�������ͣ�p��keyֵ���� ��Ҫ�����
		����ֵ�������Ƿ�ɹ�
	*/

	bool Right_transfer(Node* p);
	/*
		��;�������ֵܽ�ֵ
		�������ͣ�p��keyֵ���� ��Ҫ�����
		����ֵ�������Ƿ�ɹ�
	*/

	bool Merge(Node* p1, Node* p2);
	/*
		��;���ϲ�����
		�������ͣ�p1��p2�ϲ�
		����ֵ�������Ƿ�ɹ�
	*/

	bool Delete(Node*& root, int target);
	/*
		��;��ɾ������
		�������ͣ�rootΪB+�������� targetΪҪɾ����keyֵ
		����ֵ�������Ƿ�ɹ�
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
	��;������һ��B+������������˱�ֱ�Ӷ�ȡ��ǰ�������������û������һ�ÿ���
	�������ͣ�tmpbptΪ������B+�� tableNameΪ������Ӧ�ı���
	����ֵ��������ǰ�Ƿ񽨹�tableName
*/

void Strncat(char* s1, char* s2, int len);
void Strncat(char* s1, const char* s2, int len);
#endif
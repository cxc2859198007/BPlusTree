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
const int M = 8;//����
class Node {
public:
	int key[M + 5];
	long long bytes[M + 5];//bytes[i]Ϊkey[i]��Ӧ��¼���ڶ������ļ��е�λ��
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
		��;������B+���洢����,�洢��tableName_bplustree.txt
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
	
	long long Search_bytes(int target);
	/*
		��;����ѯĳ��keyֵ��Ӧ��bytesֵ
		�������ͣ�targetΪkeyֵ
		����ֵ������keyֵ��Ӧ��bytesֵ����������ڷ���-1
	*/
	
	void Split(Node* p1);
	/*
		��;��B+����p1��㳬��M-1��key����Ҫ����
		�������ͣ�p1Ϊ�������ƵĽ��
		����ֵ����
	*/

	bool Insert(Node*& root, int target, long long byt);
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
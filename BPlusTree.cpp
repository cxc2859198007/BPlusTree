#include"BPlusTree.h"
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<queue>
#include<fstream>
#include<cstring>
#include<cmath>
using namespace std;
string bptfile = "d://bptfile.txt";
ofstream out;
void BPlusTree::Print() {
	out.open(bptfile, ios::out | ios::app);

	queue<Node*> Q; queue<int> L;
	Q.push(root); L.push(1);
	while (!Q.empty()) {
		Node* tmpQ = Q.front(); int tmpL = L.front();
		Q.pop(); L.pop();
		for (int i = 1; i <= tmpQ->num-1; i++)
			out << tmpQ->key[i] << "--";
		out << tmpQ->key[tmpQ->num] << "    ";
		if (tmpQ->is_leaf == true) continue;
		for (int i = 1; i <= tmpQ->num + 1; i++) {
			Q.push(tmpQ->ch[i]);
			L.push(tmpL + 1);
		}
		if (!L.empty() && L.front() == tmpL + 1) out << endl;
	}
	out << endl << endl << endl;

	out.close();
	return;
}

Node* BPlusTree::Node_new() {//�½�һ���ս��
	Node* node = new Node;
	node->is_leaf = true;
	for (int i = 1; i <= M - 1; i++) node->key[i] = 0;
	for (int i = 1; i <= M; i++) node->ch[i] = NULL;
	node->fath = NULL;
	node->prev = NULL; node->next = NULL; 
	node->num = 0;
	return node;
}

void BPlusTree::Create() {//����һ��B+�� ֻ��һ���յĸ�
	Node* node = Node_new();
	root = node;
}

Node* BPlusTree::Search_leaf(Node* now, int target) {//�ҵ�targetӦ�ò���/ɾ��/��ѯ��Ҷ�ӽ��
	if (now->is_leaf == true) return now;
	bool flag = false;
	for (int i = 1; i <= now->num; i++) {
		if (target < now->key[i]) {
			flag = true;
			return Search_leaf(now->ch[i], target);
			break;
		}
	}
	if (flag == false) 
		return Search_leaf(now->ch[now->num + 1], target);
}

void BPlusTree::Split(Node* ch1) {//ch1��M��key������M-1�����ƣ�����
	Node* fa = ch1->fath;
	
	Node* ch2 = Node_new();
	ch2->is_leaf = ch1->is_leaf;
	ch2->fath = ch1->fath;
	
	if (M % 2 == 0) {
		ch1->num = M / 2;
		ch2->num = M / 2 - 1; 
	}
	else {
		ch1->num = M / 2;
		ch2->num = M / 2;
	}
	
	for (int i = 1; i <= ch2->num; i++) ch2->key[i] = ch1->key[ch1->num + i + 1];
	
	if (ch2->is_leaf == true) {
		for (int i = ch2->num + 1; i >= 2; i--) ch2->key[i] = ch2->key[i - 1];
		ch2->key[1] = ch1->key[ch1->num + 1];
		ch2->num++;
	}
	
	if (ch2->is_leaf == false) {
		for (int i = 1; i <= ch2->num + 1; i++) {
			ch2->ch[i] = ch1->ch[ch1->num + i + 1];
			ch1->ch[ch1->num + i + 1]->fath = ch2;
		}
	}

	bool flag = false;
	for (int i = 1; i <= fa->num; i++) {
		if (ch1->key[ch1->num + 1] <= fa->key[i]) {
			flag = true;
			for (int j = fa->num + 1; j >= i + 1; j--) fa->key[j] = fa->key[j - 1];
			for (int j = fa->num + 2; j >= i + 1; j--) fa->ch[j] = fa->ch[j - 1];
			fa->key[i] = ch1->key[ch1->num + 1];
			fa->ch[i + 1] = ch2;
			break;
		}
	}
	if (flag == false) {
		fa->key[fa->num + 1] = ch1->key[ch1->num + 1];
		fa->ch[fa->num + 2] = ch2;
	}
	fa->num++;

	ch2->next = ch1->next;
	if (ch1->next != NULL) ch1->next->prev = ch2;
	ch2->prev = ch1;
	ch1->next = ch2;

	return;
}

bool BPlusTree::Insert(Node*& root, int target) {
	Node* pos = Search_leaf(root, target);

	int k = 0;
	for (int i = 1; i <= pos->num; i++) {
		if (target == pos->key[i]) {
			k = i;
			break;
		}
	}
	if (k != 0) return false;//��¼�Ѵ��ڣ��������ظ�����
	
	//����
	bool flag = false;
	for (int i = 1; i <= pos->num; i++) {
		if (target <= pos->key[i]) {
			flag = true;
			for (int j = pos->num + 1; j >= i + 1; j--) pos->key[j] = pos->key[j - 1];
			pos->key[i] = target;
			break;
		}
	}
	if (flag == false) {
		pos->key[pos->num + 1] = target;
	}
	pos->num++;
	
	//����
	if (pos->num == M) {
		if (pos == root) {//�����
			Node* node = Node_new();
			node->is_leaf = false;
			node->ch[1] = pos;
			pos->fath = node;//root->fath=node
			root = node;
			Split(pos);
		}
		else {//Ҷ�ӽ��
			while (pos->num == M && pos != root) {
				Split(pos);
				pos = pos->fath;
			}
			if (pos->num == M && pos == root) {
				Node* node = Node_new();
				node->is_leaf = false;
				node->ch[1] = pos;
				pos->fath = node;//root->fath=node
				root = node;
				Split(pos);
			}
		}
	}

	return true;
}


bool BPlusTree::Left_transfer(Node* p) {
	if (p->prev != NULL && p->fath == p->prev->fath && p->prev->num > ceil(double(M) / 2) - 1) {
		Node* leftsli = p->prev;
		if (p->is_leaf == true) {//Ҷ���
			for (int i = p->num + 1; i >= 2; i--) p->key[i] = p->key[i - 1];
			p->key[1] = leftsli->key[leftsli->num];
			p->num++;
			leftsli->num--;
			for (int i = 1; i <= p->fath->num; i++) {
				if (p->fath->ch[i] == leftsli && p->fath->ch[i + 1] == p) {
					p->fath->key[i] = p->key[1];
					break;
				}
			}
		}
		else {//�м�ڵ�
			for (int i = p->num + 1; i >= 2; i--) p->key[i] = p->key[i - 1];
			for (int i = p->num + 2; i >= 2; i--) p->ch[i] = p->ch[i - 1];
				
			for (int i = 1; i <= p->fath->num; i++) {
				if (p->fath->ch[i] == leftsli && p->fath->ch[i + 1] == p) {
					p->key[1] = p->fath->key[i];
					p->fath->key[i] = leftsli->key[leftsli->num];
					p->ch[1] = leftsli->ch[leftsli->num + 1];
					p->num++;
					leftsli->num--;
					break;
				}
			}
		}
		return true;
	}
	return false;
}

bool BPlusTree::Right_transfer(Node* p) {
	if (p->next != NULL && p->fath == p->next->fath && p->next->num > ceil(double(M) / 2) - 1) {
		Node* rightsli = p->next;
		if (p->is_leaf == true) {//Ҷ���
			p->key[p->num + 1] = rightsli->key[1];
			p->num++;
			for (int i = 1; i <= rightsli->num - 1; i++) rightsli->key[i] = rightsli->key[i + 1];
			rightsli->num--;
			for (int i = 1; i < p->fath->num; i++) {
				if (p->fath->ch[i] == p && p->fath->ch[i + 1] == rightsli) {
					p->fath->key[i] = rightsli->key[1];
					break;
				}
			}
		}
		else {//�м���
			for (int i = 1; i < p->fath->num; i++) {
				if (p->fath->ch[i] == p && p->fath->ch[i + 1] == rightsli) {
					p->key[p->num + 1] = p->fath->key[i];
					p->num++;
					p->fath->key[i] = rightsli->key[1];
					rightsli->num--;
					p->ch[p->num + 1] = rightsli->ch[1];
					for (int j = 1; j <= rightsli->num; j++) rightsli->key[j] = rightsli->key[j + 1];
					for (int j = 1; j <= rightsli->num + 1; j++) rightsli->ch[j] = rightsli->ch[j + 1];
					break;
				}
			}
		}

		return true;
	}
	
	return false;
}

bool BPlusTree::Delete(Node* root, int target) {
	Node* pos = Search_leaf(root, target);
	
	int k = 0;
	for (int i = 1; i <= pos->num; i++) {
		if (target == pos->key[i]) {			
			k = i;
			break;
		}
	}
	if (k == 0) return false;//��¼������
	
	//ɾ��
	for (int i = k; i <= pos->num - 1; i++) {
		pos->key[i] = pos->key[i + 1];
	}
	pos->num--;

	if (pos != root) {//���Ǹ���㣬��Ҫ����key�������Ƿ����㶨��
		//����ɾ��֮��Ը�����Ӱ��
		if (k == 1 && pos != pos->fath->ch[1]) {
			for (int i = 2; i <= pos->fath->num + 1; i++) {
				if (pos == pos->fath->ch[i]) {
					pos->fath->key[i - 1] = pos->key[1];
					break;
				}
			}
		}

		//key����������
		while (pos->num < ceil(double(M) / 2) - 1) {
			if (Left_transfer(pos) == false);
			else if (Right_transfer(pos) == false);
			else if (Merge(pos->prev, pos) == false);
			else if (Merge(pos, pos->next) == false);
			else 
		}
	}

	return true;
}
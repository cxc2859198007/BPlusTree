#include"BPlusTree.h"
#include<fstream>
#include<istream>
#include<ostream>
#include<cstring>
#include<string>
#include<string.h>
using namespace std;

void BPlusTree::Print(char tableName[30]) {
	char filePathName[50];
	int tableNameLen = strlen(tableName);
	memset(filePathName, '\0', sizeof(filePathName));
	Strncat(filePathName, tableName, tableNameLen);
	Strncat(filePathName, "_bplustree.txt", 14);

	ofstream out;
	out.open(filePathName, ios::out);

	queue<Node*> Q; queue<int> L;
	Q.push(root); L.push(1);
	while (!Q.empty()) {
		Node* tmpQ = Q.front(); int tmpL = L.front();
		Q.pop(); L.pop();
		for (int i = 1; i <= tmpQ->num - 1; i++)
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

void BPlusTree::Save(char tableName[30]) {
	char filePathName[50];
	char tmp[30] = "_bplustree_data.txt";
	int tableNameLen = strlen(tableName);
	memset(filePathName, '\0', sizeof(filePathName));
	Strncat(filePathName, tableName, tableNameLen);
	Strncat(filePathName, tmp, 19);
	ofstream out;
	out.open(filePathName, ios::out);
	Node* pos = root;
	while (pos->is_leaf == false) {
		pos = pos->ch[1];
	}
	while (pos != NULL) {
		if (pos->next != NULL) {
			for (int i = 1; i <= pos->num; i++) {
				out << pos->key[i] << " " << pos->bytes[i] << endl;
			}
		}
		else {
			for (int i = 1; i <= pos->num - 1; i++) {
				out << pos->key[i] << " " << pos->bytes[i] << endl;
			}
			out << pos->key[pos->num] << " " << pos->bytes[pos->num];
		}
		pos = pos->next;
	}
	out.close();
	return;
}

Node* BPlusTree::Node_new() {
	Node* node = new Node;
	node->is_leaf = true;
	for (int i = 0; i < M + 5; i++) node->key[i] = 0;
	for (int i = 0; i < M + 5; i++) node->bytes[i] = 0;
	for (int i = 0; i < M + 5; i++) node->ch[i] = NULL;
	node->fath = NULL;
	node->prev = NULL; node->next = NULL;
	node->num = 0;
	return node;
}

void BPlusTree::Create() {
	Node* node = Node_new();
	root = node;
}

Node* BPlusTree::Search_leaf(Node* now, int target) {
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

long BPlusTree::Search_bytes(int target) {
	Node* pos = Search_leaf(root, target);
	int k = 0;
	for (int i = 1; i <= pos->num; i++) {
		if (target == pos->key[i]) {
			k = i;
			break;
		}
	}
	if (k == 0) return -1;//记录不存在
	return pos->bytes[k];
}

void BPlusTree::Search_rangebytes(int l, int r, int& result_num, long*& result_adr) {
	Node* pos = Search_leaf(root, l);
	while (pos != NULL) {
		for (int i = 1; i <= pos->num; i++) {
			if (l <= pos->key[i] && pos->key[i] <= r && result_num < 2000) {
				result_adr[result_num++] = pos->bytes[i];
			}
		}
		pos = pos->next;
	}
}

void BPlusTree::Split(Node* p1) {
	Node* fa = p1->fath;

	Node* p2 = Node_new();
	p2->is_leaf = p1->is_leaf;
	p2->fath = p1->fath;

	if (M % 2 == 0) {
		p1->num = M / 2;
		p2->num = M / 2 - 1;
	}
	else {
		p1->num = M / 2;
		p2->num = M / 2;
	}

	for (int i = 1; i <= p2->num; i++) {
		p2->key[i] = p1->key[p1->num + i + 1];
		p2->bytes[i] = p1->bytes[p1->num + i + 1];
	}
	if (p2->is_leaf == true) {
		for (int i = p2->num + 1; i >= 2; i--) {
			p2->key[i] = p2->key[i - 1];
			p2->bytes[i] = p2->bytes[i - 1];
		}
		p2->key[1] = p1->key[p1->num + 1];
		p2->bytes[1] = p1->bytes[p1->num + 1];
		p2->num++;
	}

	if (p2->is_leaf == false) {
		for (int i = 1; i <= p2->num + 1; i++) {
			p2->ch[i] = p1->ch[p1->num + i + 1];
			p1->ch[p1->num + i + 1]->fath = p2;
		}
	}

	bool flag = false;
	for (int i = 1; i <= fa->num; i++) {
		if (p1->key[p1->num + 1] <= fa->key[i]) {
			flag = true;
			for (int j = fa->num + 1; j >= i + 1; j--) {
				fa->key[j] = fa->key[j - 1];
				fa->bytes[j] = fa->bytes[j - 1];
			}
			for (int j = fa->num + 2; j >= i + 1; j--) fa->ch[j] = fa->ch[j - 1];
			fa->key[i] = p1->key[p1->num + 1];
			fa->bytes[i] = p1->bytes[p1->num + 1];
			fa->ch[i + 1] = p2;
			break;
		}
	}
	if (flag == false) {
		fa->key[fa->num + 1] = p1->key[p1->num + 1];
		fa->bytes[fa->num + 1] = p1->bytes[p1->num + 1];
		fa->ch[fa->num + 2] = p2;
	}
	fa->num++;

	p2->next = p1->next;
	if (p1->next != NULL) p1->next->prev = p2;
	p2->prev = p1;
	p1->next = p2;

	return;
}

bool BPlusTree::Insert(Node*& root, int target, long byt) {
	Node* pos = Search_leaf(root, target);

	int k = 0;
	for (int i = 1; i <= pos->num; i++) {
		if (target == pos->key[i]) {
			k = i;
			break;
		}
	}
	if (k != 0) return false;//记录已存在，不允许重复插入

	//插入
	bool flag = false;
	for (int i = 1; i <= pos->num; i++) {
		if (target <= pos->key[i]) {
			flag = true;
			for (int j = pos->num + 1; j >= i + 1; j--) {
				pos->key[j] = pos->key[j - 1];
				pos->bytes[j] = pos->bytes[j - 1];
			}
			pos->key[i] = target;
			pos->bytes[i] = byt;
			break;
		}
	}
	if (flag == false) {
		pos->key[pos->num + 1] = target;
		pos->bytes[pos->num + 1] = byt;
	}
	pos->num++;

	//分裂
	if (pos->num == M) {
		if (pos == root) {//根结点
			Node* node = Node_new();
			node->is_leaf = false;
			node->ch[1] = pos;
			pos->fath = node;//root->fath=node
			root = node;
			Split(pos);
		}
		else {//叶子结点
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
		if (p->is_leaf == true) {//叶结点
			for (int i = p->num + 1; i >= 2; i--) {
				p->key[i] = p->key[i - 1];
				p->bytes[i] = p->bytes[i - 1];
			}
			p->key[1] = leftsli->key[leftsli->num];
			p->bytes[1] = leftsli->bytes[leftsli->num];
			p->num++;
			leftsli->num--;
			for (int i = 1; i <= p->fath->num; i++) {
				if (p->fath->ch[i] == leftsli && p->fath->ch[i + 1] == p) {
					p->fath->key[i] = p->key[1];
					p->fath->bytes[i] = p->bytes[1];
					break;
				}
			}
		}
		else {//中间节点
			for (int i = p->num + 1; i >= 2; i--) {
				p->key[i] = p->key[i - 1];
				p->bytes[i] = p->bytes[i - 1];
			}
			for (int i = p->num + 2; i >= 2; i--) p->ch[i] = p->ch[i - 1];

			for (int i = 1; i <= p->fath->num; i++) {
				if (p->fath->ch[i] == leftsli && p->fath->ch[i + 1] == p) {
					p->key[1] = p->fath->key[i];
					p->bytes[1] = p->fath->bytes[i];
					p->fath->key[i] = leftsli->key[leftsli->num];
					p->fath->bytes[i] = leftsli->bytes[leftsli->num];
					p->ch[1] = leftsli->ch[leftsli->num + 1];
					p->ch[1]->fath = p;
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
		if (p->is_leaf == true) {//叶结点
			p->key[p->num + 1] = rightsli->key[1];
			p->bytes[p->num + 1] = rightsli->bytes[1];
			p->num++;
			for (int i = 1; i <= rightsli->num - 1; i++) {
				rightsli->key[i] = rightsli->key[i + 1];
				rightsli->bytes[i] = rightsli->bytes[i + 1];
			}
			rightsli->num--;
			for (int i = 1; i <= p->fath->num; i++) {
				if (p->fath->ch[i] == p && p->fath->ch[i + 1] == rightsli) {
					p->fath->key[i] = rightsli->key[1];
					p->fath->bytes[i] = rightsli->bytes[1];
					break;
				}
			}
		}
		else {//中间结点
			for (int i = 1; i <= p->fath->num; i++) {
				if (p->fath->ch[i] == p && p->fath->ch[i + 1] == rightsli) {
					p->key[p->num + 1] = p->fath->key[i];
					p->bytes[p->num + 1] = p->fath->bytes[i];
					p->num++;
					p->fath->key[i] = rightsli->key[1];
					p->fath->bytes[i] = rightsli->bytes[1];
					p->ch[p->num + 1] = rightsli->ch[1];
					p->ch[p->num + 1]->fath = p;
					for (int j = 1; j <= rightsli->num - 1; j++) {
						rightsli->key[j] = rightsli->key[j + 1];
						rightsli->bytes[j] = rightsli->bytes[j + 1];
					}
					for (int j = 1; j <= rightsli->num; j++) rightsli->ch[j] = rightsli->ch[j + 1];
					rightsli->num--;
					break;
				}
			}
		}

		return true;
	}

	return false;
}

bool BPlusTree::Merge(Node* p1, Node* p2) {
	if (p1 != NULL && p2 != NULL && p1->fath == p2->fath) {
		if (p1->is_leaf == true) {//叶结点
			for (int i = p1->num + 1, j = 1; j <= p2->num; i++, j++) {
				p1->key[i] = p2->key[j];
				p1->bytes[i] = p2->bytes[j];
			}
			p1->num += p2->num;
			if (p2->next != NULL) p2->next->prev = p1;
			p1->next = p2->next;
			p2->prev = NULL;
			p2->next = NULL;
			for (int i = 1; i <= p1->fath->num; i++) {
				if (p1->fath->ch[i] == p1 && p1->fath->ch[i + 1] == p2) {
					for (int j = i; j <= p1->fath->num - 1; j++) {
						p1->fath->key[j] = p1->fath->key[j + 1];
						p1->fath->bytes[j] = p1->fath->bytes[j + 1];
					}
					for (int j = i + 1; j <= p1->fath->num; j++) p1->fath->ch[j] = p1->fath->ch[j + 1];
					p1->fath->num--;
					break;
				}
			}
		}
		else {//中间结点
			for (int i = 1; i <= p1->fath->num; i++) {
				if (p1->fath->ch[i] == p1 && p1->fath->ch[i + 1] == p2) {
					p1->key[p1->num + 1] = p1->fath->key[i];
					p1->bytes[p1->num + 1] = p1->fath->bytes[i];
					p1->num += 1;
					for (int j = p1->num + 1, k = 1; k <= p2->num; j++, k++) {
						p1->key[j] = p2->key[k];
						p1->bytes[j] = p2->bytes[k];
					}
					for (int j = p1->num + 1, k = 1; k <= p2->num + 1; j++, k++) {
						p1->ch[j] = p2->ch[k];
						p1->ch[j]->fath = p1;
					}
					p1->num += p2->num;
					if (p2->next != NULL) p2->next->prev = p1;
					p1->next = p2->next;
					p2->prev = NULL;
					p2->next = NULL;
					for (int j = i; j <= p1->fath->num - 1; j++) {
						p1->fath->key[j] = p1->fath->key[j + 1];
						p1->fath->bytes[j] = p1->fath->bytes[j + 1];
					}
					for (int j = i + 1; j <= p1->fath->num; j++) p1->fath->ch[j] = p1->fath->ch[j + 1];
					p1->fath->num--;
					break;
				}
			}
		}

		return true;
	}

	return false;
}

bool BPlusTree::Delete(Node*& root, int target) {
	Node* pos = Search_leaf(root, target);

	int k = 0;
	for (int i = 1; i <= pos->num; i++) {
		if (target == pos->key[i]) {
			k = i;
			break;
		}
	}
	if (k == 0) return false;//记录不存在

	//删除
	for (int i = k; i <= pos->num - 1; i++) {
		pos->key[i] = pos->key[i + 1];
		pos->bytes[i] = pos->bytes[i + 1];
	}
	pos->num--;

	if (pos != root) {//不是根结点，需要考虑调整树结构
		//考虑删除之后对父结点的影响
		if (k == 1 && pos != pos->fath->ch[1]) {
			for (int i = 2; i <= pos->fath->num + 1; i++) {
				if (pos == pos->fath->ch[i]) {
					pos->fath->key[i - 1] = pos->key[1];
					pos->fath->bytes[i - 1] = pos->bytes[1];
					break;
				}
			}
		}

		//删除后结点个数满足条件，操作结束
		if (pos->num >= ceil(double(M) / 2) - 1) return true;

		//向兄弟结点借key
		bool flag = false;
		if (Left_transfer(pos) == true) flag = true;
		else if (Right_transfer(pos) == true) flag = true;
		if (flag == true) return true;

		//兄弟结点中没有富余的key,则当前结点和兄弟结点合并成一个新的叶子结点
		if (Merge(pos->prev, pos) == true) {}
		else if (Merge(pos, pos->next) == true) {}
		pos = pos->fath;
		//key的数量过少
		while (pos->num < ceil(double(M) / 2) - 1) {
			if (pos == root) {
				if (pos->num == 0) {
					root = root->ch[1];
					root->fath = NULL;
				}
				return true;
			}

			if (Left_transfer(pos) == true) flag = true;
			else if (Right_transfer(pos) == true) flag = true;
			if (flag == true) return true;

			if (Merge(pos->prev, pos) == true) {}
			else if (Merge(pos, pos->next) == true) {}
			pos = pos->fath;
		}
	}

	return true;
}

bool is_exist(BPlusTree& tmpbpt, char tableName[30]) {
	char filePathName[50];
	int tableNameLen = strlen(tableName);
	memset(filePathName, '\0', sizeof(filePathName));
	Strncat(filePathName, tableName, tableNameLen);
	Strncat(filePathName, "_bplustree_data.txt", 19);

	ifstream in;
	in.open(filePathName, ios::in);

	tmpbpt.Create();
	if (!in) {
		in.close();
		return false;
	}
	else {
		int tmpkey = 0;
		long tmpbytes = 0;
		while (!in.eof()) {
			in >> tmpkey >> tmpbytes;
			tmpbpt.Insert(tmpbpt.root, tmpkey, tmpbytes);
		}
		in.close();
		return true;
	}
}

void Strncat(char* s1, char* s2, int len) {
	int lens1 = strlen(s1), lens2 = strlen(s2);
	for (int i = lens1, j = 0; j < lens2; i++, j++) {
		s1[i] = s2[j];
	}
}
void Strncat(char* s1, const char* s2, int len) {
	int lens1 = strlen(s1), lens2 = strlen(s2);
	for (int i = lens1, j = 0; j < lens2; i++, j++) {
		s1[i] = s2[j];
	}
}
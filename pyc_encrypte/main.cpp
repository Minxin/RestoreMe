#include<iostream>
#include<string>
#include"SignalSlot.h"
using namespace std;

typedef struct listnode
{
	unsigned char val;
	listnode* Lnext;
	listnode* Rnext;
}ListNode,*PListNode;

class Machine1 :public MySlot
{
public:
	void encrypt(string& str)
	{
		int len = str.length();
		char temp;
		for (int i = 0; i < (len) / 2; ++i)
		{
			temp = str[i];
			str[i] = str[len - 1 - i];
			str[len - 1 - i] = temp;
		}
	}
};

class Machine2 :public MySlot
{
public:
	Machine2(ListNode* &BINTREE)
	{
		bintree = new ListNode;
		BINTREE=bintree;
	}
	void encrypt(string& str)
	{
		init(str);
		invertTree(bintree);	
	}
private:
	void init(string& str)
	{
		int len = str.length();
		ListNode* p;
		PListNode Q[830];
		int fornt=-1,rear = -1;
		p = bintree;	
		p->val = str[0];
		p->Lnext = NULL;
		p->Rnext = NULL;
		Q[++fornt] = p;
		rear = fornt + 1;
		for (int i = 1; i < len;++i)
		{
			if (fornt == rear)
				return;
			if(Q[fornt]->Lnext != NULL && Q[fornt]->Rnext != NULL)
			{
				fornt++;
			}
			if(Q[fornt]->Lnext == NULL || Q[fornt]->Rnext == NULL)
			{
				p = Q[fornt];
				if (p->Lnext == NULL)
				{
					p->Lnext = new ListNode;
					p = p->Lnext;
					p->val = str[i];
					p->Lnext = NULL;
					p->Rnext = NULL;
					Q[rear++] = p;
				}
				else
				{
					p->Rnext = new ListNode;
					p = p->Rnext;
					p->val = str[i];
					p->Lnext = NULL;
					p->Rnext = NULL;
					Q[rear++] = p;
				}
			}
		}
	}
	PListNode invertTree(PListNode root)
	{
		if (root == NULL) 
		{
			return root;
		}
		PListNode temp = root->Lnext;
		root->Lnext = root->Rnext;
		root->Rnext = temp;
		invertTree(root->Lnext);
		invertTree(root->Rnext);
		return root;
	}
	ListNode* bintree;
};

class Encryption
{
public:
	MySignal<string&> start;
	void nowStart(string& str)
	{
		start(str);
	}
};


void traversal(char* out,ListNode* root)
{
	PListNode Q[830];
	ListNode* p;
	int i=-1,fornt = -1,rear = -1;     //采用顺序队列，并假定不会上溢
	if (root == NULL)
		return;   //二叉树为空，算法结束
	Q[++rear] = root;         //根指针入队
	while (fornt != rear)     //当队列为空时
	{
		p = Q[++fornt];      //出队
		out[++i] = p->val;
		if (p->Lnext != NULL)
			Q[++rear] = p->Lnext;
		if (p->Rnext != NULL)
			Q[++rear] = p->Rnext;
	}
}
int main()
{
	Encryption enc;
	string str;
	char out[830];
	ListNode* Bintree;
	Machine1* mac1 = new Machine1;
	Machine2* mac2 = new Machine2(Bintree);
	FILE* pyc = fopen("game.pyc", "r");
	FILE* pycout = fopen("out.pyc", "wb");
	if (pyc == NULL)
		return 0;
	char pycbyte[830] = {0};
	fread(pycbyte, sizeof(char), 830, pyc);
	str.assign(pycbyte,sizeof(pycbyte)/sizeof(char));
	enc.start.connect(mac1, &Machine1::encrypt);
	enc.nowStart(str);
	enc.start.disconnect(mac1);
	enc.start.connect(mac2, &Machine2::encrypt);
	enc.nowStart(str);
	traversal(out, Bintree);
	fwrite(out, sizeof(out), 1, pycout);
	fclose(pyc);
	fclose(pycout);
	//getchar();
	return 0;
}
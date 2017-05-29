#include <stdio.h>

typedef struct listnode
{
	unsigned char val;
	listnode* Lnext;
	listnode* Rnext;
}ListNode, *PListNode;

void init(char* in,ListNode* &bintree)
{
	ListNode* NewNode = new ListNode;
	bintree = NewNode;
	ListNode* p;
	PListNode Q[830];
	int fornt = -1, rear = -1;
	p = NewNode;
	p->val = in[0];
	p->Lnext = NULL;
	p->Rnext = NULL;
	Q[++fornt] = p;
	rear = fornt + 1;
	for (int i = 1; i < 830; ++i)
	{
		if (fornt == rear)
			return;
		if (Q[fornt]->Lnext != NULL && Q[fornt]->Rnext != NULL)
		{
			fornt++;
		}
		if (Q[fornt]->Lnext == NULL || Q[fornt]->Rnext == NULL)
		{
			p = Q[fornt];
			if (p->Lnext == NULL)
			{
				p->Lnext = new ListNode;
				p = p->Lnext;
				p->val = in[i];
				p->Lnext = NULL;
				p->Rnext = NULL;
				Q[rear++] = p;
			}
			else
			{
				p->Rnext = new ListNode;
				p = p->Rnext;
				p->val = in[i];
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

void reverse(char* out, ListNode* root)
{
	PListNode Q[830];
	ListNode* p;
	int i = -1, fornt = -1, rear = -1;     //采用顺序队列，并假定不会上溢
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

void encrypt(char* str)
{
	char temp;
	for (int i = 0; i < (830) / 2; ++i)
	{
		temp = str[i];
		str[i] = str[830 - 1 - i];
		str[830 - 1 - i] = temp;
	}
}
int main()
{
	char out[830];
	ListNode* Bintree;
	FILE* pyc = fopen("in.pyc", "r");
	FILE* pycout = fopen("out.pyc", "wb");
	char pycbyte[830];
	fread(pycbyte, sizeof(char), 830, pyc);
	init(pycbyte, Bintree);
	invertTree(Bintree);
	reverse(out, Bintree);
	int len = sizeof(out);
	encrypt(out);
	fwrite(out, sizeof(out), 1, pycout);
}
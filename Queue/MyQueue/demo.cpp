#include<string>
#include<iostream>
#include "MyQueue.h"
using namespace std;

int main()
{
	MyQueue<string>* q = new MyQueue<string>(4);
	q->EnQueue("a");
	q->EnQueue("b");
	q->EnQueue("c");
	//q->EnQueue(4);
	if (q->QueueFull())
	{
		cout << "��������" << endl;
	}
	if (q->QueueEmpty())
	{
		cout << "�����ѿ�" << endl;
	}
	//q->ClearQueue();
	q->QueueTraverse();
	return 0;
}
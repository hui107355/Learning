#pragma once
#include<stdlib.h>
#include<iostream>
#include<string>
using namespace std;

template <typename T>
class MyQueue
{
public:
	MyQueue(int queueCapacity);			//��������
	~MyQueue();			//���ٶ���
	void ClearQueue();			//��ն���
	bool QueueEmpty() const;				//�пն��У������޸����ݳ�Ա��const����
	int QueueLength() const;					//���г���
	bool EnQueue(T element);			//���
	bool DeQueue(T& element);			//����
	void QueueTraverse();			//��������
	bool QueueFull();			//��������
private:
	T* m_pQueue;			//��������ָ��
	int m_QueueLen;			//���г���
	int m_QueueCapacity;			//��������
	int m_iHead;			//��ͷ����
	int m_iTail;			//��β����
};

template <typename T>
MyQueue<T>::MyQueue(int queueCapacity)
{
	m_QueueCapacity = queueCapacity;
	m_iHead = 0;
	m_iTail = 0;
	m_QueueLen = 0;
	m_pQueue = new T[m_QueueCapacity];
}

template <typename T>
MyQueue<T>::~MyQueue()
{
	delete []m_pQueue;
	m_pQueue = NULL;
}

template <typename T>
void MyQueue<T>::ClearQueue()
{
	m_iHead = 0;
	m_iTail = 0;
	m_QueueLen = 0;
}

template <typename T>
bool MyQueue<T>::QueueEmpty() const
{
	return m_QueueLen == 0 ? true : false;
}

template <typename T>
int MyQueue<T>::QueueLength() const
{
	return m_QueueLen;
}

template <typename T>
bool MyQueue<T>::QueueFull()
{
	if (m_QueueLen == m_QueueCapacity)
	{
		return true;
	}
	return false;
}

template <typename T>
bool MyQueue<T>::EnQueue(T element)
{
	if (!QueueFull())
	{
		m_QueueLen++;
		m_pQueue[m_iTail] = element;
		m_iTail++;
		m_iTail = m_iTail % m_QueueCapacity;
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool MyQueue<T>::DeQueue(T &element)
{
	if (QueueEmpty())
	{
		return false;
	}
	else
	{
		element = m_pQueue[m_iHead];
		m_iHead++;
		m_iHead = m_iHead % m_QueueCapacity;
		m_QueueLen--;
		return true;
	}
}

template <typename T>
void MyQueue<T>::QueueTraverse()
{
	for (int i = m_iHead;  i < m_iHead + m_QueueLen; i++)
	{
		cout << m_pQueue[i % m_QueueLen] << endl;
	}
}
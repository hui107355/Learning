#pragma once
#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;

template <typename T>
class MyStack
{
public:
	MyStack(int size);			//��������ʼ��ջ
	virtual ~MyStack();			//����ջ
	bool StackEmpty();			//�п�ջ
	bool StackFull();			//����ջ
	void ClearStack();			//���ջ
	int StackLength();			//ջ������Ԫ�ظ���
	bool push(T element);			//��ջ
	bool pop(T &element);		//��ջ
	void StackTraverse(bool isfromButtom);			//����ջ
private:
	T *m_pBuffer;			//ջ�ռ�ָ��
	int m_iSize;			//ջ�ռ��С
	int m_iTop;			//ջ������
};

template <typename T>
MyStack<T>::MyStack(int size)
{
	m_iSize = size;
	m_iTop = 0;
	m_pBuffer = new T(m_iSize);
}

template <typename T>
MyStack<T>::~MyStack()
{
	delete []m_pBuffer;
	m_pBuffer = NULL;
}

template <typename T>
bool MyStack<T>::StackEmpty()
{
	if (0 == m_iTop)
	{
		return true;
	}
	return false;
}

template <typename T>
bool MyStack<T>::StackFull()
{
	if (m_iTop == m_iSize)
	{
		return true;
	}
	return false;
}

template <typename T>
void MyStack<T>::ClearStack()
{
	m_iTop = 0;
}

template <typename T>
int MyStack<T>::StackLength()
{
	return m_iTop;
}

template <typename T>
bool MyStack<T>::push(T element)
{
	if (StackFull())
	{
		return false;
	}
	m_pBuffer[m_iTop] = element;
	m_iTop++;
	return true;
}

template <typename T>
bool MyStack<T>::pop(T &element)
{
	if (StackEmpty())
	{
		return false;
	}
	else
	{
		/*--m_iTop;
		element = m_pBuffer[m_iTop];*/
		element = m_pBuffer[--m_iTop];
		return true;
	}
}


template <typename T>
void MyStack<T>::StackTraverse(bool isfromButtom)
{
	if (false == isfromButtom)			//�������ϱ���
	{
		for (int i = 0; i < m_iTop; i++)
		{
			cout << m_pBuffer[i] << ",";
		}
	}
	else             //�������±���
	{
		for (int j = m_iTop - 1; j >= 0; j--)
		{
			cout << m_pBuffer[j] << ",";
		}
	}
}
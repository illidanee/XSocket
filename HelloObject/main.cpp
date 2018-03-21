#include "XObjectManager.h"
#include <stdlib.h>
#include <stdio.h>

class AAA : public XObjectManager<10, AAA>
{
public:
	AAA()
	{
		printf("AAA \n");
	}
	~AAA()
	{
		printf("~AAA \n");
	}
};

class BBB : public XObjectManager<20, BBB>
{
public:
	BBB(int n)
	{
		printf("BBB \n");
	}
	~BBB()
	{
		printf("~BBB \n");
	}
};

class CCC : public XObjectManager<30, CCC>
{
public:
	CCC(int n, int m)
	{
		printf("CCC \n");
	}
	~CCC()
	{
		printf("~CCC \n");
	}
};

int main()
{
	AAA* pA = AAA::CreateObject();
	AAA::DeleteObject(pA);

	BBB* pB = BBB::CreateObject(123);
	BBB::DeleteObject(pB);

	CCC* pC = CCC::CreateObject(123, 456);
	CCC::DeleteObject(pC);

	return 0;
}
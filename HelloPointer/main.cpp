#include <stdlib.h>
#include <stdio.h>
#include <memory>

class AAA
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

//����ָ�����ü�������
void Test1(std::shared_ptr<AAA> sObj)
{
	printf("use count = %d \n", sObj.use_count());
}

//����ָ�����ü���������
void Test2(std::shared_ptr<AAA>& sObj)
{
	printf("use count = %d \n", sObj.use_count());
}

int main()
{
	std::shared_ptr<AAA> sObj = std::make_shared<AAA>();
	printf("use count = %d \n", sObj.use_count());
	Test1(sObj);
	//Test2(sObj);
	printf("use count = %d \n", sObj.use_count());
}
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

int main()
{
	std::shared_ptr<int> p1 = std::make_shared<int>(1);
	std::shared_ptr<int> p2 = p1;
	//����ʹ��deleteɾ����
	//delete p1.get();
	std::cout << p2.use_count() << std::endl;
}
#include "XByteStream.h"

#include <string>

int main()
{
	bool ret;
	XByteStream bs(1024);

	//ret = bs.WriteInt8(1);
	//ret = bs.WriteInt16(2);
	//ret = bs.WriteInt32(3);
	//ret = bs.WriteInt64(4);
	//ret = bs.WriteFloat(5.6);
	//ret = bs.WriteDouble(7.8);

	char a[] = "hahah";
	ret = bs.WriteArray(a, strlen(a));

	int b[] = { 1, 3, 5 };
	ret = bs.WriteArray(b, sizeof(b));

	//int8_t r1;
	//ret = bs.ReadInt8(r1);

	//int16_t r2;
	//ret = bs.ReadInt16(r2);

	//int32_t r3;
	//ret = bs.ReadInt32(r3);

	//int64_t r4;
	//ret = bs.ReadInt64(r4);

	//float r5;
	//ret = bs.ReadFloat(r5);

	//double r6;
	//ret = bs.ReadDouble(r6);

	char aa[32] = {};
	ret = bs.ReadArray(aa, 32);

	int bb[32] = {};
	ret = bs.ReadArray(bb, 32);

	return 0;
}
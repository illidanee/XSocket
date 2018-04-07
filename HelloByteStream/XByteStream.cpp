#include "XByteStream.h"

XByteStream::XByteStream(char* pBuffer, size_t nSize, bool bDelete)
	:
	_pBuffer(pBuffer),
	_nSize(nSize),
	_bDelete(bDelete),
	_nReadOffset(0),
	_nWriteOffset(0)
{

}

XByteStream::XByteStream(size_t nSize)
	:
	_pBuffer(nullptr),
	_nSize(nSize),
	_bDelete(true),
	_nReadOffset(0),
	_nWriteOffset(0)
{
	_pBuffer = new char[_nSize];
}

XByteStream::~XByteStream()
{
	if (_pBuffer && _bDelete)
		delete[] _pBuffer;
}

bool XByteStream::ReadInt8(int8_t& num)
{
	return Read(num);
}

bool XByteStream::ReadInt16(int16_t& num)
{
	return Read(num);
}

bool XByteStream::ReadInt32(int32_t& num)
{
	return Read(num);
}

bool XByteStream::ReadInt64(int64_t& num)
{
	return Read(num);
}

bool XByteStream::ReadFloat(float& num)
{
	return Read(num);
}

bool XByteStream::ReadDouble(double& num)
{
	return Read(num);
}

bool XByteStream::WriteInt8(int8_t num)
{
	return Write(num);
}

bool XByteStream::WriteInt16(int16_t num)
{
	return Write(num);
}

bool XByteStream::WriteInt32(int32_t num)
{
	return Write(num);
}

bool XByteStream::WriteInt64(int64_t num)
{
	return Write(num);
}

bool XByteStream::WriteFloat(float num)
{
	return Write(num);
}

bool XByteStream::WriteDouble(double num)
{
	return Write(num);
}
#ifndef __XBYTESTREAM_H__
#define __XBYTESTREAM_H__

#include <stdint.h>
#include <memory.h>

class XByteStream
{
public:
	XByteStream(char* pBuffer, size_t nSize, bool bDelete = true);
	XByteStream(size_t nSize);
	~XByteStream();

	//Read
	template <typename T>
	bool Read(T& num)
	{
		size_t len = sizeof(T);
		if (_nReadOffset + len <= _nSize)
		{
			memcpy(&num, _pBuffer + _nReadOffset, len);
			_nReadOffset += len;
			return true;
		}

		return false;
	}

	template <typename T>
	bool OnlyRead(T& num)
	{
		size_t len = sizeof(int64_t);
		if (_nReadOffset + len <= _nSize)
		{
			memcpy(&num, _pBuffer + _nReadOffset, len);
			//_nReadOffset += len;
			return true;
		}

		return false;
	}

	template <typename T>
	size_t ReadArray(T* pBuffer, size_t nLength)
	{
		uint64_t nSize;
		bool ret = OnlyRead(nSize);
		if (ret && nSize < nLength)
		{
			if (_nReadOffset + sizeof(uint64_t) + nSize <= _nSize)
			{
				_nReadOffset += sizeof(uint64_t);
				memcpy(pBuffer, _pBuffer + _nReadOffset, nSize);
				_nReadOffset += nSize;
				return nSize;
			}
		}

		return 0;
	}

	bool ReadInt8(int8_t& num);
	bool ReadInt16(int16_t& num);
	bool ReadInt32(int32_t& num);
	bool ReadInt64(int64_t& num);
	bool ReadFloat(float& num);
	bool ReadDouble(double& num);

	//Write
	template <typename T>
	bool Write(T num)
	{
		size_t len = sizeof(T);
		if (_nWriteOffset + len <= _nSize)
		{
			memcpy(_pBuffer + _nWriteOffset, &num, len);
			_nWriteOffset += len;
			return true;
		}

		return false;
	}

	template <typename T>
	size_t WriteArray(T* pBuffer, size_t nSize)
	{
		if (_nWriteOffset + sizeof(uint64_t) + nSize <= _nSize)
		{
			WriteInt64(nSize);
			memcpy(_pBuffer + _nWriteOffset, pBuffer, nSize);
			_nWriteOffset += nSize;
			return nSize;
		}

		return 0;
	}

	bool WriteInt8(int8_t num);
	bool WriteInt16(int16_t num);
	bool WriteInt32(int32_t num);
	bool WriteInt64(int64_t num);
	bool WriteFloat(float num);
	bool WriteDouble(double num);

	char* GetBuffer();

	size_t GetReadOffset();
	size_t GetWriteOffset();
	void SetReadOffset(size_t offset);
	void SetWriteOffset(size_t offset);

private:
	char* _pBuffer;
	size_t _nSize;
	bool _bDelete;

	size_t _nReadOffset;
	size_t _nWriteOffset;
};

#endif
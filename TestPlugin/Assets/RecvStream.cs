using System;
using System.Text;

/****************************************************************************************************************
	Date   :  2018/04/20 9:34

	Author :  smile@illidan.org
	
	Brief  :  使用C#封装的接收字节流
****************************************************************************************************************/

public class RecvStream {

    private byte[] _pBuffer;                            //接收缓冲区
    private int _nOffset;                               //接收偏移位置

    public RecvStream(byte[] data)
    {
        _pBuffer = data;
    }

    public bool CanRead(int n)
    {
        return _pBuffer.Length - _nOffset >= n;
    }

    public int ReadLength()
    {
        return ReadInt32();
    }

    public MGS_TYPE ReadType()
    {
        return (MGS_TYPE)ReadInt32();
    }

    public sbyte ReadInt8(sbyte n = 0)
    {
        if (CanRead(1))
        {
            n = (sbyte)_pBuffer[_nOffset];
            _nOffset += 1;
        }

        return n;
    }

    public Int16 ReadInt16(Int16 n = 0)
    {
        if (CanRead(2))
        {
            n = BitConverter.ToInt16(_pBuffer, _nOffset);
            _nOffset += 2;
        }

        return n;
    }

    public Int32 ReadInt32(Int32 n = 0)
    {
        if (CanRead(4))
        {
            n = BitConverter.ToInt32(_pBuffer, _nOffset);
            _nOffset += 4;
        }

        return n;
    }

    public Int64 ReadInt64(Int64 n = 0)
    {
        if (CanRead(8))
        {
            n = BitConverter.ToInt64(_pBuffer, _nOffset);
            _nOffset += 8;
        }

        return n;
    }

    public float ReadFloat(float n = 0.0f)
    {
        if (CanRead(4))
        {
            n = BitConverter.ToSingle(_pBuffer, _nOffset);
            _nOffset += 4;
        }

        return n;
    }

    public double ReadDouble(double n = 0.0)
    {
        if (CanRead(8))
        {
            n = BitConverter.ToDouble(_pBuffer, _nOffset);
            _nOffset += 8;
        }

        return n;
    }

    public string ReadString(string s = "")
    {
        int nSize = ReadInt32();
        if (nSize > 0 && CanRead(nSize))
        {
            s = Encoding.UTF8.GetString(_pBuffer, _nOffset, nSize);
            _nOffset += nSize;
        }
        return s;
    }

    public Int32[] ReadInt32s(Int32[] arr = null)
    {
        int nLength = ReadInt32() / 4;
        arr = new Int32[nLength];
        for (int i = 0; i < nLength; ++i)
        {
            arr[i] = ReadInt32();
        }
        return arr;
    }
}

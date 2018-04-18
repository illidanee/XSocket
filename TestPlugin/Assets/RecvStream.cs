using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;

public class RecvStream {

    private byte[] _pBuffer;
    private int _nOffset;

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

    public byte ReadUInt8(byte n = 0)
    {
        if (CanRead(1))
        {
            n = _pBuffer[_nOffset];
            _nOffset += 1;
        }

        return n;
    }

    public UInt16 ReadUInt16(UInt16 n = 0)
    {
        if (CanRead(2))
        {
            n = BitConverter.ToUInt16(_pBuffer, _nOffset);
            _nOffset += 2;
        }

        return n;
    }

    public UInt32 ReadUInt32(UInt32 n = 0)
    {
        if (CanRead(4))
        {
            n = BitConverter.ToUInt32(_pBuffer, _nOffset);
            _nOffset += 4;
        }

        return n;
    }

    public UInt64 ReadUInt64(UInt64 n = 0)
    {
        if (CanRead(8))
        {
            n = BitConverter.ToUInt64(_pBuffer, _nOffset);
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
        if (CanRead(nSize))
        {
            s = Encoding.UTF8.GetString(_pBuffer, _nOffset, nSize);
            _nOffset += nSize;
        }
        return s;
    }

    public Int32[] ReadInt32s(Int32[] arr = null)
    {
        int nSize = ReadInt32() / 4;
        arr = new Int32[nSize];
        for (int i = 0; i < nSize; ++i)
        {
            arr[i] = ReadInt32();
        }
        return arr;
    }
}

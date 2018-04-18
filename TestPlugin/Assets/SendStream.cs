using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;

public class SendStream {

    private List<byte> _Buffer;

    public byte[] Array
    {
        get
        {
            return _Buffer.ToArray();
        }
    }

    public SendStream(int nSize = 128)
    {
        _Buffer = new List<byte>(nSize);
    }

    public void Finish()
    {
        Int32 nSize = _Buffer.Count + 4;
        _Buffer.InsertRange(0, BitConverter.GetBytes(nSize));
    }

    public void WriteType(MGS_TYPE type)
    {
        WriteInt32((Int32)type);
    }

    public void WriteInt8(sbyte n)
    {
        _Buffer.Add((byte)n);
    }

    public void WriteInt16(Int16 n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteInt32(Int32 n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteInt64(Int64 n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteUInt8(byte n)
    {
        _Buffer.Add(n);
    }

    public void WriteUInt16(UInt16 n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteUInt32(UInt32 n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteUInt64(UInt64 n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteFloat(float n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteDouble(double n)
    {
        _Buffer.AddRange(BitConverter.GetBytes(n));
    }

    public void WriteString(string s)
    {
        byte[] data = Encoding.UTF8.GetBytes(s);
        WriteInt32(data.Length);
        _Buffer.AddRange(data);
    }

    public void WriteInt32s(Int32[] arr)
    {
        WriteInt32(arr.Length * 4);
        for (int i = 0; i < arr.Length; ++i)
        {
            WriteInt32(arr[i]);
        }
    }
}

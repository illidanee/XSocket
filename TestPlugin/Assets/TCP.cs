using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class TCP : Interface {

	// Use this for initialization
	void Start () {
        Create();
        Connect();
	}
	
	// Update is called once per frame
	void Update () {
        if (IsRun())
            OnRun();

        //测试发送数据
        SendStream s = new SendStream();
        s.WriteType(MGS_TYPE.MSG_BYTESTREAM);
        s.WriteInt8(1);
        s.WriteInt16(2);
        s.WriteInt32(3);
        s.WriteInt64(4);
        s.WriteFloat(5.6f);
        s.WriteDouble(7.8);
        s.WriteString("client");
        int[] b = { 1, 2, 3, 4, 5 };
        s.WriteInt32s(b);
        s.Finish();

        SendMsg(s.Array);
    }

    private void OnDestroy()
    {
        Close();
    }

    public override void OnMsg(byte[] data)
    {
        //测试接收数据
        RecvStream s = new RecvStream(data);
        s.ReadLength();
        s.ReadType();
        sbyte n1 = s.ReadInt8();
        Int16 n2 = s.ReadInt16();
        Int32 n3 = s.ReadInt32();
        Int64 n4 = s.ReadInt64();
        float n5 = s.ReadFloat();
        double n6 = s.ReadDouble();
        string n7 = s.ReadString();
        Int32[] n8 = s.ReadInt32s();

        Debug.Log(n1);
        Debug.Log(n2);
        Debug.Log(n3);
        Debug.Log(n4);
        Debug.Log(n5);
        Debug.Log(n6);
        Debug.Log(n7);
        for (int i = 0; i < n8.Length; ++i)
        {
            Debug.Log(n8[i]);
        }
    }
}

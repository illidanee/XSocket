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
	void FixedUpdate () {
        if (IsRun())
            OnRun();

        //测试发送数据
        CppSendStream s = new CppSendStream();
        s.WriteInt8(1);
        s.Finish();
        SendStream(s.Obj);
    }

    private void OnDestroy()
    {
        Close();
    }

    public override void OnMsg(IntPtr data)
    {
        //测试接收数据
        CppRecvStream r = new CppRecvStream(data);
        sbyte n1 = r.ReadInt8();
        Debug.Log(n1);
    }
}

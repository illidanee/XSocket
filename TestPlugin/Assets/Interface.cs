using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Runtime.InteropServices;
using AOT;

public class Interface : MonoBehaviour {

    //声明代理
    public delegate void OnMsg(IntPtr csObj, IntPtr data, int len);

    //声明对象
    private GCHandle _SelfHandle;
    private IntPtr _SelfPtr = IntPtr.Zero;

    private IntPtr cppClient = IntPtr.Zero;
    public string ip = "192.168.0.99";
    public ushort port = 9090;

    //导入插件接口
    [DllImport("U3DPlugin")]
    public static extern void SetLogPath(string logPath);

    [DllImport("U3DPlugin")]
    public static extern IntPtr Open(IntPtr pObj, OnMsg pCallback);

    [DllImport("U3DPlugin")]
    public static extern void Connect(IntPtr pClient, string ip, ushort port);

    [DllImport("U3DPlugin")]
    public static extern bool IsRun(IntPtr pClient);

    [DllImport("U3DPlugin")]
    public static extern void OnRun(IntPtr pClient);

    [DllImport("U3DPlugin")]
    public static extern void Close(IntPtr pClient);

    //定义回调函数
    [MonoPInvokeCallback(typeof(OnMsg))]
    public static void OnRecvMsg(IntPtr csObj, IntPtr data, int len)
    {
        Debug.Log("OnRecvMsg:" + len);
    }

    // Use this for initialization
    void Start () {
        SetLogPath("./Server.log");

        _SelfHandle = GCHandle.Alloc(this);
        _SelfPtr = GCHandle.ToIntPtr(_SelfHandle);
        cppClient = Open(_SelfPtr, OnRecvMsg);
        Connect(cppClient, ip, port);
        Debug.Log("Client Connect successed!");
    }
	
	// Update is called once per frame
	void Update () {
		if (IsRun(cppClient))
        {
            OnRun(cppClient);
        }
	}

    private void OnDestroy()
    {
        Close(cppClient);
    }
}

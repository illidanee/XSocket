using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Runtime.InteropServices;
using AOT;

public enum MGS_TYPE
{
    MSG_HEADER,
    MSG_ERROR,
    MSG_HEART,
    MSG_BYTESTREAM
};

public class Interface : MonoBehaviour {

    //声明代理
    private delegate void OnMsgCallback(IntPtr csObj, IntPtr data, int len);

    //声明对象
    private GCHandle _SelfHandle;
    private IntPtr _SelfPtr = IntPtr.Zero;
    private IntPtr cppClient = IntPtr.Zero;

    public string ip = "192.168.0.99";
    public ushort port = 9090;

    //导入插件接口
    [DllImport("U3DPlugin")]
    private static extern void SetLogPath(string logPath);

    [DllImport("U3DPlugin")]
    private static extern IntPtr Open(IntPtr pObj, OnMsgCallback pCallback);

    [DllImport("U3DPlugin")]
    private static extern bool Connect(IntPtr pClient, string ip, ushort port);

    [DllImport("U3DPlugin")]
    private static extern void Disconnect(IntPtr pClient);

    [DllImport("U3DPlugin")]
    private static extern bool IsRun(IntPtr pClient);

    [DllImport("U3DPlugin")]
    private static extern void OnRun(IntPtr pClient);

    [DllImport("U3DPlugin")]
    private static extern void Close(IntPtr pClient);

    //[DllImport("U3DPlugin")]
    //private static extern int SendData(IntPtr pClient, IntPtr data, int len);

    [DllImport("U3DPlugin")]
    private static extern int SendStream(IntPtr pClient, IntPtr pStream);

    //定义回调函数
    [MonoPInvokeCallback(typeof(OnMsgCallback))]
    private static void OnRecvMsg(IntPtr csObj, IntPtr data, int len)
    {
        GCHandle h = GCHandle.FromIntPtr(csObj);
        Interface obj = h.Target as Interface;
        if (obj)
        {
            //byte[] str = new byte[len];
            //Marshal.Copy(data, str, 0, len);
            obj.OnMsg(data);
        }
    }

    //接口函数
    public void Create()
    {
        if (cppClient == IntPtr.Zero)
        {
            SetLogPath("./Server.log");
            _SelfHandle = GCHandle.Alloc(this);
            _SelfPtr = GCHandle.ToIntPtr(_SelfHandle);
            cppClient = Open(_SelfPtr, OnRecvMsg);
            
            //Debug.Log("Client Connect successed!");
        }
    }

    public bool Connect()
    {
        if (cppClient == IntPtr.Zero)
            return false;

        return Connect(cppClient, ip, port);
    }

    public void Disconnect()
    {
        if (cppClient == IntPtr.Zero)
            return;

        Disconnect(cppClient);
    }

    public void Close()
    {
        if (cppClient == IntPtr.Zero)
            return;

        Close(cppClient);
    }

    public bool IsRun()
    {
        if (cppClient == IntPtr.Zero)
            return false;

        return IsRun(cppClient);
    }

    public void OnRun()
    {
        if (cppClient == IntPtr.Zero)
            return;

        OnRun(cppClient);
    }

    public virtual void OnMsg(IntPtr data)
    {

    }

    //public void SendMsg(byte[] data)
    //{
    //    if (cppClient == IntPtr.Zero)
    //        return;

    //    int nSize = data.Length;
    //    IntPtr pBuffer = Marshal.AllocHGlobal(nSize);
    //    Marshal.Copy(data, 0, pBuffer, data.Length);

    //    SendData(cppClient, pBuffer, nSize);

    //    Marshal.FreeHGlobal(pBuffer);
    //}

    public void SendStream(IntPtr pStream)
    {
        if (cppClient == IntPtr.Zero)
            return;

        SendStream(cppClient, pStream);
    }

    // Use this for initialization
    void Start () {
    }
	
	// Update is called once per frame
	void Update () {
	}

    private void OnDestroy()
    {
    }
}

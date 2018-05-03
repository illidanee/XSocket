using System;
using System.Runtime.InteropServices;
using AOT;
using UnityEngine;

/****************************************************************************************************************
    Date   : 2018/04/22 10:18

    Author : smile@illidan.org

    Brief  : 封装C++ TCP Client 和 消息
****************************************************************************************************************/

public enum MGS_TYPE
{
    MSG_HEADER,
    MSG_ERROR,
    MSG_HEART,
    MSG_BYTESTREAM,
    MSG_ENROLL,
    MSG_ENROLL_RES,
    MSG_LOGIN,
    MSG_LOGIN_RES,
    MSG_SELFINFO,
    MSG_SELFINFO_RES,
    MSG_BOOKINFO,
    MSG_BOOKINFO_RES,
    MSG_FEEDBACK,
    MSG_FEEDBACK_RES
};

public class CppTcpClient : MonoBehaviour {

    //声明代理
    private delegate void OnMsgCallback(IntPtr csObj, IntPtr pStream);
    private delegate void OnLeaveCallback(IntPtr csObj);

    //声明对象
    private GCHandle _SelfHandle;
    private IntPtr _SelfPtr = IntPtr.Zero;
    private IntPtr cppClient = IntPtr.Zero;

    public string _IP = "192.168.0.99";
    public ushort _PORT = 9090;

    private bool _bClose = false;

    //导入插件接口
#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern void SetLogPath(string logPath);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern IntPtr Open(IntPtr pObj, OnMsgCallback pMsgCallback, OnLeaveCallback pLeaveCallback);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool Connect(IntPtr pClient, string ip, ushort port);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern void Disconnect(IntPtr pClient);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern void Close(IntPtr pClient);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool IsRun(IntPtr pClient);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern void OnRun(IntPtr pClient);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern int SendStream(IntPtr pClient, IntPtr pStream);

    //定义回调函数
    [MonoPInvokeCallback(typeof(OnMsgCallback))]
    private static void OnMsg(IntPtr csObj, IntPtr pStream)
    {
        GCHandle h = GCHandle.FromIntPtr(csObj);
        CppTcpClient obj = h.Target as CppTcpClient;
        if (obj)
        {
            obj.OnMsg(pStream);
        }
    }

    [MonoPInvokeCallback(typeof(OnLeaveCallback))]
    private static void OnLeave(IntPtr csObj)
    {
        GCHandle h = GCHandle.FromIntPtr(csObj);
        CppTcpClient obj = h.Target as CppTcpClient;
        if (obj)
        {
            obj.WaitClose();
        }
    }

    //接口函数
    public void Init()
    {
        if (cppClient != IntPtr.Zero)
            return;

        string path = Application.persistentDataPath + "/" + "CppTcpClient.log";
        SetLogPath(path);
        _SelfHandle = GCHandle.Alloc(this);
        _SelfPtr = GCHandle.ToIntPtr(_SelfHandle);

        //Open();
        //Connect();
    }

    public void Open()
    {
        if (cppClient != IntPtr.Zero)
            return;

        cppClient = Open(_SelfPtr, OnMsg, OnLeave);

        _bClose = false;
    }

    public bool Connect()
    {
        if (cppClient == IntPtr.Zero)
            return false;

        return Connect(cppClient, _IP, _PORT);
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
        cppClient = IntPtr.Zero;
    }

    public void WaitClose()
    {
        _bClose = true;
    }

    public void CheckClose()
    {
        if (_bClose)
        {
            Close();

            _bClose = false;
        }
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
        // ...
    }

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
	void FixedUpdate ()
    {
        // ...
        if (IsRun())
            OnRun();

        OnUpdate();

        CheckClose();
    }

    public virtual void OnUpdate()
    {

    }

    private void OnDestroy()
    {
        // ...
    }
}

using System;
using System.Text;
using System.Runtime.InteropServices;

/****************************************************************************************************************
	Date   :  2018/04/20 10:04
	 
	Author :  smile@illidan.org
	
	Brief  :  封装C++发送字节流 
****************************************************************************************************************/

public class CppSendStream {

    private IntPtr pCppStream = IntPtr.Zero;                             //C++字节流对象指针

    public IntPtr Obj
    {
        get
        {
            return pCppStream;
        }
    }



#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern IntPtr CppSendStreamCreate(int nSize);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern void CppSendStreamClose(IntPtr pStream);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool CppWriteInt8(IntPtr pStream, sbyte n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool CppWriteInt16(IntPtr pStream, Int16 n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool CppWriteInt32(IntPtr pStream, Int32 n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool CppWriteInt64(IntPtr pStream, Int64 n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool CppWriteFloat(IntPtr pStream, float n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern bool CppWriteDouble(IntPtr pStream, double n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern int CppWriteArray(IntPtr pStream, IntPtr pBuffer, int nSize);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern void CppFinish(IntPtr pStream, MGS_TYPE type);




    public CppSendStream(int nSize = 256)
    {
        pCppStream = CppSendStreamCreate(nSize);
    }

    ~CppSendStream()
    {
        CppSendStreamClose(pCppStream);
    }

    public bool WriteInt8(sbyte n)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        return CppWriteInt8(pCppStream, n);
    }

    public bool WriteInt16(Int16 n)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        return CppWriteInt16(pCppStream, n);
    }

    public bool WriteInt32(Int32 n)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        return CppWriteInt32(pCppStream, n);
    }

    public bool WriteInt64(Int64 n)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        return CppWriteInt64(pCppStream, n);
    }

    public bool WriteFloat(float n)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        return CppWriteFloat(pCppStream, n);
    }

    public bool WriteDouble(double n)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        return CppWriteDouble(pCppStream, n);
    }

    public bool WriteBytes(byte[] arr)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        if (!CppWriteInt32(pCppStream, arr.Length))
            return false;

        //IntPtr pArr = Marshal.AllocHGlobal(arr.Length);

        //Marshal.Copy(arr, 0, pArr, arr.Length);
        //CppWriteArray(pCppStream, pArr, arr.Length);

        //Marshal.FreeHGlobal(pArr);

        for (int i = 0; i < arr.Length; ++i)
        {
            CppWriteInt8(pCppStream, (sbyte)arr[i]);
        }

        return true;
    }

    public bool WriteString(string s)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        byte[] buffer = Encoding.UTF8.GetBytes(s);

        if (!CppWriteInt32(pCppStream, buffer.Length))
            return false;

        for (int i = 0; i < buffer.Length; ++i)
        {
            if (!CppWriteInt8(pCppStream, (sbyte)buffer[i]))
                return false;
        }

        return true;
    }

    public void Finish(MGS_TYPE type)
    {
        if (pCppStream == IntPtr.Zero)
            return;

        CppFinish(pCppStream, type);
    }
}

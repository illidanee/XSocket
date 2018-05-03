using System;
using System.Text;
using System.Runtime.InteropServices;

/****************************************************************************************************************
	Date   :  2018/04/20 10:04
	 
	Author :  smile@illidan.org
	
	Brief  :  封装C++接收字节流 
****************************************************************************************************************/

public class CppRecvStream
{
    private IntPtr pCppStream = IntPtr.Zero;                                     //C++字节流对象指针

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
    private static extern IntPtr CppRecvStreamCreate(IntPtr pMsgHeader);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern void CppRecvStreamClose(IntPtr pStream);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern sbyte CppReadInt8(IntPtr pStream, sbyte n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern Int16 CppReadInt16(IntPtr pStream, Int16 n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern Int32 CppReadInt32(IntPtr pStream, Int32 n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern Int64 CppReadInt64(IntPtr pStream, Int64 n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern float CppReadFloat(IntPtr pStream, float n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern double CppReadDouble(IntPtr pStream, double n);

#if UNITY_IPHONE && !UNITY_EDITOR
    [DllImport ("__Internal")]
#else
    [DllImport("U3DPlugin")]
#endif
    private static extern int CppReadArray(IntPtr pStream, IntPtr pBuffer, int nSize);



    public CppRecvStream(IntPtr pMsgHeader)
    {
        pCppStream = CppRecvStreamCreate(pMsgHeader);
    }
    
    ~CppRecvStream()
    {
        CppRecvStreamClose(pCppStream);
    }

    public int ReadLength(int n = 0)
    {
        if (pCppStream == IntPtr.Zero)
            return n;

        return CppReadInt32(pCppStream, n);
    }

    public MGS_TYPE ReadType(MGS_TYPE t = MGS_TYPE.MSG_ERROR)
    {
        if (pCppStream == IntPtr.Zero)
            return t;

        return (MGS_TYPE)CppReadInt32(pCppStream, (int)t);
    }

    public sbyte ReadInt8(sbyte n = 0)
    {
        if (pCppStream == IntPtr.Zero)
            return n;

        return CppReadInt8(pCppStream, n);
    }

    public Int16 ReadInt16(Int16 n = 0)
    {
        if (pCppStream == IntPtr.Zero)
            return n;

        return CppReadInt16(pCppStream, n);
    }

    public Int32 ReadInt32(Int32 n = 0)
    {
        if (pCppStream == IntPtr.Zero)
            return n;

        return CppReadInt32(pCppStream, n);
    }

    public Int64 ReadInt64(Int64 n = 0)
    {
        if (pCppStream == IntPtr.Zero)
            return n;

        return CppReadInt64(pCppStream, n);
    }

    public float ReadFloat(float n = 0.0f)
    {
        if (pCppStream == IntPtr.Zero)
            return n;

        return CppReadFloat(pCppStream, n);
    }

    public double ReadDouble(double n = 0.0)
    {
        if (pCppStream == IntPtr.Zero)
            return n;

        return CppReadDouble(pCppStream, n);
    }

    public byte[] ReadBytes(byte[] arr = null)
    {
        if (pCppStream == IntPtr.Zero)
            return arr;

        int length = ReadInt32();

        //IntPtr pArr = Marshal.AllocHGlobal(length);

        //CppReadArray(pCppStream, pArr, length);
        //arr = new byte[length];
        //Marshal.Copy(pArr, arr, 0, length);

        //Marshal.FreeHGlobal(pArr);

        arr = new byte[length];
        for (int i = 0; i < length; ++i)
        {
            sbyte sb = 0;
            arr[i] = (byte)CppReadInt8(pCppStream, sb);
        }

        return arr;
    }
    public string ReadString(string s = "")
    {
        if (pCppStream == IntPtr.Zero)
            return s;

        int nLength = CppReadInt32(pCppStream, 0);
        if (nLength <= 0)
            return s;

        byte[] buffer = new byte[nLength];

        for (int i = 0; i < nLength; ++i)
        {
            buffer[i] = (byte)CppReadInt8(pCppStream, 0);
        }

        s = Encoding.UTF8.GetString(buffer, 0, nLength);

        return s;
    }
}

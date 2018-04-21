using System;
using System.Runtime.InteropServices;
using System.Text;

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



    [DllImport("U3DPlugin")]
    private static extern IntPtr CppSendStreamCreate(int nSize);

    [DllImport("U3DPlugin")]
    private static extern void CppSendStreamClose(IntPtr pStream);

    [DllImport("U3DPlugin")]
    private static extern bool CppWriteInt8(IntPtr pStream, sbyte n);

    [DllImport("U3DPlugin")]
    private static extern bool CppWriteInt16(IntPtr pStream, Int16 n);

    [DllImport("U3DPlugin")]
    private static extern bool CppWriteInt32(IntPtr pStream, Int32 n);

    [DllImport("U3DPlugin")]
    private static extern bool CppWriteInt64(IntPtr pStream, Int64 n);

    [DllImport("U3DPlugin")]
    private static extern bool CppWriteFloat(IntPtr pStream, float n);

    [DllImport("U3DPlugin")]
    private static extern bool CppWriteDouble(IntPtr pStream, double n);

    [DllImport("U3DPlugin")]
    private static extern int CppWriteString(IntPtr pStream, IntPtr pBuffer, int nSize);

    [DllImport("U3DPlugin")]
    private static extern void CppFinish(IntPtr pStream);




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

    public bool WriteInt32s(Int32[] arr)
    {
        if (pCppStream == IntPtr.Zero)
            return false;

        if (!CppWriteInt32(pCppStream, arr.Length * 4))
            return false;
        for (int i = 0; i < arr.Length; ++i)
        {
            if (!CppWriteInt32(pCppStream, arr[i]))
                return false;
        }

        return true;
    }

    public void Finish()
    {
        if (pCppStream == IntPtr.Zero)
            return;

        CppFinish(pCppStream);
    }
}

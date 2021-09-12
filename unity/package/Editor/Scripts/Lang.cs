using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Lang : MonoBehaviour
{
    void OnApplicationQuit()
    {
        msg("CmdPeriod.run");
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void msg(string msg)
    {
        lang_msg(Marshal.StringToHGlobalAnsi(msg));
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void msg(TextAsset m)
    {
        msg(m.text);
    }

    [DllImport("unity")]
    private static extern void lang_msg(IntPtr msg);
}

using System;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEditor;

[InitializeOnLoad]
public class ToolMode
{
    static void Quit()
    {
        Debug.Log("Quitting Supercollider");
        server_stop();
        lang_stop();
    }

    static ToolMode()
    {
        SetPrintFunc(new Print_f(Print));

        server_new(
            57111, 
            getString("127.0.0.1"), 
            getString(Path.GetFullPath("Packages/com.supercollider/Resources/plugins"))
        );
        lang_new(
            getString(Path.GetFullPath("Packages/com.supercollider/Resources/config.yaml")),
            getString(Path.GetFullPath("Packages/com.supercollider/Resources/init.scd"))
        );
        EditorApplication.quitting += Quit;
    }

    static private IntPtr getString(string str)
    {
        return Marshal.StringToHGlobalAnsi(str);
    }

    [DllImport("unity")]
    private static extern void lang_new(IntPtr confFile, IntPtr initFile);

    [DllImport("unity")]
    private static extern void lang_stop();

    [DllImport("unity")]
    private static extern void server_new(int port, IntPtr host, IntPtr pluginsPath);

    [DllImport("unity")]
    private static extern void server_stop();

    [DllImport("libscsynth", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetPrintFunc(Print_f fp);

    private delegate int Print_f(IntPtr n, IntPtr args);
    private static int Print(IntPtr fmt, IntPtr args)
    {
        Debug.Log("[SUPERCOLLIDER]: " + Marshal.PtrToStringAnsi(fmt));
        return 0;
    }
}
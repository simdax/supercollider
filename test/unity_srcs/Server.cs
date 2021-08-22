using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Text;
using System.Runtime.InteropServices;

public class Server: MonoBehaviour
{
    [DllImport ("libscsynth", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr World_NewFromPath(IntPtr path);
    [DllImport ("libscsynth", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern int World_CloseUDP();
    [DllImport ("libscsynth", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern int World_Cleanup(IntPtr inWorld, bool unload);
    [DllImport ("libscsynth", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern int World_OpenUDP(IntPtr inWorld, IntPtr bindTo, int inPort);
    [DllImport ("libscsynth", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern void SetPrintFunc(Print_f fp);
    // [DllImport ("libscsynth", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    // private static extern int World_WaitForQuit(IntPtr inWorld, bool unload);
    // [DllImport ("libscsynth", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    // private static extern int World_OpenTCP(IntPtr inWorld, IntPtr bindTo, int inPort, int maxPort, int backlog);

    public delegate int Print_f( IntPtr n, IntPtr args);
    int Print(IntPtr fmt, IntPtr args) {
        Debug.Log(Marshal.PtrToStringAnsi(fmt));
        return 0;
    }

    IntPtr world;

    void Start()
    {
        StartCollider(
            "C:\\Users\\scornaz\\git\\supercollider\\test\\plugins",
            "127.0.0.1",
            57110
        );
    }

    void StartCollider(
        string pluginPath,
        string address, int port
    )
    {
        SetPrintFunc(new Print_f(Print));
        world = World_NewFromPath(Marshal.StringToHGlobalAnsi(pluginPath));
        if (World_OpenUDP(world, Marshal.StringToHGlobalAnsi(address), port) > 0)
        {
            Debug.Log("connected");
        }
    }
    
    void OnApplicationQuit()
    {
        Debug.Log("quitApplication");
        Debug.Log(World_CloseUDP());
        World_Cleanup(world, true);
    }
}

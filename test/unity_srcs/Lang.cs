using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Runtime.InteropServices;
using System.Text;

public class Lang: MonoBehaviour
{
    [DllImport("libsclang", EntryPoint="?createLanguageClient@@YAPEAVSC_LanguageClient@@PEBD@Z")]
    private static extern IntPtr createLanguageClient(StringBuilder name);
    [DllImport("libsclang", EntryPoint="?run@SC_TerminalClient@@UEAAHHPEAPEAD@Z")]
    private static extern void run(IntPtr client, int num, IntPtr args);

    IntPtr client;

    // [DllImport("libsclang")]
    // private static extern bool pyr_init_mem_pools(int runtime_space, int runtime_grow);
    // [DllImport("libsclang")]
    // private static extern void init_OSC(int port);
    // [DllImport("libsclang")]
    // private static extern void schedInit();
    // [DllImport("libsclang")]
    // private static extern bool compileLibrary(bool standalone);

    void Start()
    {
        client = createLanguageClient(new StringBuilder("coucou"));
        run(client, 0, IntPtr.Zero);
        // pyr_init_mem_pools(2 * 1024 * 1024, 256 * 1024);
        // init_OSC(57111);
        // schedInit();
        // compileLibrary(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

    // [DllImport("libsclang")]
    // private static extern void schedCleanup();
    // void cleanup_OSC();
    // void schedRun();
    // void schedStop();
    // void schedClear();
    // void runLibrary(struct PyrSymbol* selector);
    // void runInterpreter(struct VMGlobals* g, struct PyrSymbol* selector, int numArgsPushed);
    // struct VMGlobals* scGlobals();
    // struct PyrSymbol* getsym(const char* inName);
    // struct PyrSymbol* getmetasym(const char* name);
    // struct PyrSymbol* findsym(const char* name);


 
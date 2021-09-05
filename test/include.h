#pragma once

#include "SC_WorldOptions.h"
#include "SC_Version.hpp"
#include "SC_EventLoop.hpp"
#include "SC_ServerBootDelayWarning.h"
#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "clz.h"
#include <stdexcept>
#include <thread>
#ifdef _WIN32
#    include <winsock2.h>
#    include <vector>
#else
#    include <unistd.h> // for _POSIX_MEMLOCK
#    include <sys/wait.h>
#endif

#include "SC_LanguageClient.h"
#include <PyrPrimitive.h>
#include <iostream>

struct LangClient : SC_LanguageClient {
    FILE* gPostDest = stdout;
    bool useScprintf = false;

    LangClient(const char* name);
    void msg(const char* msg);

    void write(const char* str, size_t size, size_t len, FILE* out) {
        if (useScprintf) {
            scprintf(str);
        } else {
            fwrite(str, size, len, out);
        }
    }

    virtual void postText(const char* str, size_t len) { write(str, sizeof(char), len, gPostDest); }

    virtual void postFlush(const char* str, size_t len) {
        write(str, sizeof(char), len, gPostDest);
        fflush(gPostDest);
    }

    virtual void postError(const char* str, size_t len) {
        fprintf(gPostDest, "ERROR: ");
        write(str, sizeof(char), len, gPostDest);
    }

    virtual void flush() { fflush(gPostDest); }
};

// extern LangClient* gLangClient;
extern SC_LanguageClient* gLangClient;
extern std::thread* server_thread;
extern std::thread* lang_thread;

SCLANG_DLLEXPORT_C int server_new(int udpPortNum, const char* bindTo, const char* pluginPath);
SCLANG_DLLEXPORT_C void lang_new(const char* libPath);
SCLANG_DLLEXPORT_C void lang_msg(const char* msg);
SCLANG_DLLEXPORT_C void lang_tick();
SCLANG_DLLEXPORT_C void go();
SCLANG_DLLEXPORT_C void all_stop();
SCLANG_DLLEXPORT_C void lang_setPrintF();

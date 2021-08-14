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
#ifdef _WIN32
#    include <winsock2.h>
#    include <vector>
#else
#    include <unistd.h> // for _POSIX_MEMLOCK
#    include <sys/wait.h>
#endif
#include <thread>

#ifdef __COBALT__
#    include "XenomaiLock.h"
static XenomaiInitializer xenomaiInitializer;
#endif // __COBALT__

int synth_main();

#include "SC_LanguageClient.h"
#include <iostream>
struct LangClient : SC_LanguageClient {
    FILE* gPostDest = stdout;

    LangClient(const char* name): SC_LanguageClient(name) { }
    virtual void postText(const char* str, size_t len) { fwrite(str, sizeof(char), len, gPostDest); }
    virtual void postFlush(const char* str, size_t len) {
        fwrite(str, sizeof(char), len, gPostDest);
        fflush(gPostDest);
    }
    virtual void postError(const char* str, size_t len) {
        fprintf(gPostDest, "ERROR: ");
        fwrite(str, sizeof(char), len, gPostDest);
    }
    virtual void flush() { fflush(gPostDest); }
};
std::unique_ptr<LangClient> lang_main();


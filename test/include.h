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

int synth_main();

#include "SC_LanguageClient.h"
#include <PyrPrimitive.h>
#include <iostream>

struct LangClient : SC_LanguageClient {
    FILE* gPostDest = stdout;

    LangClient(const char* name);

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

    void msg(const char* msg);

    static int prScheduleChanged(VMGlobals* g, int numArgsPushed) {
        return 0;
    }

    virtual void onLibraryStartup() {
        int base, index = 0;

        // base = nextPrimitiveIndex();
        // definePrimitive(base, index++, "_Argv", &SC_TerminalClient::prArgv, 1, 0);
        // definePrimitive(base, index++, "_Exit", &SC_TerminalClient::prExit, 1, 0);
        // definePrimitive(base, index++, "_AppClock_SchedNotify", prScheduleChanged, 1, 0);
        // definePrimitive(base, index++, "_Recompile", &SC_TerminalClient::prRecompile, 1, 0);
    }
};

std::unique_ptr<LangClient> lang_main();

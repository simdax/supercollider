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

SCLANG_DLLEXPORT_C int server_new(int udpPortNum, const char* bindTo, const char* pluginPath);
SCLANG_DLLEXPORT_C void server_stop();
SCLANG_DLLEXPORT_C void lang_new(const char* confPath, const char *initFile);
SCLANG_DLLEXPORT_C void lang_msg(const char* msg);
SCLANG_DLLEXPORT_C void lang_stop();

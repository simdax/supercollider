/*
    SuperCollider real time audio synthesis system
    Copyright (c) 2002 James McCartney. All rights reserved.
    http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

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

#ifdef _WIN32

// according to this page: http://www.mkssoftware.com/docs/man3/setlinebuf.3.asp
// setlinebuf is equivalent to the setvbuf call below.
inline int setlinebuf(FILE* stream) { return setvbuf(stream, (char*)0, _IONBF, 0); }

#endif

int scsynth_main(int argc, char** argv) {
    int udpPortNum = 6767;
    std::string bindTo("127.0.0.1");
    WorldOptions options;

    struct World* world = World_New(&options);
    if (!world)
        return 1;
    if (udpPortNum >= 0) {
        if (!World_OpenUDP(world, bindTo.c_str(), udpPortNum)) {
            World_Cleanup(world, true);
            return 1;
        }
    }
    fflush(stdout);
    EventLoop::run([world]() { World_WaitForQuit(world, true); });
    return 0;
}

#ifdef _WIN32

int wmain(int argc, wchar_t** wargv) {
    // initialize winsock
    WSAData wsaData;
    int nCode;
    if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
        scprintf("WSAStartup() failed with error code %d.\n", nCode);
        return 1;
    }

    // convert args to utf-8
    std::vector<char*> argv;
    for (int i = 0; i < argc; i++) {
        auto argSize = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, nullptr, 0, nullptr, nullptr);
        argv.push_back(new char[argSize]);
        WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, argv[i], argSize, nullptr, nullptr);
    }

    // set codepage to UTF-8 and remember the old codepage
    auto oldCodePage = GetConsoleOutputCP();
    if (!SetConsoleOutputCP(65001))
        scprintf("WARNING: could not set codepage to UTF-8\n");

    auto t = std::thread([&]() {
        // run main
        int result = scsynth_main(argv.size(), argv.data());
        // clean up winsock
        WSACleanup();
        // reset codepage from UTF-8
        SetConsoleOutputCP(oldCodePage);
        // clear vector with converted args
        for (auto* arg : argv)
            delete[] arg;
        return result;
    });
    for (;;) { }
}
#else
int main(int argc, char** argv) { return scsynth_main(argc, argv); };
#endif //_WIN32

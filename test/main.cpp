#include "include.h"

auto server() {
    return std::thread([]() { synth_main(); });
}

#ifdef _WIN32

// according to this page: http://www.mkssoftware.com/docs/man3/setlinebuf.3.asp
// setlinebuf is equivalent to the setvbuf call below.
inline int setlinebuf(FILE* stream) { return setvbuf(stream, (char*)0, _IONBF, 0); }

int wmain(int argc, wchar_t** wargv) {
    // initialize winsock
    WSAData wsaData;
    int nCode;
    if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
        scprintf("WSAStartup() failed with error code %d.\n", nCode);
        return 1;
    }

    // set codepage to UTF-8 and remember the old codepage
    auto oldCodePage = GetConsoleOutputCP();
    if (!SetConsoleOutputCP(65001))
        scprintf("WARNING: could not set codepage to UTF-8\n");

    lang_main();
    auto t = server();
    for (;;) { }
    // clean up winsock
    WSACleanup();
    // reset codepage from UTF-8
    SetConsoleOutputCP(oldCodePage);
    // clear vector with converted args
}


#else
// int main(int argc, char** argv) { return scsynth_main(argc, argv); };
#endif //_WIN32

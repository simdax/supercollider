#include "include.h"
#include <SC_TerminalClient.h>

auto server() {
    return std::thread([]() { synth_main(); });
}

#ifdef _WIN32

// according to this page: http://www.mkssoftware.com/docs/man3/setlinebuf.3.asp
// setlinebuf is equivalent to the setvbuf call below.
inline int setlinebuf(FILE* stream) { return setvbuf(stream, (char*)0, _IONBF, 0); }

auto oldCodePage = GetConsoleOutputCP();

int initialize_winsock() {
    WSAData wsaData;
    int nCode;
    if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
        scprintf("WSAStartup() failed with error code %d.\n", nCode);
        return 1;
    }
    // set codepage to UTF-8 and remember the old codepage
    if (!SetConsoleOutputCP(65001))
        scprintf("WARNING: could not set codepage to UTF-8\n");
    return 0;
}

void cleanup() {
    WSACleanup();
    // reset codepage from UTF-8
    SetConsoleOutputCP(oldCodePage);
    // clear vector with converted args
}

int wmain(int argc, wchar_t** wargv) {
    initialize_winsock();
    auto t = server();
    // auto client = lang_main();
    auto client = static_cast<SC_TerminalClient*>(createLanguageClient("test"));
    client->run(0, nullptr);
    client->pushCmdLine("1 + 1\f", 6);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    destroyLanguageClient(client);
    // destroyLanguageClient(client.get());
    cleanup();
}


#else
// int main(int argc, char** argv) { return scsynth_main(argc, argv); };
#endif //_WIN32

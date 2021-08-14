#include "include.h"
#include <SC_TerminalClient.h>

auto server() {
    return std::thread([]() { synth_main(); });
}

#ifdef _WIN32

// according to this page: http://www.mkssoftware.com/docs/man3/setlinebuf.3.asp
// setlinebuf is equivalent to the setvbuf call below.
inline int setlinebuf(FILE* stream) { return setvbuf(stream, (char*)0, _IONBF, 0); }

int initialize_winsock() {
    WSAData wsaData;
    int nCode;
    if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
        scprintf("WSAStartup() failed with error code %d.\n", nCode);
        return 1;
    }
    return 0;
}

void cleanup() { WSACleanup(); }

int wmain(int argc, wchar_t** wargv) {
    initialize_winsock();
    auto server_thread = server();
    // auto client = lang_main();
    auto client = static_cast<SC_TerminalClient*>(createLanguageClient("test"));
    auto _ = std::thread([&]() { client->run(0, nullptr); });
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::string cmd = "a = s.options;"
                      "t = Server.remote(\"bob\", NetAddr(\"127.0.0.1\",57110), a, 0);\f";
    client->pushCmdLine(cmd.c_str(), cmd.size());
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    cmd = "(server:t).play\f";
    client->pushCmdLine(cmd.c_str(), cmd.size());
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cmd = "t.quit\f";
    client->pushCmdLine(cmd.c_str(), cmd.size());
    server_thread.join();
    cleanup();
    destroyLanguageClient(client);
}


#else
// int main(int argc, char** argv) { return scsynth_main(argc, argv); };
#endif //_WIN32

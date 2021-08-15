#include "include.h"
#include <SC_TerminalClient.h>

auto server() {
    return std::thread([]() { synth_main(); });
}

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
    auto client = lang_main();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    client->msg("Server.default = Server.remote('test', NetAddr(\"127.0.0.1\",57110), ServerOptions());");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    client->msg("{SinOsc.ar(mul:Env.perc.kr(2) * 0.3)}.play");
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    client->msg("s.quit");
    server_thread.join();
    // destroyLanguageClient(client);
    cleanup();
}

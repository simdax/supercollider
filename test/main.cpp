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
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // for(;;) {}
    auto client = lang_main();
    // auto client = static_cast<SC_TerminalClient*>(createLanguageClient("test"));
    // auto _ = std::thread([&]() { client->run(0, nullptr); });
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    client->msg("SynthDescLib.global.synthDescs.at(\\default)");
    client->msg("a = s.options;"
                "t = Server.remote(\"bob\", NetAddr(\"127.0.0.1\",57110), a, 0);");
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    client->msg("(server:t).play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    client->msg("t.quit");
    server_thread.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // destroyLanguageClient(client);
    cleanup();
}

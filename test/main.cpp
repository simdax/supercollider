#include "include.h"
#include <SC_TerminalClient.h>

SCLANG_DLLEXPORT_C void go()
{
    auto server_thread = std::thread([]() { synth_main(); });
    // std::this_thread::sleep_for(std::chrono::milliseconds(300));
    // auto client = lang_main();
    // client->msg("t = Server.remote('test', NetAddr(\"127.0.0.1\",57110), ServerOptions());"
    //             "Server.default = t;");
    // bool stop = false;
    // auto _t = std::thread([&]() {
    //     while (!stop) {
    //         client->tick();
    //         std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //     }
    // });
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // client->msg("(note:4).play");
    // client->msg("(note:0).play");
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // stop = true;
    // client->msg("s.quit");
    // client->tick();
    // _t.join();
    // server_thread.join();
    std::cout << "bye";
}

int main(int argc, char** wargv) {
    go();
    return 0;
}

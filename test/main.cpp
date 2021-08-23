#include "include.h"
#include <SC_TerminalClient.h>

std::thread *server_thread = nullptr;
std::thread *lang_thread = nullptr;

SCLANG_DLLEXPORT_C void go()
{
    server_thread = new std::thread([]() { synth_main(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    get_lang_main();
    lang_msg("t = Server.remote('test', NetAddr(\"127.0.0.1\",57110), ServerOptions());"
                "Server.default = t;");
    lang_thread = new std::thread([&]() {
        while (true) {
            lang_tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

int main(int argc, char** wargv) {
    go();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    lang_msg("(note:4).play");
    lang_msg("(note:0).play");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    lang_msg("s.quit");
    lang_tick();
    return 0;
}

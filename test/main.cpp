#include "include.h"

std::thread* server_thread = nullptr;
std::thread* lang_thread = nullptr;

SCLANG_DLLEXPORT_C void go() {
    server_thread = new std::thread(
        []() { server_new(57110, "127.0.0.1", "C:/Users/scornaz/git/supercollider/test/unity_srcs/plugins"); });
    lang_new("C:/Users/scornaz/git/supercollider/test/build/unity_srcs");
    lang_msg("t = Server.remote('test', NetAddr(\"127.0.0.1\",57110), ServerOptions());"
                 "Server.default = t;");
    lang_thread = new std::thread([]() {
        while (true) {
            lang_tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    lang_msg("(note:4).play");
    lang_msg("(note:0).play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

int main(int argc, char** argv) {
    go();
    // lang_msg("(note:4).play");
    // lang_msg("(note:0).play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    lang_msg("s.quit");
    lang_tick();
    return 0;
}

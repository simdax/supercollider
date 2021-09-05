#include "include.h"

SCLANG_DLLEXPORT_C void go() {
    auto port = 57111;
    auto host = "127.0.0.1";

    server_new(port, host, "C:/Users/scornaz/git/supercollider/test/plugins");
    lang_new("C:/Users/scornaz/git/supercollider/test");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // lang_msg(((std::stringstream("") << "t = Server.remote('test', NetAddr(\"" << host << "\"," << port
    //                                  << "), ServerOptions());"
    //                                     "Server.default = t;"))
    //              .str()
    //              .c_str());
}

// SCLANG_DLLEXPORT_C void go() {
//     auto port = 57111;
//     auto host = "127.0.0.1";

//     server_new(port, host, "C:/Users/scornaz/git/supercollider/test/plugins");
//     lang_new("C:/Users/scornaz/git/supercollider/test");
// }

SCLANG_DLLEXPORT_C void all_stop() {
    server_stop();
    lang_stop();
}

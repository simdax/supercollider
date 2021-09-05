#include "include.h"
#include <SC_LanguageConfig.hpp>

SC_LanguageClient* gLangClient = nullptr;
// LangClient* gLangClient = nullptr;
std::thread* server_thread = nullptr;
std::thread* lang_thread = nullptr;
std::atomic_bool stop = false;

SCLANG_DLLEXPORT_C void go() {
    auto port = 57111;
    auto host = "127.0.0.1";

    server_thread = new std::thread(
        [=]() { server_new(port, host, "C:/Users/scornaz/git/supercollider/test/plugins"); });
    lang_new("C:/Users/scornaz/git/supercollider/test");
    lang_msg(((std::stringstream("") << "t = Server.remote('test', NetAddr(\"" << host << "\"," << port
                                     << "), ServerOptions());"
                                        "Server.default = t;"))
                 .str()
                 .c_str());
    lang_thread = new std::thread([&]() {
        while (!stop) {
            lang_tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

SCLANG_DLLEXPORT_C void all_stop() {
    if (server_thread == nullptr || lang_thread == nullptr || gLangClient == nullptr) {
        return;
    }
    lang_msg("s.quit");
    server_thread->join();
    World_CloseUDP();
    gLangClient->shutdownLibrary();
    gLangClient->shutdownRuntime();
    stop = true;
    lang_thread->join();
    delete server_thread;
    delete lang_thread;
    destroyLanguageClient(gLangClient);
    server_thread = nullptr;
    lang_thread = nullptr;
    gLangClient = nullptr;
    stop = false;
}

SCLANG_DLLEXPORT_C int server_new(int udpPortNum, const char* bindTo, const char* pluginPath) {
    WorldOptions options;

    options.mMaxLogins = 2;
    options.mUGensPluginPath = pluginPath;
    World* world = World_New(&options);
    if (!world)
        return 1;
    if (udpPortNum >= 0) {
        if (!World_OpenUDP(world, bindTo, udpPortNum)) {
            World_Cleanup(world, true);
            return 1;
        }
    }
    fflush(stdout);
    EventLoop::run([&]() { World_WaitForQuit(world, true); });
    return 0;
}

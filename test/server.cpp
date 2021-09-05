#include "include.h"
#include <SC_LanguageConfig.hpp>

SC_LanguageClient* gLangClient = nullptr;
std::thread* server_thread = nullptr;

void server_stop() {
    if (server_thread == nullptr) {
        return;
    }
    lang_msg("s.quit");
    server_thread->join();
    World_CloseUDP();
    delete server_thread;
    server_thread = nullptr;
}

int pserver_new(int udpPortNum, const char* bindTo, const char* pluginPath) {
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

int server_new(int udpPortNum, const char* bindTo, const char* pluginPath) {
    server_thread = new std::thread(std::bind(pserver_new, udpPortNum, bindTo, pluginPath));
    return 0;
}
#include "include.h"
#include <SC_LanguageConfig.hpp>

LangClient::LangClient(const char* libraryPath): SC_LanguageClient("C_API") {
    SC_LanguageClient::Options options;

    initRuntime(options);
    compileLibrary(false);
}

void LangClient::msg(const char* msg) {
    setCmdLine(msg);
    interpretPrintCmdLine();
    flush();
}

LangClient* gLangClient = nullptr;
std::thread* server_thread = nullptr;
std::thread* lang_thread = nullptr;
std::atomic_bool stop = false;

SCLANG_DLLEXPORT_C void go() {
    server_thread = new std::thread(
        []() { 
            server_new(57110, "127.0.0.1", "C:/Users/scornaz/git/supercollider/test/unity_srcs/plugins"); 
        });
    lang_new("C:/Users/scornaz/git/supercollider/test/unity_srcs");
    lang_msg("t = Server.remote('test', NetAddr(\"127.0.0.1\",57110), ServerOptions());"
                 "Server.default = t;");
    lang_thread = new std::thread([]() {
        while (!stop) {
            lang_tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

SCLANG_DLLEXPORT_C void all_stop() 
{
    lang_msg("s.quit");
    server_thread->join();
    World_CloseUDP(); 
    gLangClient->shutdownLibrary();
    gLangClient->shutdownRuntime();
    stop = true;
    lang_thread->join();
    delete server_thread;
    delete lang_thread;
    delete gLangClient;
    server_thread = nullptr;
    lang_thread = nullptr;
    gLangClient = nullptr;
    stop = false;
}

SCLANG_DLLEXPORT_C void lang_new(const char *libPath) {
    gLangClient = new LangClient(libPath);
}

SCLANG_DLLEXPORT_C void lang_msg(const char* msg) {
    if (gLangClient) {
        gLangClient->msg(msg);
    }
}

SCLANG_DLLEXPORT_C void lang_tick() {
    if (gLangClient) {
        gLangClient->tick();
    }
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
    EventLoop::run([world]() { World_WaitForQuit(world, true); });
    return 0;
}

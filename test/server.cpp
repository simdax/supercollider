#include "include.h"
#include <PyrSymbolTable.h>

LangClient::LangClient(const char* libraryPath): SC_LanguageClient("C_API") {
    SC_LanguageClient::Options options;

    options.mRuntimeDir = const_cast<char*>(libraryPath);
    initRuntime(options);
    compileLibrary(false);
}

void LangClient::msg(const char* msg) {
    setCmdLine(msg);
    interpretPrintCmdLine();
    flush();
}

LangClient* gLangClient = nullptr;

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

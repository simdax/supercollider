#include "include.h"
#include <PyrSymbolTable.h>

LangClient::LangClient(const char* name): SC_LanguageClient(name) {
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

SCLANG_DLLEXPORT_C void* get_lang_main() {
    gLangClient = new LangClient("test");
    return (void*)gLangClient;
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

SCLANG_DLLEXPORT_C int synth_main() {
    int udpPortNum = 57110;
    std::string bindTo("127.0.0.1");
    WorldOptions options;

    options.mMaxLogins = 2;
    // options.mUGensPluginPath = "C:\\Users\\scornaz\\git\\supercollider\\test\\plugins"; 
    World* world = World_New(&options);
    if (!world)
        return 1;
    if (udpPortNum >= 0) {
        if (!World_OpenUDP(world, bindTo.c_str(), udpPortNum)) {
            World_Cleanup(world, true);
            return 1;
        }
    }
    fflush(stdout);
    EventLoop::run([world]() { World_WaitForQuit(world, true); });
    return 0;
}

#include "include.h"
#include <PyrSymbolTable.h>

int synth_main() {
    int udpPortNum = 57110;
    std::string bindTo("127.0.0.1");
    WorldOptions options;

    options.mMaxLogins = 1;
    options.mUGensPluginPath = "C:\\Users\\scornaz\\git\\supercollider\\test\\plugins"; 
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

LangClient::LangClient(const char* name): SC_LanguageClient(name) {
    SC_LanguageClient::Options options;

    // options.mPort = 57120;
    initRuntime(options);
    compileLibrary(false);
}

void LangClient::msg(const char* msg) {
    setCmdLine(msg);
    interpretPrintCmdLine();
    flush();
}

std::unique_ptr<LangClient> lang_main() { return std::make_unique<LangClient>("test"); }

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

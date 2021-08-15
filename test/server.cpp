#include "include.h"
#include <PyrSymbolTable.h>

int synth_main() {
    int udpPortNum = 57110;
    std::string bindTo("127.0.0.1");
    WorldOptions options;

    options.mMaxLogins = 1;
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

LangClient::LangClient(const char *name): SC_LanguageClient(name)
{
    SC_LanguageClient::Options options;

    // options.mPort = 57120;
    initRuntime(options);
    compileLibrary(false);
}

void LangClient::msg(const char *msg)
{
    setCmdLine(msg);
    runLibrary("interpretPrintCmdLine");
    flush();
}

std::unique_ptr<LangClient> lang_main() {
    return std::make_unique<LangClient>("test");
}

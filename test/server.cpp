#include "include.h"

int synth_main() {
    int udpPortNum = 57110;
    std::string bindTo("127.0.0.1");
    WorldOptions options;

    struct World* world = World_New(&options);
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

std::unique_ptr<LangClient> lang_main() {
    SC_LanguageClient::Options options;
    auto client = std::make_unique<LangClient>("test");

    options.mPort = 57120;
    client->initRuntime(options);
    client->compileLibrary(false);
    return client;
}

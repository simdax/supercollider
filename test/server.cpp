#include <iostream>
#include "include.h"
#include "SC_LanguageClient.h"

int synth_main() {
    int udpPortNum = 6767;
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

struct LangClient : SC_LanguageClient {
    FILE* gPostDest = stdout;

    LangClient(const char* name): SC_LanguageClient(name) { }

    virtual void postText(const char* str, size_t len) { fwrite(str, sizeof(char), len, gPostDest); }

    virtual void postFlush(const char* str, size_t len) {
        fwrite(str, sizeof(char), len, gPostDest);
        fflush(gPostDest);
    }

    virtual void postError(const char* str, size_t len) {
        fprintf(gPostDest, "ERROR: ");
        fwrite(str, sizeof(char), len, gPostDest);
    }

    virtual void flush() { fflush(gPostDest); }
};

int lang_main() {
    LangClient client("test");
    SC_LanguageClient::Options options;
    options.mPort = 6768;

    client.initRuntime(options);
    client.compileLibrary(false);
    // int returnCode = client->run(argc, argv);
    // destroyLanguageClient(client);
    return 0;
}

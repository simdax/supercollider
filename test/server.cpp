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

int lang_main(int argc, char** argv) {
    SC_LanguageClient* client = createLanguageClient("sclang");
    if (!client)
        return 1;
    int returnCode = client->run(argc, argv);
    destroyLanguageClient(client);
    return returnCode;
}

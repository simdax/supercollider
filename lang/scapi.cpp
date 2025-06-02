#include "scapi.h"
#include <vector>

World* StartServer() {
    WorldOptions Options;

    Options.mUGensPluginPath = "C:\\Users\\s.cornaz\\SCBundle\\SuperCollider\\plugins";
    Options.mSharedMemoryID = 57110;
    auto* World = World_New(&Options);
    World_OpenUDP(World, "127.0.0.1", 57110);
    return World;
}

SC_LanguageClient* CreateClient() { return createLanguageClient("test"); }

void StartClient(SC_LanguageClient* Client, char* path, bool daemon) {
    if (Client) {
        std::vector<char*> argv = { "", "--include-path", path, "D:/My project/Assets/Scripts/server.scd" };
        if (daemon) {
            //argv.push_back("-D");
        }
        Client->run(argv.size(), argv.data());
    }
}

void PlayFile(SC_LanguageClient* Client, const char* path) { Client->executeFile(path); }

void PlayString(SC_LanguageClient* Client, const char* cmdline) {
    Client->setCmdLine(cmdline);
    Client->interpretCmdLine();
}

void SetLogFD(const char* path) {
    // gPostDest = fopen(path, "w");
}

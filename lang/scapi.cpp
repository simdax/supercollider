#include <vector>
#include <iostream>

#include "scapi.h"
#include "PyrLexer.h"
#include "SC_WorldOptions.h"
#include "SC_TerminalClient.h"
#include "SC_CLIOptions.hpp"

World* StartServer() {
    WorldOptions Options;

    Options.mUGensPluginPath = "C:\\Users\\s.cornaz\\SCBundle\\SuperCollider\\plugins";
    Options.mSharedMemoryID = 57110;
    auto* World = World_New(&Options);
    World_OpenUDP(World, "127.0.0.1", 57110);
    return World;
}

void KillClient(SC_LanguageClient* Client) {
    if (SC_TerminalClient* client = static_cast<SC_TerminalClient*>(Client)) {
        client->onQuit(0);
    }
}

SC_LanguageClient* StartClient(char* path) {
    SC_LanguageClient* Client =
        SC_LanguageClient::instance() ? SC_LanguageClient::instance() : createLanguageClient("test");

    std::thread lang([Client, p = std::string(path)]() {
        if (Client) {
            auto* World = StartServer();
            std::vector<char*> argv = { "", "-a", "--include-path", const_cast<char*>(p.c_str()),
                                        "D:/My project/Assets/Scripts/server.scd" };

            Client->run(argv.size(), argv.data());
            destroyLanguageClient(Client);
            World_Cleanup(World, true);
        }
    });
    lang.detach();
    return Client;
}

void PlayFile(SC_LanguageClient* Client, const char* path) { Client->executeFile(path); }

void PlayString(SC_LanguageClient* Client, const char* cmdline) {
    Client->setCmdLine(cmdline);
    Client->interpretCmdLine();
}

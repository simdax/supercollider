#include "include.h"
#include <SC_TerminalClient.h>

std::thread* lang_thread = nullptr;
SC_LanguageClient* gLangClient = nullptr;

void lang_new(const char* confPath, const char *initFile) {
    if (gLangClient) 
    {
        return ;
    }
    SC_LanguageClient::Options options;
    gLangClient = createLanguageClient("test");
    lang_thread = new std::thread([=]() {
        std::vector<const char*> args{
            "fakebin",
            "-D",
            "-l", confPath,
            initFile
        };
        gLangClient->run(args.size(), const_cast<char**>(reinterpret_cast<const char**>(args.data())));
        destroyLanguageClient(gLangClient);
        gLangClient = nullptr;
    });
}

void lang_stop() {
    if (lang_thread == nullptr || gLangClient == nullptr) {
        return;
    }
    if (auto client = dynamic_cast<SC_TerminalClient*>(gLangClient))
    {
        client->stop();
        lang_thread->join();
        // Lang_CloseUDP();
        delete lang_thread;
        lang_thread = nullptr;
    }
}

void lang_msg(const char* msg) {
    if (gLangClient) {
        gLangClient->setCmdLine(msg);
        gLangClient->interpretPrintCmdLine();
        gLangClient->flush();
    }
}

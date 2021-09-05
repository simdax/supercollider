#include "include.h"
#include <SC_TerminalClient.h>

std::thread* lang_thread = nullptr;
SC_LanguageClient* gLangClient = nullptr;

void lang_new(const char* libPath) {
    SC_LanguageClient::Options options;
    gLangClient = createLanguageClient("test");
    lang_thread = new std::thread([]() {
        std::vector<char*> args({
            "fakebin",
            "-D",
            //  "-a",
            "-d", "C:/Users/scornaz/git/supercollider/test",
            "-l", "C:/Users/scornaz/git/supercollider/test/sclang_conf.yaml",
            "C:/Users/scornaz/git/supercollider/test/test.scd"
        });
        gLangClient->run(args.size(), reinterpret_cast<char**>(args.data()));
        destroyLanguageClient(gLangClient);
        gLangClient = nullptr;
    });
}

void lang_stop() {
    if (lang_thread == nullptr || gLangClient == nullptr) {
        return;
    }
    auto client = dynamic_cast<SC_TerminalClient*>(gLangClient);
    client->stop();
    lang_thread->join();
    delete lang_thread;
    lang_thread = nullptr;
}

void lang_msg(const char* msg) {
    if (gLangClient) {
        gLangClient->setCmdLine(msg);
        gLangClient->interpretPrintCmdLine();
        gLangClient->flush();
    }
}

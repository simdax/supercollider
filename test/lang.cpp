#include "include.h"
#include <SC_TerminalClient.h>

std::atomic_bool stop = false;
std::thread* lang_thread = nullptr;
std::thread* lang_tick_thread = nullptr;
SC_LanguageClient* gLangClient = nullptr;

void lang_new(const char* libPath) {
    SC_LanguageClient::Options options;
    gLangClient = createLanguageClient("test");
    lang_thread = new std::thread([]() {
        std::vector<char*> args({
            "fakebin",
            "-D",
            // "-a",
            // "-r", "C:/Users/scornaz/git/supercollider/test",
            // "-l", "C:/Users/scornaz/git/supercollider/test/sclang_conf.yaml"
        });
        gLangClient->run(args.size(), reinterpret_cast<char**>(args.data()));
        destroyLanguageClient(gLangClient);
        gLangClient = nullptr;
    });
    stop = false;
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

LangClient::LangClient(const char* libraryPath): SC_LanguageClient("C_API") {
    SC_LanguageClient::Options options;

    initRuntime(options);
    compileLibrary(false);
}

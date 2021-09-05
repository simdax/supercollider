#include "include.h"
#include <SC_TerminalClient.h>

// SCLANG_DLLEXPORT_C void lang_new(const char* libPath) {
//     gLangClient = createLanguageClient("test");
//     lang_thread = new std::thread([&]() {
//         gLangClient->run({}, 0);
//     });
// }

std::atomic_bool stop = false;
std::thread* lang_thread = nullptr;

void lang_stop() {
    if (lang_thread == nullptr || gLangClient == nullptr) {
        return;
    }
    gLangClient->shutdownLibrary();
    gLangClient->shutdownRuntime();
    stop = true;
    destroyLanguageClient(gLangClient);
    lang_thread->join();
    delete lang_thread;
    lang_thread = nullptr;
    gLangClient = nullptr;
}

void lang_new(const char* libPath) {
    gLangClient = new LangClient(libPath);
    stop = false;
    lang_thread = new std::thread([&]() {
        while (!stop) {
            lang_tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
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

void lang_tick() {
    if (gLangClient) {
        gLangClient->tick();
    }
}

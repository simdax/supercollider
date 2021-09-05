#include "include.h"
#include <SC_TerminalClient.h>

// SCLANG_DLLEXPORT_C void lang_new(const char* libPath) { 
//     gLangClient = createLanguageClient("test");
// }

SCLANG_DLLEXPORT_C void lang_msg(const char* msg) {
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

void LangClient::msg(const char* msg) {
    setCmdLine(msg);
    interpretPrintCmdLine();
    flush();
}

SCLANG_DLLEXPORT_C void lang_new(const char* libPath) { gLangClient = new LangClient(libPath); }

SCLANG_DLLEXPORT_C void lang_tick() {
    if (gLangClient) {
        gLangClient->tick();
    }
}

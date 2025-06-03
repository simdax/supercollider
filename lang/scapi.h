#include "SC_LanguageClient.h"

extern "C" __declspec(dllexport) SC_LanguageClient* StartClient(char* pluginsPath);
extern "C" __declspec(dllexport) void KillClient(SC_LanguageClient* Client);
extern "C" __declspec(dllexport) void PlayString(SC_LanguageClient* Client, const char* cmdline);
extern "C" __declspec(dllexport) void PlayFile(SC_LanguageClient* Client, const char* path);

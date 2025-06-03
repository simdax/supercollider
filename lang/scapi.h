#include "SC_LanguageClient.h"
#include "SC_WorldOptions.h"

extern "C" __declspec(dllexport) World* StartServer();
extern "C" __declspec(dllexport) SC_LanguageClient* CreateClient();
extern "C" __declspec(dllexport) void StartClient(SC_LanguageClient*, char* pluginsPath, bool daemon);
extern "C" __declspec(dllexport) void RunClient(SC_LanguageClient*);
extern "C" __declspec(dllexport) void PlayString(SC_LanguageClient* Client, const char* cmdline);
extern "C" __declspec(dllexport) void PlayFile(SC_LanguageClient* Client, const char* path);
extern "C" __declspec(dllexport) void SetLogFD(const char* path);

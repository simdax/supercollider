#include <thread>
#include <condition_variable>
#include <iostream>
#include "scapi.h"

int main() {
    static auto* Lang = CreateClient();

    std::thread bob([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        PlayString(Lang, "().play;");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        PlayString(Lang, "().play;");
    });

    std::cout << "IO" << std::endl;
    auto* World = StartServer();
    StartClient(Lang, "C:/Users/s.cornaz/SCBundle/SuperCollider/SCClassLibrary", false);
    std::cout << "Bye" << std::endl;
}

//
// int main() {
//    static std::condition_variable cv;
//    static std::mutex cv_m;
//    static int i = 0;
//    static auto* Lang = createLanguageClient("test");
//
//    std::thread server([]() {
//        std::cout << "IO" << std::endl;
//        volatile static auto* World = StartServer();
//        {
//            std::lock_guard lock(cv_m);
//            i = 1;
//        }
//        cv.notify_all();
//        StartClient(Lang, "C:/Users/s.cornaz/SCBundle/SuperCollider/SCClassLibrary", true);
//    });
//    {
//        std::unique_lock lock(cv_m);
//        cv.wait(lock, []() { return i == 1; });
//    }
//    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
//    std::cout << "init tree" << std::endl;
//    PlayString(Lang, "s.initTree; ().play;");
//    std::this_thread::sleep_for(std::chrono::seconds(3));
//    std::cout << "Bye" << std::endl;
//}

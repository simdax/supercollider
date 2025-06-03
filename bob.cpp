#include <thread>
#include <condition_variable>
#include <iostream>
#include "scapi.h"

int main() {
    std::cout << "Start Client" << std::endl;
    auto* Lang = StartClient("C:/Users/s.cornaz/SCBundle/SuperCollider/SCClassLibrary");
    std::this_thread::sleep_for(std::chrono::milliseconds(2001));
    std::cout << "Start Input" << std::endl;
    PlayString(Lang, "().play;");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    KillClient(Lang);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Bye" << std::endl;
}

#include "include.h"

int print(const char* format, va_list ap) { 
    std::cout << format << std::endl; 
    flush(std::cout);
    return 0;
}


int main(int argc, char** argv) {
    SetPrintFunc(&print);
    go();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    lang_msg("(note:4).play");
    lang_msg("(note:0).play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    all_stop();
    go();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    lang_msg("(note:4).play");
    lang_msg("(note:0).play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    all_stop();
    return 0;
}

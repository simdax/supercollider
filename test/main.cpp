#include "include.h"

int print(const char* format, va_list ap) { 
    std::cout << format << std::endl; 
    flush(std::cout);
    return 0;
}

void inline test(const char *msg)
{
    go();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    lang_msg(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    all_stop();
}

int main(int argc, char** argv) {
    SetPrintFunc(&print);
    test("(note:4).play;(note:0).play");
    test(
        "Pbind("
            "\\note, Pseq([0,2,3], inf),"
            "\\tempo, 5,"
        ").play;"
    );
    test("(note:4).play;(note:0).play");
    return 0;
}

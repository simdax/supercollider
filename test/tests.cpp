#include "include.h"

int print(const char* format, va_list ap) {
    std::cout << format << std::endl;
    flush(std::cout);
    return 0;
}

void go() {
    auto port = 57111;
    auto host = "127.0.0.1";

    server_new(port, host, "C:/Users/scornaz/git/supercollider/build/server/plugins/Debug");
    lang_new(
        "C:/Users/scornaz/git/supercollider/test/config.yaml",
        "C:/Users/scornaz/git/supercollider/test/test.scd"
    );
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void all_stop() {
    server_stop();
    lang_stop();
}

void inline test(const char* msg) {
    go();
    lang_msg(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    all_stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main(int argc, char** argv) {
    SetPrintFunc(&print);
    go();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    lang_msg("SoundFile(\"Music.wav\").play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    lang_msg("CmdPeriod.run");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    lang_msg("SoundFile(\"Music.wav\").play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    all_stop();
    return 0;
}

int main2(int argc, char** argv) {
    SetPrintFunc(&print);
    test("(note:4).play;(note:0).play");
    test("Pbind("
         "\\note, Pseq([0,2,3], inf),"
         "\\tempo, 5,"
         ").play;");
    test("(note:4).play;(note:0).play");
    return 0;
}
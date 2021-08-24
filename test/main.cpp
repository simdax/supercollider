#include "include.h"

int main(int argc, char** argv) {
    go();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    lang_msg("(note:4).play");
    lang_msg("(note:0).play");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    all_stop();
    return 0;
}

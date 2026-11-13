#include "display.h"
#include "test.h"

static void test_lit_while_the_link_is_held_or_offered(void) {
    assert(display_lit(ModeIdle));
    assert(display_lit(ModeVoice));
    assert(display_lit(ModeNoLink));
    assert(display_lit(ModeVoiceCut));
}

static void test_dark_once_the_offer_stops(void) {
    assert(!display_lit(ModeStopped));
}

int main(void) {
    test_run(test_lit_while_the_link_is_held_or_offered);
    test_run(test_dark_once_the_offer_stops);
    return 0;
}

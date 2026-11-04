#include "feedback.h"
#include "test.h"

static void test_sent_actions_vibrate(void) {
    assert(feedback_vibrates(ActionEnter));
    assert(feedback_vibrates(ActionArrowUp));
    assert(feedback_vibrates(ActionArrowDown));
    assert(feedback_vibrates(ActionEsc));
    assert(feedback_vibrates(ActionEffortHigh));
    assert(feedback_vibrates(ActionEffortMax));
    assert(feedback_vibrates(ActionClear));
    assert(feedback_vibrates(ActionVoiceStart));
}

static void test_the_rest_stay_silent(void) {
    assert(!feedback_vibrates(ActionNone));
    assert(!feedback_vibrates(ActionVoiceStop));
    assert(!feedback_vibrates(ActionExit));
}

static void test_commands_flash(void) {
    assert(feedback_flashes(ActionEffortHigh));
    assert(feedback_flashes(ActionEffortMax));
    assert(feedback_flashes(ActionClear));
    assert(feedback_flashes(ActionEsc));
}

static void test_the_rest_do_not_flash(void) {
    assert(!feedback_flashes(ActionNone));
    assert(!feedback_flashes(ActionEnter));
    assert(!feedback_flashes(ActionArrowUp));
    assert(!feedback_flashes(ActionArrowDown));
    assert(!feedback_flashes(ActionVoiceStart));
    assert(!feedback_flashes(ActionVoiceStop));
    assert(!feedback_flashes(ActionExit));
}

int main(void) {
    test_run(test_sent_actions_vibrate);
    test_run(test_the_rest_stay_silent);
    test_run(test_commands_flash);
    test_run(test_the_rest_do_not_flash);
    return 0;
}

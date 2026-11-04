#include "keymap.h"
#include "test.h"

static Action keymap_test_fresh(KeymapKey key, KeymapEvent event) {
    Keymap* keymap = keymap_alloc();
    Action action = keymap_handle(keymap, key, event);
    keymap_free(keymap);
    return action;
}

static void test_tap_acts(void) {
    assert(keymap_test_fresh(KeymapKeyUp, KeymapEventShort) == ActionArrowUp);
    assert(keymap_test_fresh(KeymapKeyDown, KeymapEventShort) == ActionArrowDown);
    assert(keymap_test_fresh(KeymapKeyOk, KeymapEventShort) == ActionEnter);
}

static void test_tap_on_left_or_back_does_nothing(void) {
    assert(keymap_test_fresh(KeymapKeyLeft, KeymapEventShort) == ActionNone);
    assert(keymap_test_fresh(KeymapKeyBack, KeymapEventShort) == ActionNone);
}

static void test_hold_acts(void) {
    assert(keymap_test_fresh(KeymapKeyUp, KeymapEventLong) == ActionEffortHigh);
    assert(keymap_test_fresh(KeymapKeyDown, KeymapEventLong) == ActionEffortMax);
    assert(keymap_test_fresh(KeymapKeyLeft, KeymapEventLong) == ActionClear);
    assert(keymap_test_fresh(KeymapKeyOk, KeymapEventLong) == ActionVoiceStart);
    assert(keymap_test_fresh(KeymapKeyBack, KeymapEventLong) == ActionExit);
}

static void test_right_acts_on_press_alone(void) {
    assert(keymap_test_fresh(KeymapKeyRight, KeymapEventPress) == ActionEsc);
    assert(keymap_test_fresh(KeymapKeyRight, KeymapEventShort) == ActionNone);
    assert(keymap_test_fresh(KeymapKeyRight, KeymapEventLong) == ActionNone);
}

static void test_press_acts_only_on_right(void) {
    for(KeymapKey key = KeymapKeyUp; key < KeymapKeyMAX; key++) {
        if(key != KeymapKeyRight) {
            assert(keymap_test_fresh(key, KeymapEventPress) == ActionNone);
        }
    }
}

static void test_repeat_never_acts(void) {
    for(KeymapKey key = KeymapKeyUp; key < KeymapKeyMAX; key++) {
        assert(keymap_test_fresh(key, KeymapEventRepeat) == ActionNone);
    }
}

static void test_release_without_voice_does_nothing(void) {
    for(KeymapKey key = KeymapKeyUp; key < KeymapKeyMAX; key++) {
        assert(keymap_test_fresh(key, KeymapEventRelease) == ActionNone);
    }
}

static void test_hold_ok_turns_voice_on(void) {
    Keymap* keymap = keymap_alloc();
    assert(!keymap_is_voice_on(keymap));
    keymap_handle(keymap, KeymapKeyOk, KeymapEventLong);
    assert(keymap_is_voice_on(keymap));
    keymap_free(keymap);
}

static void test_release_ok_stops_voice_once(void) {
    Keymap* keymap = keymap_alloc();
    keymap_handle(keymap, KeymapKeyOk, KeymapEventLong);
    assert(keymap_handle(keymap, KeymapKeyOk, KeymapEventRelease) == ActionVoiceStop);
    assert(!keymap_is_voice_on(keymap));
    assert(keymap_handle(keymap, KeymapKeyOk, KeymapEventRelease) == ActionNone);
    keymap_free(keymap);
}

static void test_tap_ok_leaves_voice_off(void) {
    Keymap* keymap = keymap_alloc();
    keymap_handle(keymap, KeymapKeyOk, KeymapEventPress);
    keymap_handle(keymap, KeymapKeyOk, KeymapEventShort);
    assert(!keymap_is_voice_on(keymap));
    assert(keymap_handle(keymap, KeymapKeyOk, KeymapEventRelease) == ActionNone);
    keymap_free(keymap);
}

static void test_voice_ignores_every_other_event(void) {
    Keymap* keymap = keymap_alloc();
    keymap_handle(keymap, KeymapKeyOk, KeymapEventLong);
    for(KeymapKey key = KeymapKeyUp; key < KeymapKeyMAX; key++) {
        for(KeymapEvent event = KeymapEventPress; event < KeymapEventMAX; event++) {
            if(key != KeymapKeyOk || event != KeymapEventRelease) {
                assert(keymap_handle(keymap, key, event) == ActionNone);
                assert(keymap_is_voice_on(keymap));
            }
        }
    }
    keymap_free(keymap);
}

static void test_buttons_act_again_after_voice(void) {
    Keymap* keymap = keymap_alloc();
    keymap_handle(keymap, KeymapKeyOk, KeymapEventLong);
    keymap_handle(keymap, KeymapKeyOk, KeymapEventRelease);
    assert(keymap_handle(keymap, KeymapKeyBack, KeymapEventLong) == ActionExit);
    keymap_free(keymap);
}

int main(void) {
    test_run(test_tap_acts);
    test_run(test_tap_on_left_or_back_does_nothing);
    test_run(test_hold_acts);
    test_run(test_right_acts_on_press_alone);
    test_run(test_press_acts_only_on_right);
    test_run(test_repeat_never_acts);
    test_run(test_release_without_voice_does_nothing);
    test_run(test_hold_ok_turns_voice_on);
    test_run(test_release_ok_stops_voice_once);
    test_run(test_tap_ok_leaves_voice_off);
    test_run(test_voice_ignores_every_other_event);
    test_run(test_buttons_act_again_after_voice);
    return 0;
}

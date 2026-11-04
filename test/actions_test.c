#include "actions.h"
#include "test.h"
#include "transport_fake.h"

static void
    assert_event(const Transport* transport, size_t index, TransportKey key, bool pressed) {
    TransportFakeEvent event = transport_fake_event(transport, index);
    assert(event.key == key);
    assert(event.pressed == pressed);
}

static void test_effort_high_sends_ctrl_x_then_ctrl_f(void) {
    Transport* transport = transport_fake_alloc();

    actions_perform(transport, ActionEffortHigh);

    assert(transport_fake_count(transport) == 4);
    assert_event(transport, 0, TransportKeyControlX, true);
    assert_event(transport, 1, TransportKeyControlX, false);
    assert_event(transport, 2, TransportKeyControlF, true);
    assert_event(transport, 3, TransportKeyControlF, false);
    transport_fake_free(transport);
}

static void test_effort_max_sends_ctrl_x_then_ctrl_q(void) {
    Transport* transport = transport_fake_alloc();

    actions_perform(transport, ActionEffortMax);

    assert(transport_fake_count(transport) == 4);
    assert_event(transport, 0, TransportKeyControlX, true);
    assert_event(transport, 1, TransportKeyControlX, false);
    assert_event(transport, 2, TransportKeyControlQ, true);
    assert_event(transport, 3, TransportKeyControlQ, false);
    transport_fake_free(transport);
}

static void test_every_other_action_sends_nothing(void) {
    static const Action others[] = {
        ActionNone,
        ActionEnter,
        ActionArrowUp,
        ActionArrowDown,
        ActionEsc,
        ActionClear,
        ActionVoiceStart,
        ActionVoiceStop,
        ActionExit,
    };
    Transport* transport = transport_fake_alloc();

    for(size_t i = 0; i < sizeof(others) / sizeof(others[0]); i++) {
        actions_perform(transport, others[i]);
    }

    assert(transport_fake_count(transport) == 0);
    transport_fake_free(transport);
}

int main(void) {
    test_run(test_effort_high_sends_ctrl_x_then_ctrl_f);
    test_run(test_effort_max_sends_ctrl_x_then_ctrl_q);
    test_run(test_every_other_action_sends_nothing);
    return 0;
}

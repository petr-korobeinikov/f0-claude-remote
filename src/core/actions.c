#include "actions.h"

static void actions_tap(Transport* transport, TransportKey key) {
    transport_press(transport, key);
    transport_release(transport, key);
}

/* README.md binds these chords on the Mac to `command:effort high` and `command:effort max`.
 * Typed text would come out garbled under a non-Latin layout, and its Enter would send the draft. */
void actions_perform(Transport* transport, Action action) {
    switch(action) {
    case ActionEffortHigh:
        actions_tap(transport, TransportKeyControlX);
        actions_tap(transport, TransportKeyControlF);
        break;
    case ActionEffortMax:
        actions_tap(transport, TransportKeyControlX);
        actions_tap(transport, TransportKeyControlQ);
        break;
    default:
        break;
    }
}

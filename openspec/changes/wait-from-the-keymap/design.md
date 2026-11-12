# Design

## Context

See `proposal.md`, Why.
The rule moves from platform-specific code into the core, `src/core/keymap.c`.
`app.c` keeps the timer alone, which exists only in the firmware's event loop.

| Fact | Where |
|---|---|
| Past `keymap_alloc`, the mode and the link change in `keymap_set_screen` alone | `src/core/keymap.c:40-58` |
| Nine calls change the screen, and six of them begin or end a wait | `keymap.c:86-152` |
| Every event ends in `app_settle`, which reads the mode and the link, then follows the wait | `src/platform/app.c:73-101` |
| A timer's start restarts it, and stopping a stopped one does nothing | `event_loop_timer.h:69-74`, `:84-90`, SDK 1.5.1 |

## Goals / Non-Goals

**Goals:** the spec's, with every board, key and second as before.

**Non-Goals:** `ActionUseUsb` and `ActionUseBluetooth`, by which the transport still switches;
the backlight's rule in `app_perform` (`app.c:51-53`), a finding of its own.

## Decisions

| File | Layer | Its one task | What changes |
|---|---|---|---|
| `src/core/keymap.h`, `.c` | Core | Maps a button event, a link change or the wait's end to an action, given the mode | `KeymapWait`, `keymap_wait`, the wait's `switch` |
| `test/keymap_test.c` | Test | Pins what the keymap answers | Eleven tests |
| `src/platform/app.c` | Platform | Runs the application's event loop | `app_follow_wait`; `wait_ms` goes |

`keymap.h` gains the answer, read after an event as the mode and the link are:

```c
typedef enum {
    KeymapWaitKeep,
    KeymapWaitStart,
    KeymapWaitStop,
} KeymapWait;

KeymapWait keymap_wait(const Keymap* keymap);
```

`Keymap` holds it as `wait`.
`keymap_alloc` sets `KeymapWaitStart`;
`keymap_handle`, `keymap_set_connected` and `keymap_time_out` each open with `keymap->wait = KeymapWaitKeep;`;
`keymap_set_screen` assigns `keymap_wait_entering(keymap, mode, link)` before it moves the mode and the link;
and `keymap_wait_ms` returns `keymap_mode_wait_ms(keymap->mode)`.

```c
static uint32_t keymap_mode_wait_ms(Mode mode) {
    switch(mode) {
    case ModeNoLink:
    case ModeVoiceCut:
        return keymap_link_wait_ms;
    case ModeIdle:
    case ModeVoice:
    case ModeStopped:
        break;
    }
    return 0;
}

static KeymapWait keymap_wait_entering(const Keymap* keymap, Mode mode, Link link) {
    bool waited = keymap_mode_wait_ms(keymap->mode) > 0;
    bool waits = keymap_mode_wait_ms(mode) > 0;

    if(waits && (!waited || link != keymap->link)) {
        return KeymapWaitStart;
    }
    if(waited && !waits) {
        return KeymapWaitStop;
    }
    return KeymapWaitKeep;
}
```

In `app.c`, `App.wait_ms`, its assignments and `app_perform`'s `if` on the switch actions go,
with both comments (`:35`, `:59-61`): nothing asks whether the timer runs any more.
`app_settle` and `app_alloc` call `app_follow_wait` as now:

```c
static void app_follow_wait(App* app) {
    switch(keymap_wait(app->keymap)) {
    case KeymapWaitStart:
        furi_event_loop_timer_start(
            app->wait_timer, furi_ms_to_ticks(keymap_wait_ms(app->keymap)));
        break;
    case KeymapWaitStop:
        furi_event_loop_timer_stop(app->wait_timer);
        break;
    case KeymapWaitKeep:
        break;
    }
}
```

The tests.
Each, the opening's aside, first asserts `keymap_wait` right after an event that answered otherwise,
so an answer left over fails it:

| Test | Events | Answer |
|---|---|---|
| `test_opening_starts_the_wait` | `keymap_alloc` | Start |
| `test_drop_starts_the_wait` | A link down in `ModeIdle`; in `ModeVoice` | Start |
| `test_starting_over_starts_the_wait` | A press in `ModeStopped` | Start |
| `test_each_switch_starts_the_wait_afresh` | Right's press, hold, release, press, hold | Keep, Start, Keep, then Start |
| `test_link_up_stops_the_wait` | A link up in `ModeNoLink`; in `ModeVoiceCut` | Stop |
| `test_wait_end_stops_the_wait` | `keymap_time_out` in `ModeNoLink` | Stop |
| `test_presses_with_no_link_keep_the_wait` | Every key and event in `ModeNoLink` but Right's hold | Keep each |
| `test_ok_released_after_cut_keeps_the_wait` | OK's release in `ModeVoiceCut` | Keep, in `ModeNoLink` |
| `test_repeated_link_down_keeps_the_wait` | A link down in `ModeNoLink`; in `ModeVoiceCut` | Keep |
| `test_events_with_a_link_keep_the_wait` | With a link: `keymap_time_out`, a link up, OK held, OK released | Keep each |
| `test_events_once_stopped_keep_the_wait` | Once stopped: a link up, a link down, `keymap_time_out` | Keep each |

The docs' two sentences become:
`docs/internals/architecture.md:63`, "The wait takes a timer in `app.c`, started and stopped as `keymap.c`'s `keymap_wait` answers after every event.";
`docs/internals/usb.md:115`, "`keymap.c` answers the switch with a start, and starting the timer restarts it".

- **Derived where the screen changes,**
  not written beside each of the nine calls that change it:
  one written wrong is a remote that never gives up.
- **Read after the event, and reset by each entry point,**
  not returned beside the action:
  83 asserts of `test/keymap_test.c` compare the `Action` those three return.
- **`app.c` turns the answer into the firmware's two calls,**
  not a `timer.h` the core commands, as `transport.h`:
  its three cases hold no rule, so a fake timer would pin nothing.
- **The wait's end answers a stop,**
  not nothing, though the timer has fired:
  one rule then covers both ends of a wait.

Test seams:

- `keymap.c`: the host suite, through `keymap.h`.
  Red is the undeclared `keymap_wait`, then, with the answer set and never reset, an assert of `KeymapWaitKeep`.
- `app.c` has no host test:
  `mise run build` stops at a `KeymapWait` its `switch` leaves out,
  the spec's `git grep` finds a count kept there,
  and task 4.1 reads a start, a press and the end off the device.

## Risks / Trade-offs

- An entry point added later skips the reset → its events repeat the last answer; each of the three resets fails a test of its own.
- The hold's fresh count is not seen on the device again → it shares the start's case with task 4.1's drop, and showed at 60.43 s and 60.49 s before the move (`usb-transport`'s tasks 4.2, 4.3).

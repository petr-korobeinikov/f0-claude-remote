# Design

## Context

See `proposal.md`, Why.
The core keeps deciding the link, in `src/core/keymap.c`.
Following it lands in platform-specific code, one line of `app.c` and one function of `transport_hid.c`:
closing one link and opening the other reaches the firmware's two stacks,
and only the transport knows which link is open, where a copy for the core to compare would hold the link twice.

| Fact | Where |
|---|---|
| Past `keymap_alloc`, the link moves in one call, Right's hold on `ModeNoLink` | `src/core/keymap.c:131-137` |
| Every event ends in `app_settle`, which hands the screen the mode and the link, then performs the action, if any | `src/platform/app.c:66-96` |

By both rows, no event both moves the link and answers an action, once the hold answers none,
so a transport set right after the screen calls the firmware in today's order.

## Goals / Non-Goals

**Goals:** the spec's, with every board, key, buzz and second as before, and every call into the firmware in its order.

**Non-Goals:** `ActionGiveUp` and `ActionStartOver` (`proposal.md`, Impact);
`transport_link.h` and the two links' files, which stay line for line.

## Decisions

Each source file keeps its task, as `docs/internals/architecture.md:23-42` gives it.

| File | Layer | What changes |
|---|---|---|
| `src/core/action.h` | Core | `ActionUseUsb` and `ActionUseBluetooth` go |
| `src/core/keymap.c` | Core | The hold answers no action; `keymap_use_actions` goes |
| `src/core/actions.c`, `feedback.c`, `deed.c`, `src/ui/view_idle.c` | Core, UI | Their five switches lose the two cases |
| `src/core/transport.h` | Core | `transport_use` and the include of `link.h` go |
| `src/platform/transport_hid.h`, `.c` | Platform | `transport_use` becomes `transport_hid_set_link` |
| `src/platform/app.c` | Platform | `app_settle` gains one line |
| `test/transport_fake.h`, `.c` | Test | `transport_use`, `transport_fake_link` and the link it reads go |

In `keymap.c`, the hold's case:

```c
    case ModeNoLink:
        if(key == KeymapKeyRight && event == KeymapEventLong) {
            keymap_set_screen(keymap, ModeNoLink, keymap_other_links[keymap->link]);
        }
        break;
```

In `transport_hid.c`, after `transport_hid_free`, and declared after it in `transport_hid.h`;
until task 3.2, `transport_use` stays and calls it:

```c
/* A Connected that one link left queued would read as the other's. */
void transport_hid_set_link(Transport* transport, Link link) {
    if(transport_hid_links[link] == transport->link) {
        return;
    }
    transport_hid_close(transport);
    furi_message_queue_reset(transport->link_queue);
    transport_hid_open(transport, link);
}
```

In `app.c`:

```c
/* The board goes first: a switch then holds this loop at least 700 ms. */
static void app_settle(App* app, Action action) {
    screen_set_mode(app->screen, keymap_mode(app->keymap));
    screen_set_link(app->screen, keymap_link(app->keymap));
    transport_hid_set_link(app->transport, keymap_link(app->keymap));
    if(action != ActionNone) {
        app_perform(app, action);
    }
    backlight_set(app->backlight, display_lit(keymap_mode(app->keymap)));
    app_follow_wait(app);
}
```

The tests.
`test/actions_test.c` loses `test_use_usb_switches_sending_nothing` and `test_use_bluetooth_switches_back_sending_nothing`,
`test/deed_test.c` two asserts and `test/feedback_test.c` four.
In `test/keymap_test.c`, `ActionUseUsb` and `ActionUseBluetooth` become `ActionNone`,
and every test that told by an answer whether the link moved asserts the link, keeping its other asserts:

| Test | `keymap_link`, asserted |
|---|---|
| `test_no_link_acts_only_on_held_back_or_right` | After each event of the sweep, against the link read before it: moved on Right's hold alone |
| `test_right_held_on_no_link_takes_usb_once` | After the hold and after the second long press: `LinkUsb` |
| `test_right_held_again_switches_back_and_forth` | As now (`:232`, `:237`, `:242`) |
| `test_right_pressed_on_usb_stopped_returns_nothing`, `test_right_pressed_on_usb_idle_returns_nothing`, `test_right_tapped_on_usb_no_link_does_nothing` | After the last event: `LinkUsb` |
| `test_right_held_elsewhere_on_usb_returns_nothing` | After each of its three holds: `LinkUsb` |
| `test_right_tapped_on_no_link_does_nothing`, `test_right_long_with_no_press_does_nothing`, `test_right_pressed_on_stopped_takes_no_usb`, `test_right_pressed_on_idle_takes_no_usb` | After the last event: `LinkBluetooth` |
| `test_right_held_elsewhere_takes_no_usb` | After each keymap's hold: `LinkBluetooth` |

The sweep expects `ActionExit` of Back's hold and `ActionNone` of every other event, Right's hold included.

The docs.
`docs/internals/architecture.md:54` becomes
"Before step 5, `app.c` hands `screen.c` the mode and the link `keymap.c` holds,
then `transport_hid.c` that link, which switches to it unless it is the one in use.";
`docs/development/host-tests.md:30-32` loses its last clause, the fake's reader of the link.

- **The transport is set before the action,**
  not after it, beside the backlight:
  an action's keys then go over the link the keymap holds.
- **The guard compares the row asked for with the row in use,**
  not a `Link` kept beside the row:
  `Transport` then says once which link is open.
- **`transport_hid_set_link`, as `screen_set_link`,**
  not `transport_hid_use`:
  it is asked after every event for a state, where `use` was a command.
- **The sweep keeps its name, `button-actions`' requirement, and its one loop,**
  not a second test for the link:
  Right's hold still acts there, on the link.

Test seams:

- `keymap.c`: the host suite, through `keymap.h`.
  Red is the sweep's assert on the hold's answer, green the hold answering none.
  The asserts that the link stayed are green before and after:
  `keymap_set_screen` not forgetting the presses fails `test_right_held_on_no_link_takes_usb_once` at its last assert.
- `actions.c`, `feedback.c`, `deed.c`: their suites.
  Red is the build at the two names, once `action.h` loses them.
- `transport_hid.c` and `app.c` have no host test:
  `mise run build` types the call, the spec's `The platform follows` finds its line,
  and task 5.1 switches both ways with a key over each link.

## Risks / Trade-offs

- The guard written backwards reopens the link on every event → task 5.1 never holds `D_Main`.
- No suite sees the transport switch any more → the keymap's suite pins the link, the spec's `git grep` the line that hands it over, and task 5.1 the switch.

# Design

## Context

See `proposal.md` for why.
The board is `D_stopped` of the wireframe, version `1793998126-5e31`;
its table is the draw list; `I_back` is in `images/`.
A firmware bump can break the second list:

- **Documented** (SDK headers):
  `furi_hal_bt_start_advertising`, `furi_hal_bt_stop_advertising` (`furi_hal_bt.h:132-138`);
  the backlight's always-on lock, its unlock and its force off (`notification_messages.h:84-94`);
  a timer stops idempotently and reads not running once expired (`event_loop_timer.h:84-117`).
- **Firmware 1.5.1 sources alone:**
  advertising every 80–100 ms for 60 s after each start and each drop, then every 1–2.5 s
  (`gap.c:15`, `:143`, `:430-435`, `:474`);
  stopping cuts a connection in progress (`gap.c:481-483`),
  takes GAP's `state_mutex`, which the BLE thread holds through `Verify code` (`gap.c:127`, `:260-266`),
  and waits until the radio is idle (`furi_hal_bt.c:261-267`);
  stopping reports `BtStatusOff` (`bt.c:312`);
  the lock is a counter, an extra unlock logging `Incorrect BacklightEnforce use` (`notification_app.c:229-247`);
  every press lights the backlight for the Backlight Time by itself (`notification_app.c:507-511`, `:212-226`);
  the rest of a press is dropped once the view port it began on is gone (`gui.c:310-333`, `:399-401`).
- **Seen:** the Mac reconnects in 0.87 s; the press that lights the screen reaches the app (`app-keymap`'s task 3.7).

## Goals / Non-Goals

**Goals:** the specs; rules in the core.

**Non-Goals:** a setting for the window; a countdown on `D_disconnected`; USB; the vibration option;
the Mac's sleep, which stays unchecked.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/core/mode.h`, `action.h` | Core | Name the modes, the actions | Compiled by the suite | The enums |
| `src/core/keymap.h`, `.c` | Core | Maps an event to an action, given the mode | `test/keymap_test.c` | `keymap_time_out`, `keymap_wait_ms` |
| `src/core/actions.c` | Core | Turns an action into calls on the transport | `test/actions_test.c` | `transport.h` |
| `src/core/feedback.c` | Core | Says how an action confirms itself | `test/feedback_test.c` | `Action` |
| `src/core/transport.h` | Core | Names what the core needs of a link | The fake | `transport_offer`, `transport_withdraw` |
| `test/transport_fake.h`, `.c` | Test | Records what the core asked of the link | The suites | `transport_fake_offered` |
| `src/platform/transport_bluetooth.c` | Platform | Owns the Bluetooth link for the app's life | Frames, the Mac's settings | `furi_hal_bt` |
| `src/platform/backlight.h`, `.c` | Platform | Holds the display lit while asked to | The owner's eye | The notification service |
| `src/platform/app.c` | Platform | Runs the application's event loop | On the device | The headers above |
| `src/ui/view.c` | UI | Draws a mode's board | Frames against the boards | `Canvas*` |

Advertising, the backlight's lock and event-loop timers exist only in the firmware.

```c
Action keymap_time_out(Keymap* keymap);
uint32_t keymap_wait_ms(const Keymap* keymap);
void transport_offer(Transport* transport);
void transport_withdraw(Transport* transport);
bool transport_fake_offered(const Transport* transport);
Backlight* backlight_alloc(void);
void backlight_free(Backlight* backlight);
void backlight_set(Backlight* backlight, bool lit);
```

`keymap`'s moves, as before but for these; other events in `ModeStopped` do nothing:

| Mode | A press | Back's long | Link reports | The wait ends | `keymap_wait_ms` |
|---|---|---|---|---|---|
| `ModeIdle`, `ModeVoice` | As before | As before | As before | Nothing | 0 |
| `ModeNoLink`, `ModeVoiceCut` | Nothing | Exit | As before | `ActionGiveUp`, `ModeStopped` | 60 000 |
| `ModeStopped` | Back's: `ActionExit`; any other: `ActionStartOver`, `ModeNoLink` | Exit | Stay | Nothing | 0 |

- **`keymap` owns the 60 s, as `feedback` owns a flash's length,**
  not a constant in `app.c`:
  the length is a rule, pinned on the host.
- **`app.c` runs one timer that follows `keymap_wait_ms` after every event:
  started when it turns nonzero with the timer idle, stopped when it reads 0,**
  not a restart on each link-down report, since stopping reports one itself (`bt.c:312`);
  its expiry performs `keymap_time_out`'s answer.
- **Both actions reach the link through `actions` and `transport.h`,**
  not calls from `app.c` into `transport_bluetooth`:
  the fake then proves neither sends a key.
- **`transport_withdraw` stops advertising and `transport_offer` starts it,**
  not `bt_disconnect`, the same stop through the Bt thread (`bt.c:436-439`).
- **`backlight.c` takes the lock at alloc and on `ActionStartOver`,
  gives it on `ActionGiveUp` and at free while held,
  and sends `sequence_display_backlight_off` after giving it,**
  not the bare pair `app.c` holds today:
  the lock is a counter, an app leaving `D_stopped` holds none,
  and a press's own Backlight Time would otherwise outlive the unlock.
- **`feedback` lists both actions as silent and flashless,**
  not a branch in `app.c`.
- **`view.c` draws `D_stopped` beside the no-link boards,**
  not in a file of its own:
  it shares their status row.

## Risks / Trade-offs

- The wait ends during `Verify code` → the stop waits behind the dialog, then cuts the link just paired;
  60 s leave 30 for reaching Connect, and a press on `D_stopped` brings the link back.
- The Mac reconnects at the wait's last moment → the stop cuts it; one press returns it.
- A `Connected` read after the stop is stale → `ModeStopped` ignores link reports.
- The stop holds the event loop until the radio is idle → milliseconds, or the dialog's length above.
- A lock taken twice keeps the desktop lit → the flag; `Leaving after starting over` checks it.
- `D_stopped` is seen only unlit, as the press that lights it starts over → drawn for ambient light.

# Design

## Context

See `proposal.md` for why.
The boards are `D_disconnected` and `D_disc_voice` of the wireframe,
version `1793794331-d8ca`.
A firmware bump can break the second list:

- **Documented** (SDK headers, docs.flipper.net's controllers page, manifest docs):
  a profile start restarts the second core, NULL on failure;
  `mac_xor`, the template's fields;
  a loop's thread;
  connecting from the computer; `fap_libs`.
- **Firmware 1.5.1 sources alone:**
  start and exit order (`hid.c:200-239`);
  address, and `adv_name`'s AD type byte (`hid_profile.c:399`, `gap.c:358`);
  advertising again after a drop (`gap.c:143`), re-reported every 60 s (`gap.c:472`);
  `Connected` only on pairing complete (`gap.c:280`);
  status on the Bt thread (`bt.c:558`), none on setting the callback (`bt_api.c:45`);
  the dialog (`bt.c:106`);
  advertising only with the setting on (`bt.c:411`);
  the signal callback gone with the loop (`event_loop.c:261`).

## Goals / Non-Goals

**Goals:** a link status, its boards, a launch that still replaces a running copy.

**Non-Goals:** keystrokes; USB; the vibration option; any reconnect code: the firmware advertises again.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/core/mode.h` | Core | Names the remote's modes | Compiled by the suite | The enum |
| `src/core/keymap.h`, `.c` | Core | Maps a button event to an action, given the mode | `test/keymap_test.c` | Its own key and event types, a `bool` link |
| `src/ui/view.h`, `.c` | UI | Draws a mode's board | Frames against the boards | `Canvas*` |
| `src/ui/view_idle.h`, `.c` | UI | Draws the idle board's buttons | Frames against the boards | `Canvas*` |
| `src/platform/screen.h`, `.c` | Platform | Keeps what the display shows | Frames, on the device | `view_draw` |
| `src/platform/transport_bluetooth.h`, `.c` | Platform | Owns the Bluetooth link for the app's life | The Mac's settings, frames | `link_queue` |
| `src/platform/app.c` | Platform | Runs the application's event loop | Buttons and link, on the device | The headers above |

`Bt`, the HID profile and thread signals exist only in the firmware.

```c
Mode keymap_mode(const Keymap* keymap);
void keymap_set_connected(Keymap* keymap, bool connected);
void view_draw(Canvas* canvas, Mode mode, Action flash);
void view_idle_draw(Canvas* canvas, Action flash);
Screen* screen_alloc(FuriEventLoop* event_loop, FuriMessageQueue* input_queue, Mode mode);
void screen_set_mode(Screen* screen, Mode mode);
TransportBluetooth* transport_bluetooth_alloc(FuriMessageQueue* link_queue);
```

`keymap` moves by this table; a fresh one has no link:

| Mode | A button | OK released | Link up | Link down |
|---|---|---|---|---|
| `ModeIdle` | The action table | Nothing | Stays | `ModeNoLink` |
| `ModeVoice` | Nothing | `ActionVoiceStop`, `ModeIdle` | Stays | `ModeVoiceCut` |
| `ModeNoLink` | Exit alone | Nothing | `ModeIdle` | Stays |
| `ModeVoiceCut` | Exit alone | Nothing, `ModeNoLink` | `ModeIdle` | Stays |

Tests pin each move, every key and event with no link, and a repeated status.

`transport_bluetooth` follows the stock order:

```text
alloc  bt_disconnect, delay 200 ms, keys path APP_DATA_PATH(".bt_hid.keys"),
       bt_profile_start, furi_hal_bt_start_advertising, status callback
free   callback NULL, bt_disconnect, delay 200 ms, default keys path,
       bt_profile_restore_default
```

- **A queue of `bool`, true for `BtStatusConnected`, handed in as `screen_alloc` takes its own,**
  not a callback type in the header:
  status runs on the Bt thread, and `screen_input_callback` crosses the same way.
- **A copy of `ble_profile_hid` whose `get_gap_config` writes `Claude Remote` over `adv_name`,**
  not `device_name_prefix`, capped under 8 characters:
  `start` still sets `ble_profile_hid` as its config (`hid_profile.c:145`).
- **`.mac_xor = 0xC1A0`,**
  not 0, the stock Remote's address (`hid.c:217` passes no params), nor `0x0002`, BadUSB's.
- **`furi_check` on both profile calls,**
  not `No link` forever:
  as the stock app (`hid.c:219,239`), since NULL means no usable radio stack.
- **`screen_set_mode` after every button event,**
  not only on an action:
  OK's release from `ModeVoiceCut` is `ActionNone`.
- **`app_free` frees the screen before the radio:**
  the menu returns at once.
- **Once the loop ends, `app_run` sets a signal callback that takes `FuriSignalExit` and touches nothing, until `app_free` clears it,**
  not ours for the loop's whole life, when the default serves:
  `runfap.py:74-88` exits 4 on an unconsumed signal's `has to be closed manually` (`loader_cli.c:101`).
- **`view.c` keeps the status row and the voice and no-link boards, `view_idle.c` the ring, its helpers and the flash,**
  not a file per board:
  the no-link boards differ by a hint.

The no-link boards, baselines as y:

```text
icon  bluetooth_off                 2  1
str   "No link"                    15  9  FontPrimary
line  0,12 to 127,12
str   "Reconnecting..."            27 37  FontPrimary
str   "Keys are not sent"          28 49  FontSecondary  D_disconnected
str   "Voice stopped: release OK"  11 49  FontSecondary  D_disc_voice
```

`bluetooth_off`, 10×9, 1-bit PNG:

```text
...#......
...##.....
.#.#.#....
..###.....
...#......
..###.#.#.
.#.#.#.#..
...##.#.#.
...#......
```

## Risks / Trade-offs

- The Apps browser holds `DialogsSrv` while open: launched over it, pairing shows no dialog, and `BleEventWorker`, waiting with `state_mutex` held, hangs closing → `README.md` says launch from the desktop.
- A press begun with no link fires after it returns → the buzz still means sent.
- An unrequested drop goes unchecked → `Opened again` and `Disconnected from the Mac` cover its halves.
- A full `link_queue` drops a status → 8 slots.
- GDB halting the app drops the link → `device-debug`'s scenarios not rerun, the owner's call.
- The link back inside a flash shows its rest → under 400 ms.
- Keyboard Setup Assistant asks for a layout → task 4.2 notes it for `effort-actions`.

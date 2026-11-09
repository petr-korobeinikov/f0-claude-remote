# Design

## Context

See `proposal.md` for why.
The boards are the wireframe's `D_disconnected` and `D_usb_*`, at the version task 1.1 records;
their tables are the draw list.
`I_usb` 7×9 and `I_usb_off` 10×9 fill the Bluetooth marks' boxes, so no text moves.
A firmware bump can break the second list:

- **Documented:**
  switching the USB mode, false when locked (`furi_hal_usb.h:49-51`);
  the keyboard's config, state, callback, press and release (`furi_hal_usb_hid.h:173`, `:198-223`);
  a timer's start restarts it (`event_loop_timer.h:69-74`);
  a new USB accessory asks first on an Apple silicon laptop ([Apple](https://support.apple.com/en-us/102282)).
- **Firmware 1.5.1 sources alone:**
  the stock order, mode read, unlock, switch, restore on leaving (`hid.c:177-187`);
  an RPC session locks the switch (`rpc_cli.c:49`);
  `usb_hid` has one interface, so the console goes (`furi_hal_usb_hid.c:156`);
  a switch holds its caller 500 ms (`furi_hal_usb.c:15`, `:354`) and keeps the config for a restart (`:374`);
  no config leaves no name, on IDs 046D:C529 (`furi_hal_usb_hid.c:16-17`, `:398-402`);
  the callback runs in the USB interrupt (`furi_hal_interrupt.c:343`, `furi_hal_usb.c:320`),
  where a queue put takes timeout 0 (`message_queue.c:67-78`);
  setting it reports a link already up (`furi_hal_usb_hid.c:279-289`);
  a release clears the report with no link (`:303-311`, `:455`);
  a pending event on an emptied queue calls nothing (`event_loop.c:107`).
- **Unknown until the device says** (task 6.1):
  whether the Mac's alert and Keyboard Setup Assistant appear for these IDs, and the link's delay.

## Goals / Non-Goals

**Goals:** the specs; the rule in the core.

**Non-Goals:** a way back to Bluetooth; anything saved; USB offered on another board;
a second application or Flipper Apps Catalog listing; frames read on USB.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/core/link.h` | Core | Names the remote's links | The device build | The enum |
| `src/core/keymap.c`, `actions.c`, `feedback.c` | Core | As before | Their suites | `Action`, `transport.h` |
| `test/transport_fake.h`, `.c` | Test | Records what the core asked of the link | The suites | `transport_fake_on_usb` |
| `src/platform/transport_hid.h`, `.c` | Platform | Routes the core's keys to the link in use | The Mac's keys | Both links' headers |
| `src/platform/transport_bluetooth.h`, `.c` | Platform | Is the Mac's Bluetooth keyboard for its own life | Frames, the Mac's settings | `furi_hal_bt` |
| `src/platform/transport_usb.h`, `.c` | Platform | Is the Mac's USB keyboard for its own life | `system_profiler`, the owner | `furi_hal_usb_hid.h` |
| `src/platform/screen.h`, `.c`, `app.c` | Platform | As before | On the device | `screen_set_link` |
| `src/ui/view.h`, `.c` | UI | Draws a mode's board | Frames, the owner's eye | `Canvas*`, `Link` |

USB's mode and keyboard, the key codes and the queue exist only in the firmware;
`link.h` sits in the core because the UI reaches no further.

```c
typedef enum {
    LinkBluetooth,
    LinkUsb,
} Link;

void transport_use_usb(Transport* transport);
bool transport_fake_on_usb(const Transport* transport);
Transport* transport_hid_alloc(FuriMessageQueue* link_queue);
void transport_hid_free(Transport* transport);
TransportUsb* transport_usb_alloc(FuriMessageQueue* link_queue);
void transport_usb_free(TransportUsb* usb);
void transport_usb_press(TransportUsb* usb, uint16_t key);
void transport_usb_offer(TransportUsb* usb);
void screen_set_link(Screen* screen, Link link);
void view_draw(Canvas* canvas, Mode mode, Link link, Action flash);
```

`_release` follows `_press`; `transport_bluetooth.h` takes the same names, and `_withdraw`.

Tests: in `ModeNoLink`, Right's press then long answers `ActionUseUsb`, once, staying there;
its tap answers nothing;
a long whose press came in `ModeStopped` or `ModeIdle` answers nothing;
so does Right's long in every other mode.

- **`keymap` notes at each press of Right whether it came in `ModeNoLink`,**
  not a table entry, which would act on `D_Main`.
- **`transport_hid.c` routes by `if` and holds the key table,**
  not a table of function pointers, as BadUSB's `bad_usb_hid.c`:
  two links and four calls read top to bottom, and both take the same codes.
- **`transport_use_usb` frees Bluetooth, empties the link queue, then allocates USB,**
  not the switch alone:
  a Bluetooth `Connected` left queued would read as USB's.
- **`transport_usb_alloc` follows the stock order, then sets the callback,
  with a static config named `Claude Remote` on the default IDs,**
  not `NULL`, nor IDs of its own that nobody assigned;
  the unlock keeps an open RPC session from failing the `furi_check`.
- **`transport_usb_offer` puts the current state on the queue,**
  not nothing:
  `ModeStopped` ignored the report that came, and none follows until the cable moves.
- **`app_perform`, on `ActionUseUsb`, hands the screen `LinkUsb` and zeroes `wait_ms` before `actions_perform`,**
  not after:
  the switch holds the loop at least 700 ms, and `app_follow_wait` then restarts the timer.
- **`Link` reaches `view` as one more `_Atomic` word in `screen`,**
  not `ModeUsb*`, which would double `keymap`'s tables, nor `#ifdef`: there is one build.
- **`view.c` picks the mark and the line by `Link`, and draws the plate for `ModeNoLink` on Bluetooth,**
  not a file per link:
  they share their rows.

## Risks / Trade-offs

- The Mac's alert declined → the Mac ignores the keyboard while the status may read `Connected`; `guide/usb.md` names the alert.
- `Connected` follows bus activity, before the Mac configures the keyboard (`furi_hal_usb_hid.c:433`) → a press in that gap sends nothing.
- A wrong hold leaves Bluetooth → leaving and opening again returns it.
- USB's linked boards cannot be read as frames → `D_usb_disconnected` and `D_usb_stopped` are read in task 4.2, before USB lands; the rest differ by the mark.
- Keys go 2–4 ms apart (`furi_hal_usb_hid.c:458`) → BadUSB types at that pace; task 6.2 tries the chords.

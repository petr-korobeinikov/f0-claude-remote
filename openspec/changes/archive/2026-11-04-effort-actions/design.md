# Design

## Context

See `proposal.md` for why.
An update can break the second list:

- **Documented** (keybindings, commands, model-config and prompt-caching pages, SDK headers):
  `command:<name>` runs `/<name>`, in `Chat`;
  the file reloads by itself;
  a Ctrl shortcut under Cyrillic matches by an ASCII control code or a reported position;
  a level after `/effort` is saved, `max` session-only;
  `/effort` during a turn takes effect with the next message;
  no confirmation on the 5.5 family and Fable 5.1 with a subscription;
  the key functions and `KEY_MOD_LEFT_CTRL`.
- **Sources alone:**
  Claude Code 2.1.311: the binding's text after `command:` runs as typed, a space only warns in the debug log;
  a keybinding's command leaves the draft, and runs at once during a turn.
  Firmware 1.5.1: a full buffer retries 1000 times, 1 ms apart (`gatt.c:124`);
  the return is true on failure (`gatt.c:144`), against `hid_profile.h:26`;
  `start` sets `ble_profile_hid` as its config (`hid_profile.c:145`), so a copied template passes `kb_press`'s check (`hid_profile.c:190`).
- **Seen on the Mac:** the proposal's trial.
  It left `ctrl+x ctrl+q` in the owner's file.

## Goals / Non-Goals

**Goals:** the two levels, and the seam `remote-actions` adds its keys to.

**Non-Goals:** the slider; USB; `/clear`, Enter, arrows, Esc, voice; `fap_category`.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/core/transport.h` | Core | Names the keys the core presses | Compiled by the suite | `Transport`, `TransportKey` |
| `src/core/actions.h`, `.c` | Core | Sends an action's keys | `test/actions_test.c` | `transport.h` |
| `test/transport_fake.h`, `.c` | Test | Records the keys sent | Used by the suite | `transport.h` |
| `src/platform/transport_bluetooth.h`, `.c` | Platform | Is the Mac's Bluetooth keyboard for the app's life | The owner's Mac | `link_queue`, `transport.h` |
| `src/platform/app.c` | Platform | Runs the application's event loop | Buttons, on the device | The headers above |

`Bt`, the HID profile and its key codes exist only in the firmware.

```c
typedef enum {
    TransportKeyControlF,
    TransportKeyControlQ,
    TransportKeyControlX,
    TransportKeyMAX,
} TransportKey;

typedef struct Transport Transport;

void transport_press(Transport* transport, TransportKey key);
void transport_release(Transport* transport, TransportKey key);

void actions_perform(Transport* transport, Action action);

Transport* transport_bluetooth_alloc(FuriMessageQueue* link_queue);
void transport_bluetooth_free(Transport* transport);
```

The block, in `keybindings.json` of Claude Code's config directory:

```json
{
  "context": "Chat",
  "bindings": {
    "ctrl+x ctrl+f": "command:effort high",
    "ctrl+x ctrl+q": "command:effort max"
  }
}
```

Tests, over the fake: both levels' exact events in order,
and no event for every other `Action`, `ActionNone` through `ActionExit`.

- **`TransportKey` in the core, mapped to `KEY_MOD_LEFT_CTRL | HID_KEYBOARD_X` by a table in `transport_bluetooth.c`,**
  not raw HID numbers in the core:
  the names stay the firmware's, and `keymap_input.c` already maps this way.
- **`struct Transport` is defined by whichever `.c` is linked, replacing `TransportBluetooth`,**
  not a table of function pointers:
  one implementation per build; the USB plan decides whether both must live at run time.
- **`transport.h` holds keystrokes, `transport_bluetooth.h` life and `link_queue`,**
  as `ble-transport` proposed: `actions` and `app` are two consumers.
- **`press` and `release`, not `tap`:**
  voice in `remote-actions` holds a key, and the order is a rule the core's test pins.
- **Both return `void`,**
  not `bool`:
  the firmware's result reads inverted, and a press and its release run together, so neither can strand a key.
- **No pause between reports,**
  not pacing on a timer:
  BadUSB sends press and release back to back (`ducky_script.c:149`), the firmware retries a full buffer, and four reports need no more.
- **`actions_perform` opens `app_perform`, before the buzz,**
  not after it:
  a buzz follows the send.
- **`transport_bluetooth_alloc` keeps `bt_profile_start`'s result,**
  as the stock app does (`hid.c:217`).
- **`.mise/tasks/test` links `test/*_fake.c` into every suite,**
  not a list per suite:
  the task stays one loop, and an unused fake costs nothing.
- **A comment in `actions.c` names the bindings and why chords,**
  not a bare table:
  no code can say what those keys mean on the Mac.

## Risks / Trade-offs

- A Claude Code update refuses the space → the chords do nothing, by `Keys not bound`; the slider becomes a plan.
- An editor validating against `$schema` marks the two lines → `README.md` says so.
- A cache-busting model or provider asks `Change effort level?` → the owner answers on the Mac, or with OK after `remote-actions`.
- A dialog is open or another window focused → a buzz with no change; the proposal accepts it, and no Enter goes out.
- A buffer that stays full holds the event loop 1 s per report → not expected at four reports.

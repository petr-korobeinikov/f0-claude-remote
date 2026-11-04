# Proposal

## Why

The remote still reaches nothing:
`src/platform/app.c` opens no radio,
the status row reads a fixed `Connected`,
and the Mac's Bluetooth list holds no Flipper,
so the brief's "pairing exists" does not hold here.
The first goal is sending an effort change, in two changes:
this one brings up the link,
and `effort-actions` types.
`remote-actions` and USB follow.
Wireframe version `1793794331-d8ca` holds the boards,
`D_disconnected` and `D_disc_voice`.

Decisions, already settled:

- **With no link only holding Back acts, and everything else stays silent,**
  not a buzz or a flash for a press that went nowhere:
  the owner's call, so no buzz means not sent.
- **The app gets its own Bluetooth name, address and key file,
  `Claude Remote`, paired once in the Mac's settings,**
  not the stock Remote's `Control <device name>`:
  no bond exists on this Mac to pick up, and the stock app keeps its own.
  without the device's name, the owner's choice:
  `adv_name` holds 16 characters.
- **No ring on the no-link boards, and voice does not resume by itself,**
  not a dimmed ring:
  the board's rule, since no command goes out.
- **`fap_category` stays `Examples`,**
  not `Bluetooth`, as `app-screen`'s proposal left to this change:
  the owner decides it apart, before publication.
- **Nothing is typed,**
  not a first keystroke:
  `No action reaches the Mac` stands until `effort-actions`.

Decisions proposed here, for the owner to strike:

- **No board for pairing,**
  not a third no-link board:
  the firmware draws its own `Verify code` dialog over the app,
  and `Reconnecting...` reads until pairing ends.
- **The app advertises even with Bluetooth off in Settings,**
  not a refusal:
  the stock app does (`hid.c:221`),
  though the documentation says Bluetooth must be on,
  and leaving restores the setting.
- **`keymap` holds the link, and one `Mode` replaces `keymap_is_voice_on`,**
  not a check in `app` nor a second flag:
  what a button does with no link is a rule,
  and one word gives the GUI thread no torn pair.
- **`src/platform/transport_bluetooth.h` now,
  `src/core/transport.h` with `effort-actions`,**
  not the core's header today:
  no core file consumes it yet,
  and `app` needs life and status where `actions` needs keystrokes,
  which the config calls two headers.
- **A drop does not buzz,**
  not a warning:
  a buzz means sent,
  and no dictation exists to lose before `remote-actions`,
  which weighs it again.
- **`view_idle` takes the idle board out of `view.c`,**
  not three boards in one file:
  `screen-redesign` left this seam to the third board.
- **The app takes the exit signal for as long as it closes,**
  not a retry in `.mise/tasks/launch`:
  `ufbt launch` repeats `loader close` every 200 ms
  and fails once the event loop has ended, which closing the radio now outlasts,
  and a retry prints that error on every launch.

## What Changes

- New `src/platform/transport_bluetooth.h`, `.c`;
  `application.fam` links `ble_profile`.
- New `src/core/mode.h`; `keymap` holds the link and answers with a `Mode`.
- New `src/ui/view_idle.h`, `.c`; `view.c` draws the no-link boards,
  and `images/` gains `bluetooth_off.png`.
- `screen` shows a `Mode`;
  `app` feeds the link to `keymap` and holds the exit signal.
- `test/keymap_test.c` pins the link's rules.
- `README.md` says how to pair.

## Capabilities

### New Capabilities

- `bluetooth-link`: the app's own Bluetooth identity, its pairing,
  its return after a drop, and the radio handed back on leaving.

### Modified Capabilities

- `app-screen`: the status follows the link, the no-link boards arrive,
  and `D_Main` needs the link up.
- `button-actions`: buttons act with the link up,
  only holding Back acts without it, and a lost link stops voice.

## Impact

No tool enters `mise.toml`;
`libble_profile.a` comes with the SDK pin and holds 2.3 KB of code and data,
against today's 9.9 KB package.
The device gains `/ext/apps_data/f0_claude_remote/.bt_hid.keys`,
and the Mac a new Bluetooth device,
perhaps with Keyboard Setup Assistant on first pairing.
While the app is open it holds the radio, so the phone app's connection waits.
Device checks of `D_Main` and `D_sent_*` now need a paired Mac.

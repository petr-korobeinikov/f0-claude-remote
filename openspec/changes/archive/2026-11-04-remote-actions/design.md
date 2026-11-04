# Design

## Context

See `proposal.md` for why.
An update can break the second list:

- **Documented** (keybindings, voice-dictation, commands, interactive-mode pages;
  SDK headers):
  `command:<name>` in `Chat`;
  Ctrl+L redrawing with input and conversation kept;
  a Ctrl shortcut under Cyrillic matching through its control code;
  `voice:pushToTalk` taking one key,
  a modifier combination starting on the first press,
  hold detection needing key repeat;
  a command sent during a turn queued, `Up` taking it back;
  `/clear` leaving the old conversation to `/resume`;
  the key codes, `kb_press` and `kb_release`.
- **Sources alone:**
  Claude Code 2.1.311:
  the voice key compared by character;
  a first press arming a 2000 ms fallback,
  then a 200 ms gap between repeats ending the recording;
  a keybinding's command leaving the draft;
  `clear` not immediate, so queued.
  Firmware 1.5.1:
  only a release clears `kb_report` (`hid_profile.c:188-240`);
  a failed report update only logs (`gatt.c:141`),
  `firmware.elf` holding no `Failed to update characteristic`;
  a Long whose Press went elsewhere discarded (`gui.c:283-329`).
- **Seen on the Mac:** the proposal's trial;
  `ctrl+x ctrl+q` under a non-Latin layout reaching Claude Code
  through iTerm2, tmux and herdr (`effort-actions`, 4.1),
  and starting a recording once `ctrl+q` held voice (task 1.3).
  Key repeat is at macOS's defaults.

## Goals / Non-Goals

**Goals:** every action's keys over Bluetooth,
and a drop that leaves no key down.

**Non-Goals:** USB; the lock; the vibration option; `fap_category`;
anything on the screen.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/core/transport.h` | Core | Names the keys the core presses | Compiled by the suite | `TransportKey` |
| `src/core/actions.c` | Core | Sends an action's keys | `test/actions_test.c` | `transport.h` |
| `src/core/keymap.h`, `.c` | Core | Maps a button event or a link change to an action | `test/keymap_test.c` | Its answer |
| `src/platform/transport_bluetooth.c` | Platform | Is the Mac's Bluetooth keyboard for the app's life | The owner's Mac | `transport.h` |
| `src/platform/app.c` | Platform | Runs the application's event loop | Buttons, on the device | The headers above |

The key codes exist only in the firmware's headers.
`view`, `screen` and `feedback` do not change.

```c
typedef enum {
    TransportKeyArrowDown,
    TransportKeyArrowUp,
    TransportKeyControlF,
    TransportKeyControlL,
    TransportKeyControlQ,
    TransportKeyControlX,
    TransportKeyEnter,
    TransportKeyEsc,
    TransportKeyMAX,
} TransportKey;

Action keymap_set_connected(Keymap* keymap, bool connected);
```

| Action | Keys | New code in `transport_bluetooth_keys` |
|---|---|---|
| Enter | `TransportKeyEnter` down, up | `HID_KEYBOARD_RETURN` |
| Arrow up | `TransportKeyArrowUp` down, up | `HID_KEYBOARD_UP_ARROW` |
| Arrow down | `TransportKeyArrowDown` down, up | `HID_KEYBOARD_DOWN_ARROW` |
| Esc | `TransportKeyEsc` down, up | `HID_KEYBOARD_ESCAPE` |
| Clear | `TransportKeyControlX` down, up, `TransportKeyControlL` down, up | `KEY_MOD_LEFT_CTRL \| HID_KEYBOARD_L` |
| Voice start | `TransportKeyControlQ` down | None |
| Voice stop | `TransportKeyControlQ` up | None |
| Effort high | Unchanged | None |
| Effort max | `TransportKeyControlX` down, up, down, up | None |
| Exit, none | Nothing | None |

`keymap_set_connected` moves the mode by its tables, as now,
and answers `ActionVoiceStop` for `ModeVoice` going to `ModeVoiceCut`,
`ActionNone` for the rest.

The block, in `keybindings.json` of Claude Code's config directory:

```json
{
  "context": "Chat",
  "bindings": {
    "ctrl+q": "voice:pushToTalk",
    "ctrl+x ctrl+f": "command:effort high",
    "ctrl+x ctrl+l": "command:clear",
    "ctrl+x ctrl+x": "command:effort max"
  }
}
```

Tests, over the fake: each row's exact events in order.
On `keymap`: the cut answers `ActionVoiceStop` once,
every other link change `ActionNone`.

- **A drop releases Ctrl+Q through `keymap`'s answer and `actions_perform`,**
  not `ble_profile_hid_kb_release_all`
  in `transport_bluetooth`'s status callback:
  the rule stays under a host test and on the loop's thread,
  and `ActionVoiceStop` is already silent.
  Left in `kb_report`,
  Ctrl+Q would go out with the next key after the link returns,
  and start dictation.
- **The release runs with no link,**
  not on the link's return:
  `kb_report` clears whatever the update answers,
  and the report it matters for comes later.
- **Effort max moves to `ctrl+x ctrl+x`, by the owner,**
  not `ctrl+x ctrl+q` or `ctrl+x ctrl+w`:
  with `ctrl+q` bound to voice the first starts a recording (task 1.3),
  and the second adds a key where `TransportKeyControlX` already passes.
- **The comment in `actions.c` names voice's binding, why not Option+K,
  and why effort max's chord does not end in Ctrl+Q,**
  not a bare `TransportKeyControlQ`:
  no code can say what the key means on the Mac,
  and the failure would be tried again.

## Risks / Trade-offs

- Another window is focused → Enter and Esc act there; the brief's critique leaves focus to the owner.
- A stray tap on OK sends a draft or approves a permission prompt → the proposal leaves it to the lock.
- Right twice in quick succession clears the draft or opens rewind → as on the keyboard; `Up` recalls the draft.
- OK tapped during `Voice: processing…` sends the interim text and drops the rest → `README.md` says to wait it out.
- Key repeat off on the Mac → a recording ends about 2.2 s into the hold; `README.md` names it.
- The Mac keeps repeating after a drop → unproven, since no real use holds a link down; dictation ends 200 ms after the last repeat.

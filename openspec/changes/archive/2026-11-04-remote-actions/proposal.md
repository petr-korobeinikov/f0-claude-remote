# Proposal

## Why

The remote still sends only the two effort levels:
`actions_perform` has no other case,
so Enter, the arrows, Esc, `/clear` and voice confirm themselves on the device
and reach nothing.
Voice is what the remote is for,
and the key decided for it fails here:
on 2026-11-04 the owner held Option+K under a non-Latin layout
and no dictation started.
Claude Code 2.1.311 compares that key with the character it receives,
and its documentation promises layout independence for Ctrl shortcuts alone.
This change finishes what the Mac receives over Bluetooth; USB follows.

Decisions, already settled:

- **Enter stays OK's tap and voice never sends,**
  not `autoSubmit`:
  the brief's call, since a transcript arrives with a delay.

Decisions proposed here, for the owner to strike:

- **`ctrl+q` holds voice, on the Mac's keyboard as well,**
  not Option+K, Option with a digit, or F18:
  a Ctrl letter arrives as a control code under any layout,
  as the documentation states,
  Q is the one single Ctrl letter free across Claude Code, tmux, herdr
  and the prompt's editing keys,
  and F keys are not among the documented special keys.
  `voice:pushToTalk` takes one key at a time,
  so Option+K stops dictating on the Mac.
  Task 1.2 tries it on the Mac's keyboard before any code.
- **`ctrl+x ctrl+l` runs `/clear`,**
  not typed text:
  the reasons are effort's,
  `command:clear` is documented with no argument to rely on,
  and Ctrl+L landing alone only redraws the screen.
- **`/clear` during a turn waits for the turn's end,**
  not an interrupt:
  the documentation says so of a command sent during a turn,
  and `Up` takes it back.
- **The unexplained flash gets no guard,**
  not a delay on Left nor a confirmation:
  on 2026-11-04 one frame read within 2 s of a launch
  differed from `D_Main` by 324 cells,
  the area of `/clear`'s flash, with no press;
  only the count was kept,
  and a diagnostic build then saw 0 input events over 6 launches and 167 s.
  A guard would answer an unknown cause:
  the firmware already drops a Long whose Press went elsewhere,
  and a stray `/clear` costs one `/resume`.
  From here on, a read that differs from its board keeps the whole frame.
- **The lock stays in the backlog,**
  though a stray press now acts for real:
  a tap on OK sends a draft or approves a permission prompt,
  and Right interrupts a turn.
  The owner put the mechanics first on 2026-11-03.
- **A drop during voice stays silent,**
  the weighing `ble-transport` left here,
  not a warning buzz:
  a buzz means sent,
  and the words said before the drop stay in the prompt.

## What Changes

- `src/core/transport.h` gains Enter, both arrows, Esc and Ctrl+L;
  `src/platform/transport_bluetooth.c` maps them.
- `src/core/actions.c` sends every action's keys.
- `keymap_set_connected` answers with the action a drop causes,
  and `app` performs it.
- `test/actions_test.c` and `test/keymap_test.c` pin the keys and the drop.
- `README.md`'s `## Effort` becomes `## Keys on the Mac`,
  and its block covers voice and `/clear`.

## Capabilities

### New Capabilities

- `prompt-control`: Enter, the arrows, Esc and `/clear` in Claude Code's prompt,
  from the device.
- `voice-control`: dictation into the prompt for as long as OK is held.

### Modified Capabilities

- `effort-control`: `Nothing else reaches the Mac` goes,
  since every action but exit now sends keys,
  and effort max's chord becomes Ctrl+X twice.
- `button-actions`: `Exit sends no key` holds what is left of that rule.

## Impact

No tool enters `mise.toml`, and `application.fam` does not change.
The owner's `keybindings.json` loses `alt+k` and `ctrl+x ctrl+q`
and gains `ctrl+q`, `ctrl+x ctrl+l` and `ctrl+x ctrl+x`.
A press now acts on whatever the Mac has focused.
Every device check that presses runs in a second Claude Code session:
`/clear`, Enter or Esc in the implementing one would wipe or interrupt it.

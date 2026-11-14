# Proposal

## Why

The remote sends neither the left arrow nor the right:
Left's tap does nothing, and Right spends its press on Esc (`src/core/keymap.c:21-22`).
Claude Code moves its `/effort` slider by those two keys, as `effortSlider:decreaseEffort` and `effortSlider:increaseEffort`,
and a dialog's tabs with them (code.claude.com/docs/en/keybindings, `Effort slider actions` and `Tabs actions`),
so from the device the effort is `high` or `max`, `effort-control`'s two holds,
and every other level takes the Mac's keyboard.
The owner asked for both,
and Claude Design drew their boards at canvas version `1794586781-3562`.

Decisions:

- **A tap on Left sends ←, a tap on Right →, and Esc moves to Right's hold** (the canvas's note `noteD`),
  not kept on Right's press, where a tap would send Esc and → both.
  The press was there for a slow one, which past the long press did nothing (`keymap.c:17`);
  a hold takes the slow press whole.
  It costs 300 ms before Esc (`input.c:13-14`, firmware 1.5.1),
  and a release before them sends →.
- **Right's hold on `No link` keeps switching the link** (`usb-link`, `bluetooth-link`),
  not moved off Right:
  with no link no key is sent (`keymap.c:142`), so the two holds never act on one screen.
  It ends what `usb-transport` picked Right for, the one button with no hold to try there
  (`openspec/changes/archive/2026-11-09-usb-transport/proposal.md:33-37`):
  Esc tried on `No link` switches the link, under a plate that says so.
- **The arrows buzz and flash their cell for 100 ms, as Up's and Down's do,
  and `/clear` and Esc keep 400 ms, each in its own cell** (the canvas's note `sent`).
- **What ← does on an empty prompt stays Claude Code's, and the guide says it:**
  it sends the session to the background and opens agent view (code.claude.com/docs/en/agent-view),
  not guarded in the app, which cannot see the prompt.
  Esc returns to the conversation, and `leftArrowOpensAgents` in `/config` turns it off.
- **The owner retakes `asset/screenshot/main.png` in qFlipper, as in `839a0f4`,**
  not a frame rendered by a script:
  the Flipper Apps Catalog takes qFlipper's own export (its `documentation/Manifest.md:127`, at `36b19a2`),
  and on Bluetooth the console stays for it.
- **One trial by the owner on a scratch session, its frames logged, and no press over RPC:**
  with this session's window focused, ← would send it to the background and Left's hold would clear it.
- **Complex, so implemented on `feature/side-arrows`:**
  `Action` and the core's `transport.h` gain two names each, three layers and four suites change,
  Esc changes its gesture, and the checks take the owner's hands.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `feat(keymap): send left and right arrows on a tap, Esc on a hold`.**

## What Changes

- **BREAKING** Esc acts when Right's long press fires, no longer as Right goes down.
- A tap on Left sends the left arrow and a tap on Right the right one,
  each with a buzz and a 100 ms flash.
- `D_Main` shows `[/clear | ◀]` and `[▶ | Esc]`, split as Up's and Down's plates are;
  one function draws a plate's fill, on either side.
- The guide's keys table, the Flipper Apps Catalog's description and the screenshot follow.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `button-actions`: Left's and Right's taps act, Esc acts on Right's hold, and nothing acts on a press.
- `prompt-control`: the left and right arrows join the plain actions, and Esc comes by a hold.
- `app-screen`: both side plates hold a tap and a hold, and the four arrows flash for 100 ms.
- `user-guide`: the keys table holds eleven rows.

## Impact

`src/core/`: `action.h`, `transport.h`, `keymap.c`, `actions.c`, `feedback.c`, `deed.c`;
`src/platform/transport_hid.c`; `src/ui/view_idle.c`;
`test/`: every suite but the display's;
`docs/guide/using-the-remote.md`, `docs/guide/troubleshooting.md`, `docs/internals/usb.md`,
`catalog/description.md` and `asset/screenshot/main.png`.
`app.c`, `screen.c` and the links' files stay as they are.
`usb-link`, `bluetooth-link`, `effort-control`, `dolphin-deeds`
and `button-actions`' `A tap or hold acts on the screen it began on` hold as written:
that rule names no button but Back.

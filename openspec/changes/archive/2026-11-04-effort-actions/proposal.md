# Proposal

## Why

The link is up and still carries nothing:
`git grep ble_profile_hid_kb src` finds no call,
so holding Up or Down only buzzes and flashes.
Changing effort is the first goal of the connection work,
and the brief's way of doing it fails here:
typed `/effort high` and Enter come out under a non-Latin layout as `.уаащке рушр`,
which Enter then sends to Claude as a message,
joined to whatever draft the prompt holds.
The brief's critique rates this risk high and leaves the decision open.
This change settles it, and `remote-actions` follows on the same keys.

Decisions, already settled:

- **One Ctrl chord per level, bound on the Mac to `command:effort high` and `command:effort max`,**
  not typed text, nor `command:effort` with the slider's arrows:
  the owner's call.
  The slider needs a pause for it to open, a series of presses and an end stop the documentation does not state,
  and a blind Enter that, with no slider open, sends the draft or approves a permission prompt.
  The chord needs none of them.
  The argument is not documented, and Claude Code's schema has no space in a command's name;
  its source runs it anyway.
  Tried on 2.1.311 on the Mac's own keyboard under a non-Latin layout,
  through iTerm2, tmux and herdr:
  `ctrl+x ctrl+q` set `max` twice, and the draft stayed.
- **The remote sends no Enter for effort,**
  which follows from the chord:
  a permission prompt cannot be approved by holding Up or Down.

Decisions proposed here, for the owner to strike:

- **`ctrl+x ctrl+f` for high, `ctrl+x ctrl+q` for max,**
  not one Ctrl letter each:
  of the single Ctrl letters only Q is free across Claude Code, tmux, herdr and the prompt's editing keys.
  `ctrl+x` is Claude Code's own chord prefix, an unbound second key only cancels it,
  and F or Q landing alone does no harm.
- **`high` is saved as the default and `max` lasts the session,**
  not a session-only `high`:
  that is what `/effort high` does when typed, and the brief asks for that command.
- **`README.md` holds the block for `keybindings.json`, and nothing writes it for the owner,**
  not a mise task:
  the file sits in the owner's Claude Code config, outside the repository,
  and the brief wants nothing installed on the Mac beyond configuration.
- **The buzz means sent, not applied,**
  not a confirmation from the Mac:
  a keyboard gets no answer, so with a dialog open or another window focused the buzz still comes.
- **Every other action still sends nothing,**
  not all of them at once:
  Enter, the arrows, Esc, `/clear` and voice are `remote-actions`.

## What Changes

- New `src/core/transport.h`, the keys the core presses;
  `src/platform/transport_bluetooth.c` implements it.
- New `src/core/actions.h`, `.c`: an action's keys;
  `app` performs them.
- New `test/actions_test.c` and `test/transport_fake.h`, `.c`;
  `.mise/tasks/test` links the fake.
- `README.md` says what to add to `keybindings.json`.

## Capabilities

### New Capabilities

- `effort-control`: setting Claude Code's effort from the device,
  whatever the Mac's layout and whatever the prompt holds.

### Modified Capabilities

- `button-actions`: `No action reaches the Mac` goes,
  replaced by `effort-control`'s `Nothing else reaches the Mac`.

## Impact

No tool enters `mise.toml`, and `application.fam` does not change.
The owner's `keybindings.json` gains two lines,
one of them already there from the trial above.
Holding Up now rewrites the saved level for the current model in the owner's settings.
The `/effort` of whatever session is focused changes,
the session implementing this change included.

# Proposal

## Why

On USB the press that wakes `Reconnect stopped` acts a second time once the cable is in.
It answers `ActionStartOver` and shows `No link` (`keymap.c:101-109`),
`transport_usb_offer` queues the cable's state at once (`transport_usb.c:55-58`),
the main screen comes up before the release,
and the same tap or hold then acts there (`keymap.c:83-87`):
OK's tap sends Enter, posting a draft or approving a permission prompt,
OK's hold starts dictation, Left's sends `/clear`, each with a buzz.
`button-actions` rules this out, "no buzz, no flash and no key sent",
and `docs/guide/using-the-remote.md:73-74` promises "sending nothing".
`usb-transport`'s task 6.3 walked this path, tapping Up, and saw the main screen at once
(`docs/internals/usb.md:121-125`).
The review of 2026-11-10 found it, confirmed it on the host,
and named its class: a gesture begun on one screen and ended on another,
closed so far for Right's hold alone, by `right_pressed_on_no_link`.
It is the review's one bug in what ships, so its change lands first.

Decisions proposed here, for the owner to strike:

- **A tap or hold acts only if the screen has not changed since its press, for every button,**
  not the start-over press alone, swallowed until its release as Right's is:
  one rule takes `right_pressed_on_no_link`'s place instead of a second flag beside it,
  and also closes the review's other window, a tap on `No link` that the link returns during.
- **Back's hold stays outside the rule,**
  not bound with the rest:
  exit sends no key, and holding Back leaves from every screen but voice.
- **Release stays outside the rule,**
  as it only ends voice or `Voice stopped`, whose OK went down on another screen by design.
- **One device check, task 6.3's path with a terminal as the witness,**
  not one per scenario:
  the rule lives in the core, which the host suite runs, and `app.c` hands it every input event.

## What Changes

- `keymap` keeps, per button, whether its press came on the screen still shown,
  and forgets it when the mode or the link changes;
  a tap or hold other than Back's acts only with it kept.
  `right_pressed_on_no_link` goes.
- `test/keymap_test.c` pins the rule, and every tap and hold there starts with its press, as the firmware's do.
- `docs/internals/usb.md` gives the rule beside Right's hold, and says the press that starts over sends nothing.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `button-actions`: a tap or hold acts on the screen it began on, Back's hold excepted.

## Impact

`src/core/keymap.c`, `test/keymap_test.c` and `docs/internals/usb.md`.
`keymap.h`, the platform layer and the screen stay as they are.

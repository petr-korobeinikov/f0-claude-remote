# Proposal

## Why

`bluetooth-link` contradicts itself:
its first requirement has the device advertise
"while the application is open on Bluetooth" (`openspec/specs/bluetooth-link/spec.md:11-14`),
and `The application offers the link a minute at a time` stops the offer
60 s after the opening, a drop or the press that starts over (`:51-55`).
The minute came with `reconnect-timeout` (`6be8b40`, 2026-11-08),
which renamed and modified the third requirement and left the first and the Purpose as they were.
The code follows the minute: `src/core/actions.c:46-49` withdraws and offers the link,
and `src/platform/transport_bluetooth.c:74-79` starts and stops advertising.
The review of 2026-11-10 found it.
A spec that says two things is a false statement, the first kind the housekeeping pass lands.

Decisions:

- **The first requirement advertises while the link is offered,**
  not the code advertising for as long as the app is open:
  the stop after 60 s is the owner's, settled in `reconnect-timeout`.
- **Its `Name` scenario says the same,**
  not `the application is open`:
  a scenario checks what its requirement promises, and past the minute the device does not advertise.
- **The Purpose is edited in the main spec directly,**
  as `docs-from-main` does for `docs-deploy`:
  sync and archive ignore a delta's Purpose,
  and `for as long as the application is open` is the same false statement,
  which since `usb-transport` also ignores a hold of Right that takes USB.

## What Changes

- `bluetooth-link`'s `The application is a keyboard of its own` and its `Name` scenario
  hold advertising to the offer.
- `bluetooth-link`'s Purpose becomes:
  `Makes the device the Mac's Bluetooth keyboard, offered a minute at a time,
  with a bond of its own that survives restarts and a radio handed back on leaving.`

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `bluetooth-link`: the device advertises as `Claude Remote` while the application offers the link, not while it is open.

## Impact

`openspec/specs/bluetooth-link/spec.md` alone.
The app, `README.md` and `docs/` stay as they are:
`docs/guide/pairing.md:20` and `docs/internals/bluetooth.md:74` already give the minute.

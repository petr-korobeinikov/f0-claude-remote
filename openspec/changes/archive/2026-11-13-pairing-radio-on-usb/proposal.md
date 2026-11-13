# Proposal

## Why

`docs/guide/pairing.md:36-37` says "While the app is open it holds the radio,
so the phone app's connection waits".
Since `d421d5b` (2026-11-09) a hold of Right on `No link` takes USB with the app open,
and taking it frees the Bluetooth transport (`src/core/actions.c:51-52`, `src/platform/transport_hid.c:68-72`),
whose `free` restores the default key path and profile (`src/platform/transport_bluetooth.c:53-62`),
as `bluetooth-link`'s `Leaving hands the radio back` requires of "Ending the application, or taking USB"
(`openspec/specs/bluetooth-link/spec.md:81-83`).
`usb-transport` left the page as it was,
and its `user-guide` delta left `Pairing is written down`,
whose scenario `Moved` pins the README's line of 2026-11-07,
"While the app is open it holds the radio, and on leaving it hands the radio back"
(`openspec/specs/user-guide/spec.md:38-44`; `a11f165^:README.md`).
`bluetooth-link-offer`'s task 2.1 found it (`openspec/changes/archive/2026-11-12-bluetooth-link-offer/tasks.md:18`).
A page that has the phone app wait while the app is on USB is a false statement,
the first kind the housekeeping pass lands.

Decisions:

- **`Moved` checks only that the README holds no step, and three scenarios check what the page holds,**
  not the page against the README of 2026-11-07,
  which `reconnect-timeout` already broke (`6be8b40`, 2026-11-08), dropping its 30 s Connect for a press on `Reconnect stopped`,
  and which this change breaks again.
- **Relatively simple, so implemented on `develop`:**
  two lines of one page and one requirement, and no code.
- **It builds on `develop` alone:**
  `wait-from-the-keymap`, queued, touches neither the page nor `user-guide`.
- **It lands as `docs(pairing): hold the radio only on bluetooth`.**

## What Changes

- `docs/guide/pairing.md:36-39` become, lines 36 and 38 changed:

  ```markdown
  While the app is on Bluetooth it holds the radio,
  so the phone app's connection waits.
  On taking [USB](usb.md) or leaving, it hands the radio back,
  and the device's Bluetooth stands as its setting says.
  ```

  The link is the Guide's own, as in `docs/guide/using-the-remote.md:9,62`.
- `user-guide`'s `Pairing is written down` holds the radio to Bluetooth,
  and checks the page by what it holds instead of against the README.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `user-guide`: the pairing page has the app hold the radio only while on Bluetooth, and is checked by what it holds.

## Impact

`docs/guide/pairing.md` alone, and `openspec/specs/user-guide/spec.md` through the archive.
The app, `README.md` and the rest of `docs/` stay as they are:
`docs/internals/usb.md:66` already makes freeing Bluetooth its leaving.

# Proposal

## Why

A Mac with no Bluetooth link to the device cannot use the remote:
`D_disconnected` only waits, and 60 s later `D_stopped` gives up,
though the firmware's USB keyboard takes the same key codes (`furi_hal_usb_hid.h`)
and neither `keymap` nor `actions` names a link.
USB comes next, by the owner's "Bluetooth first, USB after" of 2026-11-04.

Decisions, already settled by the owner:

- **One application that switches to USB from inside,**
  not a second application in Apps → USB, as the stock Remote builds (`hid_app/application.fam`),
  nor a choice by the cable:
  one `.fap` and one Flipper Apps Catalog listing, with the build and the release untouched;
  the cable would take the console at every launch on it,
  and `furi_hal_usb.h` has no query for what it ends in.
- **USB is offered on `No link` alone, as a fallback:**
  a paired Mac in range brings Bluetooth up in 0.87 s (`ble-transport`'s task 4.3),
  so there USB takes forgetting the device or turning Bluetooth off.
- **USB's boards carry a USB mark and a line about the cable,**
  not the Bluetooth mark and `Reconnecting...`.
- **Holding Right on `D_usb_disconnected` returns to Bluetooth,**
  not the cable coming out, nor 60 s with no cable, nor leaving:
  a device off the cable reaches the Mac again, by the same gesture.
- **Every no-link board keeps its lines on `D_stopped`'s rows, and `D_disconnected` and `D_usb_disconnected` add a plate of `Switch to` and the target's mark,**
  not `USB` in the corner:
  the screens read as serious, and nothing jumps.

Decisions proposed here, for the owner to strike:

- **Holding Right takes USB,**
  not a tap nor another button:
  the switch ends the link in use,
  and Right alone has no hold of its own,
  where OK, Up, Down or Left tried with no link would leave Bluetooth.
- **A hold counts only from a press made on its own board:**
  the long press fires at 300 ms (`input.c:13-14`),
  so a slow press starting over on `D_stopped` would switch the link too.
- **Nothing is saved,**
  not a setting:
  the next launch opens on Bluetooth.
- **USB keeps the 60 s and `D_stopped`, and each hold counts them afresh,**
  not a rule of its own:
  a forgotten device still goes dark,
  and a cable connected past the minute waits for a press.
- **A hold neither buzzes nor flashes, and the board changes at once:**
  a buzz means sent.
- **The line reads `Connect the cable`, and the Mac lists the keyboard as `Claude Remote`,**
  not nameless, as the stock Remote's `NULL` config leaves it.
- **`README.md`'s and Home's Features, `fap_description` and the Flipper Apps Catalog's description
  gain a line about USB.**

## What Changes

- `keymap` gains the holds; `actions` and `feedback` the new actions; `transport.h` the switches.
- New `src/platform/transport_hid.h`, `.c` route the core's keys to the link in use;
  new `src/platform/transport_usb.h`, `.c` are the USB keyboard.
- New `src/core/link.h`; `screen` carries the link,
  and `view` draws USB's mark and line and both plates.

## Capabilities

### New Capabilities

- `usb-link`: the device as the Mac's USB keyboard, from the hold until the return or leaving.

### Modified Capabilities

- `bluetooth-link`: taking USB hands the radio back, and the return brings the keyboard again.
- `app-screen`: the redrawn no-link boards, USB's boards, and a launch replacing the app on Bluetooth alone.
- `button-actions`: holding Right acts with no link.
- `user-guide`: `USB` is written down, and three pages follow.
- `internals-guide`: `USB` explains the switch, and a press's path gains `transport_hid.c`.
- `docs-site`: the sidebar lists the two new pages.

## Impact

No dependency, and `fap_libs` stays:
`furi_hal_hid_*`, `furi_hal_usb_*` and `usb_hid` are public in SDK 1.5.1 (`api_symbols.csv`).
While on USB the device has no console,
so `mise run launch` and frames over RPC wait for leaving USB,
and USB's linked boards are checked by the owner's eye.
The device checks forget the device on the Mac once and pair again at the end.

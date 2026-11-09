# Proposal

## Why

`D_disconnected` and `D_usb_disconnected` fill one element, `Switch to`,
and nothing on them says a fill means a hold:
`app-screen` gives them "no ring or legend", and the only legend stands on `D_Main`.
The owner found it on the device after `usb-transport` landed,
and the boards are redrawn, at canvas version `1794235687-4b85`.

Decisions, settled by the owner:

- `HOLD` alone, not `HOLD | TAP`: nothing on these two screens takes a press.
- `D_disc_voice`, `D_stopped` and their USB copies stay with no legend:
  the first has no button, and the second's `exit` is a tap in a thin frame.
- The check is a look at the two screens, with nothing beyond it.

## What Changes

- `D_disconnected` and `D_usb_disconnected` show `HOLD` at the right end of the status row.
- `app-screen` stops ruling out a legend on those two,
  and `D_stopped` stops taking `D_disconnected`'s status row whole.
- `docs/guide/using-the-remote.md` names `HOLD` on `No link`.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-screen`: the no-link boards with a plate gain the `HOLD` legend; the other no-link boards keep none.

## Impact

`src/ui/view.c` and `docs/guide/using-the-remote.md`.
The core, the platform layer, the keymap and the host tests stay as they are.

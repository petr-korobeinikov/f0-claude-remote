# Proposal

## Why

`SECURITY.md:21-22` says that to the Mac the device is
"a Bluetooth keyboard, paired once in the Mac's settings and confirmed on the device".
Since `d421d5b` (2026-11-09) a hold of Right on `No link` makes it a USB keyboard as well,
which pairs nothing: whatever computer the cable reaches takes it as `Claude Remote`
on `046D:C529`, the firmware's default IDs and Logitech's (`src/platform/transport_usb.c:11-17`).
The file last changed in `4129eb7` (2026-11-07), and no spec holds what it says,
so the change that brought USB left it as it was.
`usb-link` names the keyboard `Claude Remote` but not its IDs,
which `docs/internals/usb.md:43-47` alone gives.
The review of 2026-11-10 found it.
GitHub shows the file from `develop`, its default branch, where USB already is,
and a security policy that leaves out a way in is a false statement, the first kind the housekeeping pass lands.

Decisions:

- **The IDs are named with their owner, Logitech,**
  not `Claude Remote` alone:
  the name is the app's to pick, and nothing in it tells a reader the device shows up under Logitech's IDs.
- **`usb-link` names the IDs,**
  not `SECURITY.md` alone:
  a change of IDs is then a spec change, and `security-policy` leads it to this file.
- **A capability of its own, `security-policy`,**
  not a requirement in `user-guide`, whose pages take a user to a working remote:
  this file's reader weighs the risk,
  and a change that adds a link or a lock finds `SECURITY.md` among the specs it touches.

## What Changes

- `SECURITY.md`'s `## What the App Can Do` names both links, how the Mac comes to trust each,
  and USB's name, IDs and their owner, linking the guide for the Mac's alert.
- `usb-link`'s keyboard requirement names the IDs beside the name.

## Capabilities

### New Capabilities

- `security-policy`: `SECURITY.md` names each way the device reaches the Mac, how the Mac trusts it, and that a press has no lock.

### Modified Capabilities

- `usb-link`: on USB the Mac lists the keyboard on vendor ID `046D` and product ID `C529`.

## Impact

`SECURITY.md` alone.
The app, `README.md` and `docs/` stay as they are.

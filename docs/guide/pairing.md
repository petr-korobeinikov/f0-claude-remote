---
description: Pairing the device with the Mac once, and what happens after.
---

# Pairing

The app is a Bluetooth keyboard of its own, `Claude Remote`,
with an address and keys apart from the device's stock Remote,
so pairing one leaves the other's untouched.
It advertises whatever the device's Bluetooth setting.

## Once per Mac

With the device on its desktop:

1. Run `mise run launch`.
   Not over the device's open Apps browser:
   the pairing dialog waits behind it, and closing the app hangs until the browser ends.
2. In the Mac's Bluetooth settings, press Connect on `Claude Remote` under Nearby Devices,
   and accept the request.
   This takes up to about 30 seconds once the device has advertised for a minute;
   press it again if it times out.
3. Press OK in `Verify code` on the device.
   Cancel leaves the app open on `No link`.
4. Close Keyboard Setup Assistant if it opens: the device has no keys for it.

## After pairing

From then on the Mac connects by itself, within a second of a drop,
and the app reconnects with no press when it opens again.

To pair again, forget the device on the Mac first.
Connecting then shows `Verify code` again.

While the app is open it holds the radio,
so the phone app's connection waits.
On leaving, it hands the radio back,
and the device's Bluetooth stands as its setting says.

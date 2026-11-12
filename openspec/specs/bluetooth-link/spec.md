# bluetooth-link Specification

## Purpose

Makes the device the Mac's Bluetooth keyboard, offered a minute at a time,
with a bond of its own that survives restarts and a radio handed back on leaving.

## Requirements

### Requirement: The application is a keyboard of its own
While the application is open on Bluetooth, the device SHALL advertise as a keyboard
named `Claude Remote`,
at an address and with a key file of its own,
whatever the device's Bluetooth setting.

#### Scenario: Name
- **WHEN** the application is open and the Mac's Bluetooth settings are open
- **THEN** they list `Claude Remote`

#### Scenario: Bluetooth off in the device's settings
- **WHEN** Bluetooth is off in the device's settings and the application opens
- **THEN** the Mac's Bluetooth settings list it

#### Scenario: Keys apart
- **WHEN** the owner has paired
- **THEN** `/ext/apps_data/f0_claude_remote/.bt_hid.keys` exists,
  and `/ext/apps_data/hid_ble/.bt_hid.keys` keeps its MD5

### Requirement: Pairing starts on the Mac and ends in the device's own dialog
Pairing SHALL start from the Mac's Bluetooth settings
and end with the firmware's `Verify code` dialog,
with no screen of the application's for it.

#### Scenario: Confirmed
- **WHEN** no bond exists, the owner connects from the Mac and presses OK in the dialog
- **THEN** a frame read off the device equals `D_Main`

#### Scenario: Canceled
- **WHEN** no bond exists, the owner connects from the Mac and presses Cancel in the dialog
- **THEN** the application stays open, a frame equals `D_disconnected`,
  and the Mac shows no connection

#### Scenario: Opened again
- **WHEN** the application closes and opens again after pairing
- **THEN** the link returns with no dialog and no press on either device

#### Scenario: Pairing again
- **WHEN** the Mac forgets the device and connects to it again
- **THEN** the dialog shows, and OK in it brings the link up

### Requirement: The application offers the link a minute at a time
From its opening, a drop, or the press that starts over,
the application SHALL offer the link with no press on the device for 60 s,
which no other press extends,
and then SHALL stop offering it until a button is pressed.

#### Scenario: Disconnected from the Mac
- **WHEN** the Mac's settings disconnect the device, and the Mac connects again by itself
- **THEN** a frame equals `D_disconnected`, then `D_Main`, with no press on the device

#### Scenario: Forgotten on the Mac
- **WHEN** the Mac forgets the device, and connects to it 20 s later
- **THEN** a frame equals `D_disconnected` until the dialog shows

#### Scenario: A minute with no link
- **WHEN** the Mac forgets the device and 60 s pass with no press
- **THEN** a frame equals `D_stopped`, and a connect from the Mac's settings brings no dialog to the device

#### Scenario: A press does not extend it
- **WHEN** a button is tapped 30 s after a drop
- **THEN** the offer still stops 60 s after the drop

#### Scenario: Started over
- **WHEN** a button is pressed on `D_stopped`, and the Mac connects within 60 s
- **THEN** the link comes up, through the dialog if the Mac had forgotten the device

#### Scenario: A paired Mac comes back
- **WHEN** a button is pressed on `D_stopped` with the paired Mac in range
- **THEN** a frame equals `D_Main` with no other press

### Requirement: Leaving hands the radio back
Ending the application, or taking USB, SHALL end the link
and return the device to its default Bluetooth, as its setting stands.

#### Scenario: Setting on
- **WHEN** the application ends with Bluetooth on in the device's settings
- **THEN** the Mac shows no connection, the device's status bar shows the Bluetooth mark,
  and `/int/.bt.keys` keeps the MD5 it had before the application opened

#### Scenario: Setting off
- **WHEN** the application ends with Bluetooth off in the device's settings
- **THEN** the Mac shows no connection and the device's status bar shows no Bluetooth mark

#### Scenario: USB taken
- **WHEN** USB is taken and the application then ends
- **THEN** `/int/.bt.keys` keeps the MD5 it had before the application opened

### Requirement: Holding Right on USB's no-link screen returns to Bluetooth
Holding Right on `D_usb_disconnected`, from a press made there, until the long press fires,
SHALL end the USB keyboard and make the device the Bluetooth keyboard again, on its own key file,
with no buzz, no flash and no key sent,
and SHALL count the 60 s offer afresh from the hold.

#### Scenario: Returned
- **WHEN** Right is held on `D_usb_disconnected`
- **THEN** `D_disconnected` shows at once, and the Mac's Bluetooth settings list `Claude Remote`

#### Scenario: Counted afresh
- **WHEN** Right is held 20 s after `D_usb_disconnected` first showed
- **THEN** `D_stopped` shows 60 s after the hold

#### Scenario: A paired Mac comes back
- **WHEN** Right is held on `D_usb_disconnected` with the paired Mac in range
- **THEN** a frame equals `D_Main` with no other press and no dialog

#### Scenario: Taken again
- **WHEN** Right is held on `D_disconnected` after a return
- **THEN** `D_usb_disconnected` shows at once

#### Scenario: A press made elsewhere
- **WHEN** Right goes down on `D_usb_stopped` and stays down past the long press
- **THEN** `D_usb_disconnected` shows, on USB

#### Scenario: Another screen
- **WHEN** Right is held on `D_usb_disc_voice`, `D_usb_main` or `D_usb_voice`
- **THEN** the application stays on USB

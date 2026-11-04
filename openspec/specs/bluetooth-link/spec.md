# bluetooth-link Specification

## Purpose

Makes the device the Mac's Bluetooth keyboard for as long as the application is open,
with a bond of its own that survives restarts and a radio handed back on leaving.

## Requirements

### Requirement: The application is a keyboard of its own
While the application is open, the device SHALL advertise as a keyboard
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

### Requirement: The application keeps offering the link after a drop
After a drop the application SHALL keep offering the link,
with no press on the device, for as long as it is open.

#### Scenario: Disconnected from the Mac
- **WHEN** the Mac's settings disconnect the device, and the Mac connects again by itself
- **THEN** a frame equals `D_disconnected`, then `D_Main`, with no press on the device

#### Scenario: Forgotten on the Mac
- **WHEN** the Mac forgets the device, and connects to it 2 minutes later
- **THEN** a frame equals `D_disconnected` until the dialog shows

### Requirement: Leaving hands the radio back
Ending the application SHALL end the link
and return the device to its default Bluetooth, as its setting stands.

#### Scenario: Setting on
- **WHEN** the application ends with Bluetooth on in the device's settings
- **THEN** the Mac shows no connection, the device's status bar shows the Bluetooth mark,
  and `/int/.bt.keys` keeps the MD5 it had before the application opened

#### Scenario: Setting off
- **WHEN** the application ends with Bluetooth off in the device's settings
- **THEN** the Mac shows no connection and the device's status bar shows no Bluetooth mark

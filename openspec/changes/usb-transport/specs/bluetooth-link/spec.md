## MODIFIED Requirements

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

## ADDED Requirements

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

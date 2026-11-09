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

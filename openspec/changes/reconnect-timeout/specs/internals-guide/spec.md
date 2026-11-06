## MODIFIED Requirements

### Requirement: Bluetooth explains the app's own profile
`docs/internals/bluetooth.md` SHALL explain the app's own HID profile:
its name within the 16 characters advertised, its own address and keys file,
pairing in the firmware's `Verify code` dialog, advertising after a drop and its stop 60 s later,
keys released on a drop, the radio handed back on leaving, and why the open Apps browser blocks pairing.

#### Scenario: Marks
- **WHEN** each fact is read
- **THEN** it names its kind, documented, firmware 1.5.1's sources or seen,
  and links its source: the documentation page, the file and line at tag `1.5.1`, or the archived task

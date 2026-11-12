## MODIFIED Requirements

### Requirement: On USB the application is a keyboard named Claude Remote
On USB, with the cable in, the Mac SHALL list the device as `Claude Remote`
on vendor ID `046D` and product ID `C529`,
and every button SHALL do what it does over Bluetooth.

#### Scenario: Name
- **WHEN** the cable is in on USB
- **THEN** `system_profiler SPUSBHostDataType` lists `Claude Remote` on vendor ID `046D` and product ID `C529`

#### Scenario: Buttons
- **WHEN** the owner runs a Claude Code session on USB, dictation included
- **THEN** the owner reports no difference from Bluetooth

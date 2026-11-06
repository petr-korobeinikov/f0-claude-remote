## RENAMED Requirements

- FROM: `### Requirement: The application keeps offering the link after a drop`
- TO: `### Requirement: The application offers the link a minute at a time`

## MODIFIED Requirements

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

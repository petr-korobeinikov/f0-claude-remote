## MODIFIED Requirements

### Requirement: A press acts with the backlight off
A button SHALL act the same whether or not the backlight is on.

#### Scenario: Backlight off
- **WHEN** the backlight is off on `D_stopped` and Up is tapped
- **THEN** a frame equals `D_disconnected`

### Requirement: With no link only holding Back acts
With no link, holding Back SHALL exit, and no other press SHALL act, vibrate or flash,
so a press with no buzz was not sent;
`D_stopped` alone answers a press otherwise, as its requirements say.

#### Scenario: Every other press
- **WHEN** the link is offered with none up, every other button is tapped and held, and Back is tapped
- **THEN** nothing acts, the device does not vibrate and the screen does not change

#### Scenario: Exit
- **WHEN** there is no link and Back is held until the long press fires
- **THEN** the application ends

#### Scenario: The link returns
- **WHEN** the link returns and Up is tapped
- **THEN** the up arrow acts

## ADDED Requirements

### Requirement: A press on the stopped screen starts over
The first press of any button but Back on `D_stopped` SHALL offer the link again,
with no buzz, no flash and no key sent.

#### Scenario: Any button but Back
- **WHEN** Up, Down, Left, Right or OK is pressed on `D_stopped`
- **THEN** the device does not vibrate, a frame equals `D_disconnected`, and no key is sent

#### Scenario: OK held
- **WHEN** OK is held on `D_stopped` past the long press
- **THEN** voice does not start

## RENAMED Requirements

- FROM: `### Requirement: With no link only holding Back acts`
- TO: `### Requirement: With no link only holding Back or Right acts`

## MODIFIED Requirements

### Requirement: With no link only holding Back or Right acts
With no link, holding Back SHALL exit, holding Right on `D_disconnected` SHALL take USB, as `usb-link` says,
and no other press SHALL act, vibrate or flash,
so a press with no buzz was not sent;
`D_stopped` alone answers a press otherwise, as its requirements say.

#### Scenario: Every other press
- **WHEN** the link is offered with none up, every button but Back and Right is tapped and held, and Back and Right are tapped
- **THEN** nothing acts, the device does not vibrate and the screen does not change

#### Scenario: Exit
- **WHEN** there is no link and Back is held until the long press fires
- **THEN** the application ends

#### Scenario: The link returns
- **WHEN** the link returns and Up is tapped
- **THEN** the up arrow acts

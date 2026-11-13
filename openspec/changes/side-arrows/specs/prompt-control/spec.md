## MODIFIED Requirements

### Requirement: The plain actions do what their keys do on the Mac's keyboard
With the link up,
Enter, the arrows and Esc SHALL act
in the Claude Code session focused on the Mac,
under a Latin and a non-Latin layout,
with nothing configured on the Mac.

#### Scenario: Enter
- **WHEN** the prompt holds text and OK is tapped
- **THEN** the message is sent

#### Scenario: The arrows
- **WHEN** the prompt is empty after a sent message, Up is tapped, then Down
- **THEN** the prompt shows that message, then is empty again

#### Scenario: A menu
- **WHEN** Claude Code shows a menu, Down is tapped, then OK
- **THEN** the next option is picked

#### Scenario: The side arrows
- **WHEN** Claude Code shows the slider of `/effort` above its lowest level, Left is tapped, then Right
- **THEN** the slider moves one level down, then back

#### Scenario: Esc
- **WHEN** Claude is working and Right is held until the long press fires
- **THEN** the turn stops

### Requirement: Each plain action is one key
Enter, the four arrows and Esc
SHALL each press and release their key once,
with no modifier.

#### Scenario: The plain keys
- **WHEN** Enter, an arrow or Esc acts
- **THEN** the keys are, in order, that key down and up, and nothing else

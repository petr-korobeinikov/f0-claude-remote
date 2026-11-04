# Spec Delta

## MODIFIED Requirements

### Requirement: The screen shows the status and every button's action
The idle screen, with the link up, voice off and no flash running, SHALL show a status row above a ring of the device's buttons,
with a thin frame on every action taken by a press,
a black plate on every held one,
and a legend saying so.

#### Scenario: Status row
- **WHEN** the screen is idle
- **THEN** the top row reads, left to right,
  a Bluetooth mark, `Connected`, and a legend of `HOLD` joined to `TAP`,
  above a line across the display

#### Scenario: Legend
- **WHEN** the screen is idle
- **THEN** a ring holds an element split by a diagonal,
  a microphone at its upper left and an Enter arrow at its lower right,
  with `/effort high` and an up arrow at the ring's top,
  `/clear` and a left arrow on its left,
  a right arrow and `Esc` on its right,
  `/effort max` and a down arrow at its bottom,
  and a Back mark and `exit` at the lower right of the display

#### Scenario: Hold and press read apart
- **WHEN** the screen is idle
- **THEN** `HOLD`, `/effort high`, `/effort max`, the microphone, `/clear`, `exit`, the left arrow and the Back mark stand white on a black plate,
  and `TAP`, the up, down and right arrows, the Enter arrow and `Esc` stand black in a thin frame

#### Scenario: The board
- **WHEN** the link comes up with voice off and no flash running
- **THEN** a frame read off the device equals board `D_Main`

#### Scenario: Nothing clipped
- **WHEN** the application is open, in any state
- **THEN** every label is whole and stands clear of its neighbors

## REMOVED Requirements

### Requirement: The status is static
**Reason**: a real link makes a fixed `Connected` a lie.
**Migration**: `The status follows the link` replaces it.

## ADDED Requirements

### Requirement: The status follows the link
The screen SHALL read `Connected` only while the Mac holds a paired link.
With no link it SHALL show `No link` and `Reconnecting...` with no ring or legend,
and `Keys are not sent` unless a voice was cut.

#### Scenario: Opened with no link
- **WHEN** the application opens with no Mac connected
- **THEN** a frame read off the device equals board `D_disconnected`

#### Scenario: The link drops
- **WHEN** the link drops on the idle screen
- **THEN** a frame equals `D_disconnected`, with no press

#### Scenario: The link drops during a flash
- **WHEN** the link drops while a flash runs
- **THEN** a frame equals `D_disconnected`, and stays equal past the flash's end

### Requirement: A voice cut by the link has a screen of its own
While OK stays down after a link lost during voice,
the screen SHALL show `No link`, `Reconnecting...` and `Voice stopped: release OK`.

#### Scenario: The link drops during voice
- **WHEN** the link drops during voice
- **THEN** a frame equals `D_disc_voice`

#### Scenario: OK released
- **WHEN** OK is released on that screen
- **THEN** a frame equals `D_disconnected`

#### Scenario: The link returns with OK still down
- **WHEN** the link returns on that screen
- **THEN** a frame equals `D_Main`

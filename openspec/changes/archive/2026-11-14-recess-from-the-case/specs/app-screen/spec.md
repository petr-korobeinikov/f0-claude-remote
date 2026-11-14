## MODIFIED Requirements

### Requirement: The screen shows the status and every button's action
The idle screen, with the link up, voice off and no flash running, SHALL show a status row above a ring of the device's buttons,
with a thin frame on every action taken by a press,
a black plate on every held one,
and a legend saying so.
The ring and Back's plate SHALL stand in one outline, shaped as the recess the device's case has around its buttons.

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
  and a Back mark and `exit` under `Esc`

#### Scenario: The recess
- **WHEN** the screen is idle
- **THEN** one outline holds the split element and Back's plate, and passes under the ring's four plates:
  round about the ring, then narrowing to a round end past Back's plate, with a level bottom,
  and no line of it inside a plate

#### Scenario: Hold and press read apart
- **WHEN** the screen is idle
- **THEN** `HOLD`, `/effort high`, `/effort max`, the microphone, `/clear`, `Esc`, `exit` and the Back mark stand white on a black plate,
  and `TAP`, the four arrows and the Enter arrow stand black in a thin frame

#### Scenario: The board
- **WHEN** the link comes up with voice off and no flash running
- **THEN** a frame read off the device equals board `D_Main`

#### Scenario: Nothing clipped
- **WHEN** the application is open, in any state
- **THEN** every label is whole and stands clear of its neighbors

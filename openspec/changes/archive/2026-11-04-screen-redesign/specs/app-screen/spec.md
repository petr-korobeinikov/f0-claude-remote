# Spec Delta

## REMOVED Requirements

### Requirement: A sent command flashes its element
**Reason**: Enter and the arrows flash now.
**Migration**: `A sent action flashes its element` replaces it.

## MODIFIED Requirements

### Requirement: The screen shows the status and every button's action
The idle screen, with voice off and no flash running, SHALL show a status row above a ring of the device's buttons,
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
- **WHEN** the application opens
- **THEN** a frame read off the device equals board `D_Main`

#### Scenario: Nothing clipped
- **WHEN** the application is open, in any state
- **THEN** every label is whole and stands clear of its neighbors

## ADDED Requirements

### Requirement: A sent action flashes its element
Effort high, effort max, clear, Esc and Enter SHALL each invert the inside of their part of their element for 400 ms,
and the up and down arrows theirs for 100 ms,
with the element's frame unchanged.

#### Scenario: The long flash
- **WHEN** one of the five acts
- **THEN** a frame read off the device equals its board,
  `D_sent_high`, `D_sent_max`, `D_sent_clear`, `D_sent_esc` or `D_sent_enter`,
  and 400 ms later one equals `D_Main`

#### Scenario: The short flash
- **WHEN** the up or down arrow acts
- **THEN** a frame read off the device equals `D_sent_up` or `D_sent_down`,
  and 100 ms later one equals `D_Main`

#### Scenario: Only the half that acted flashes
- **WHEN** an action of Up, Down or OK flashes
- **THEN** the other half of its element does not change

#### Scenario: Acting again restarts it
- **WHEN** an action flashes while a flash runs
- **THEN** that action's flash shows and lasts its own duration from then on

#### Scenario: A series of arrow taps
- **WHEN** Up is tapped 5 times a second
- **THEN** frames read off the device alternate `D_sent_up` and `D_Main`, once per tap,
  and the owner sees each flash apart

#### Scenario: Voice takes over
- **WHEN** voice starts while a flash runs and stays on past its end
- **THEN** the voice screen shows throughout

# Spec Delta

## MODIFIED Requirements

### Requirement: The screen shows the status and every button's action
The idle screen, with voice off and no flash running, SHALL show a status row above a ring of the device's buttons,
with a thin frame on every action taken by a press,
a black plate on every held one,
and a legend saying so.

#### Scenario: Status row
- **WHEN** the screen is idle
- **THEN** the top row reads, left to right,
  a Bluetooth mark, `Connected`, and a legend of `TAP` joined to `HOLD`,
  above a line across the display

#### Scenario: Legend
- **WHEN** the screen is idle
- **THEN** a ring holds `Enter` over `voice`,
  with an up arrow and `high` at its top,
  `clear` and a left arrow on its left,
  a right arrow and `Esc` on its right,
  a down arrow and `max` at its bottom,
  and a Back mark and `exit` at the lower left

#### Scenario: Hold and press read apart
- **WHEN** the screen is idle
- **THEN** `HOLD`, `high`, `max`, `voice`, `clear`, `exit`, the left arrow and the Back mark stand white on a black plate,
  and `TAP`, the up, down and right arrows, `Enter` and `Esc` stand black in a thin frame

#### Scenario: Nothing clipped
- **WHEN** the application is open, in any state
- **THEN** every label is whole and stands clear of its neighbors

## ADDED Requirements

### Requirement: Voice has a screen of its own
While voice is on, the screen SHALL show, white on black, the Bluetooth mark and `Connected` above a line across the display,
a microphone, `Listening` and `Release OK to stop`,
with no legend and no ring.

#### Scenario: Voice on
- **WHEN** voice starts
- **THEN** a frame read off the device equals board `D_voice`

#### Scenario: Voice off
- **WHEN** voice stops
- **THEN** a frame read off the device equals board `D_Main`

### Requirement: A sent command flashes its element
Effort high, effort max, clear and Esc SHALL each invert the inside of their element for 400 ms,
with the element's frame unchanged.

#### Scenario: The flash
- **WHEN** one of the four acts
- **THEN** a frame read off the device equals its board,
  `D_sent_high`, `D_sent_max`, `D_sent_clear` or `D_sent_esc`,
  and 400 ms later one equals `D_Main`

#### Scenario: Only the held half flashes
- **WHEN** effort high or effort max acts
- **THEN** its arrow's half of the element does not change

#### Scenario: Acting again restarts it
- **WHEN** a command acts while a flash runs
- **THEN** that command's flash shows and lasts 400 ms from then on

#### Scenario: Arrows and Enter do not flash
- **WHEN** an arrow or Enter acts
- **THEN** the screen does not change, and a running flash runs on

#### Scenario: Voice takes over
- **WHEN** voice starts while a flash runs and stays on past its 400 ms
- **THEN** the voice screen shows throughout

## REMOVED Requirements

### Requirement: The other buttons do nothing
**Reason**: They act now.
**Migration**: `button-actions` states what each does.

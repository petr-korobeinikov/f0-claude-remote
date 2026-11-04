# app-screen Specification

## Purpose

Shows the remote's one screen.

## Requirements

### Requirement: The application carries its name
The device SHALL list the application as `Claude Remote`.

#### Scenario: Apps menu
- **WHEN** Apps → Examples opens on the device after `mise run launch`
- **THEN** it lists `Claude Remote`

### Requirement: The application opens on its one screen
Starting the application SHALL show its screen across the whole display,
with no menu first and no system status bar.

#### Scenario: Start
- **WHEN** `mise run launch` runs
- **THEN** the device shows the screen, with no status bar above it

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

### Requirement: The status is static
The status SHALL read `Connected` whatever the device's Bluetooth state.

#### Scenario: Bluetooth off
- **WHEN** Bluetooth is off in the device's settings and the application opens
- **THEN** the status reads `Connected`

### Requirement: Holding Back leaves the application
Holding Back for the device's long press SHALL end the application,
and a short press SHALL do nothing.

#### Scenario: Long press
- **WHEN** Back is held until the long press fires
- **THEN** the application ends and the device returns to the screen it was started from

#### Scenario: Short press
- **WHEN** Back is pressed and released before the long press fires
- **THEN** the application stays open and its screen does not change

### Requirement: A new launch replaces the running application
`mise run launch` SHALL succeed while the application is open.

#### Scenario: Launch over a running copy
- **WHEN** `mise run launch` runs while the application is open
- **THEN** it exits 0, its log reads `App closed`, and the application opens again

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

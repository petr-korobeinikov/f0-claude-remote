# Spec Delta

## Purpose

Shows the remote's one screen.

## ADDED Requirements

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
The screen SHALL show a status row above a ring of the device's buttons,
with a thin frame on every action taken by a press,
a black plate on every held one,
and a legend saying so.

#### Scenario: Status row
- **WHEN** the application is open
- **THEN** the top row reads, left to right,
  a Bluetooth mark, `Connected`, and a legend of `TAP` joined to `HOLD`,
  above a line across the display

#### Scenario: Legend
- **WHEN** the application is open
- **THEN** a ring holds `Enter` over `voice`,
  with an up arrow and `high` at its top,
  `clear` and a left arrow on its left,
  a right arrow and `Esc` on its right,
  a down arrow and `max` at its bottom,
  and a Back mark and `exit` at the lower left

#### Scenario: Hold and press read apart
- **WHEN** the application is open
- **THEN** `HOLD`, `high`, `max`, `voice`, `clear`, `exit`, the left arrow and the Back mark stand white on a black plate,
  and `TAP`, the up, down and right arrows, `Enter` and `Esc` stand black in a thin frame

#### Scenario: Nothing clipped
- **WHEN** the application is open
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

### Requirement: The other buttons do nothing
Up, Down, Left, Right and OK SHALL have no effect, pressed or held.

#### Scenario: Any other button
- **WHEN** one of them is pressed or held
- **THEN** the screen does not change, the device does not vibrate
  and the Mac receives no keystroke

### Requirement: A new launch replaces the running application
`mise run launch` SHALL succeed while the application is open.

#### Scenario: Launch over a running copy
- **WHEN** `mise run launch` runs while the application is open
- **THEN** it exits 0, its log reads `App closed`, and the application opens again

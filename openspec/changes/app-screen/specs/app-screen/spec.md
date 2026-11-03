# Spec Delta

## Purpose

Shows the remote's one screen on the Flipper,
with what each button stands for and how to leave.

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
The screen SHALL show a status row above a legend
that places each action where its button sits,
with a black plate on every action taken by holding the button.

#### Scenario: Status row
- **WHEN** the application is open
- **THEN** the top row reads, left to right,
  a Bluetooth mark, `Connected`, a Back mark and `EXIT`,
  above a line across the display

#### Scenario: Legend
- **WHEN** the application is open
- **THEN** the legend shows an up arrow and `high` at the top,
  `clear` on the left,
  a frame around a microphone and an Enter mark in the center,
  `Esc` on the right,
  and a down arrow and `max` at the bottom

#### Scenario: Hold and press read apart
- **WHEN** the application is open
- **THEN** `EXIT`, `high`, `clear`, `max` and the microphone stand white on a black plate,
  and the arrows, the Enter mark and `Esc` stand black on white

#### Scenario: Nothing clipped
- **WHEN** the application is open
- **THEN** every label is whole and stands clear of its neighbors

### Requirement: The status is static
The status SHALL read `Connected` whatever the device's Bluetooth state,
because nothing here uses Bluetooth.

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

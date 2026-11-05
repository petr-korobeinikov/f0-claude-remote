# app-screen Specification

## Purpose

Shows the remote's one screen.

## Requirements

### Requirement: The application carries its name
The device SHALL list the application as `Claude Remote`.

#### Scenario: Apps menu
- **WHEN** Apps → Bluetooth opens on the device after `mise run launch`
- **THEN** it lists `Claude Remote`

### Requirement: The application opens on its one screen
Starting the application SHALL show its screen across the whole display,
with no menu first and no system status bar.

#### Scenario: Start
- **WHEN** `mise run launch` runs
- **THEN** the device shows the screen, with no status bar above it

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

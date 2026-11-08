## MODIFIED Requirements

### Requirement: The status follows the link
The screen SHALL read `Connected` only while the Mac holds a paired link.
While the link is offered with none up, it SHALL show `No link` and `Reconnecting...` with no ring or legend,
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

### Requirement: Holding Back leaves the application
Holding Back for the device's long press SHALL end the application,
and a short press SHALL do nothing,
except on `D_stopped`, where Back's press ends it at once.

#### Scenario: Long press
- **WHEN** Back is held until the long press fires
- **THEN** the application ends and the device returns to the screen it was started from

#### Scenario: Short press
- **WHEN** Back is pressed and released before the long press fires, on any board but `D_stopped`
- **THEN** the application stays open and its screen does not change

#### Scenario: Stopped screen
- **WHEN** Back is pressed on `D_stopped`
- **THEN** the application ends at once, and the device returns to the screen it was started from

## ADDED Requirements

### Requirement: A stopped offer has a screen of its own
Once the application stops offering the link, whatever is held,
the screen SHALL show board `D_stopped` until a button is pressed:
`D_disconnected`'s status row, `Reconnect stopped`, `Any button: retry`,
and the Back mark with `exit` in a thin frame, as a tap.

#### Scenario: Stopped
- **WHEN** the application stops offering the link
- **THEN** a frame equals `D_stopped`

#### Scenario: Stopped with OK still down
- **WHEN** OK stays down on `D_disc_voice` until the application stops offering the link
- **THEN** a frame equals `D_stopped`

#### Scenario: Started over
- **WHEN** a button but Back is pressed on `D_stopped`
- **THEN** a frame equals `D_disconnected`

### Requirement: The backlight follows the offer
While the application holds or offers the link, the backlight SHALL stay on,
whatever the device's Backlight Time.
Once it stops offering the link, the backlight SHALL go off at once,
and the press that starts over SHALL bring it back.
Leaving the application SHALL hand the backlight back to the device's Backlight Time.

#### Scenario: Held on
- **WHEN** the link comes up after a press on `D_stopped`,
  and twice the device's Backlight Time passes with no press
- **THEN** the backlight is on

#### Scenario: Off on stopping
- **WHEN** the application stops offering the link
- **THEN** the backlight goes off with no press

#### Scenario: Leaving after starting over
- **WHEN** the application ends after a press on `D_stopped`,
  and twice the Backlight Time passes on the desktop with no press
- **THEN** the backlight is off

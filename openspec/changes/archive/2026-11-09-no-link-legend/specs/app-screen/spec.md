## MODIFIED Requirements

### Requirement: The status follows the link
The screen SHALL read `Connected` only while the Mac holds the link, which over Bluetooth means paired.
While the link is offered with none up, it SHALL show `No link` and `Reconnecting...` with no ring,
and `Keys are not sent` unless a voice was cut;
`D_disconnected` alone SHALL also show Right's hold for USB, on a black plate reading `Switch to` and the USB mark,
and a legend of `HOLD` alone, white on a black plate, ending the status row where `D_Main`'s legend ends.
`D_disc_voice` SHALL show no legend.
Every no-link board SHALL keep its two lines on the rows of `D_stopped`.

#### Scenario: Opened with no link
- **WHEN** the application opens with no Mac connected
- **THEN** a frame read off the device equals board `D_disconnected`

#### Scenario: The link drops
- **WHEN** the link drops on the idle screen
- **THEN** a frame equals `D_disconnected`, with no press

#### Scenario: The link drops during a flash
- **WHEN** the link drops while a flash runs
- **THEN** a frame equals `D_disconnected`, and stays equal past the flash's end

#### Scenario: Nothing jumps
- **WHEN** OK is released on `D_disc_voice`, or the offer stops on `D_disconnected`
- **THEN** the two lines stay on their rows

#### Scenario: The legend
- **WHEN** the owner looks at `D_disconnected` on the device
- **THEN** the status row ends in `HOLD`, white on a black plate, with no `TAP`

### Requirement: A stopped offer has a screen of its own
Once the application stops offering the link, whatever is held,
the screen SHALL show board `D_stopped` until a button is pressed:
`D_disconnected`'s status row with no legend, `Reconnect stopped`, `Any button: retry`,
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

### Requirement: USB has boards of its own
Once USB is taken, the status row SHALL show a USB mark where Bluetooth's boards show theirs,
`Connect the cable` in place of `Reconnecting...`,
and, on `D_usb_disconnected` alone, Right's hold for Bluetooth on a black plate reading `Switch to` and the Bluetooth mark,
with the legend of `D_disconnected`.

#### Scenario: Boards
- **WHEN** the application is on USB
- **THEN** `D_usb_main`, `D_usb_voice`, `D_usb_disconnected`, `D_usb_disc_voice` and `D_usb_stopped`
  stand in for `D_Main`, `D_voice`, `D_disconnected`, `D_disc_voice` and `D_stopped`

#### Scenario: A flash
- **WHEN** an action flashes on USB
- **THEN** its element flashes as on Bluetooth, under the USB mark

#### Scenario: On Bluetooth
- **WHEN** the application is on Bluetooth
- **THEN** no status row shows a USB mark

#### Scenario: The legend on USB
- **WHEN** the owner looks at `D_usb_disconnected` on the device
- **THEN** the status row ends in `HOLD`, white on a black plate, with no `TAP`

## MODIFIED Requirements

### Requirement: Installation offers each route
`docs/guide/installation.md` SHALL name what the app needs,
then offer the latest Release's `.fap` and the source, each with its update and removal.

#### Scenario: Routes
- **WHEN** the page is read
- **THEN** it names firmware 1.5.1, a computer taking a Bluetooth keyboard or, over a cable, a USB one,
  Claude Code 2.1.311 as tried,
  and for the source an Apple Silicon Mac with mise;
  the `.fap` goes into the SD card's `apps/Bluetooth/` through qFlipper,
  and the source runs `mise trust`, `mise install`, `mise run setup` and `mise run launch`

### Requirement: Using the remote covers every button and screen
`docs/guide/using-the-remote.md` SHALL give each button's tap and hold, what it sends and does in Claude Code,
the buzz and flash confirming it, and each screen.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it holds the nine rows of the keys table,
  and the main screen, `Listening`, `No link` with `Keys are not sent` and Right's hold for USB and back,
  `Voice stopped: release OK`,
  and `Reconnect stopped` with what a press does there

### Requirement: Troubleshooting maps symptoms to fixes
`docs/guide/troubleshooting.md` SHALL give each failure the specs and archived changes record
as symptom, cause and fix.

#### Scenario: Rows
- **WHEN** the page is read
- **THEN** it has a row for no `Verify code` over the open Apps browser, Connect timing out,
  a recording ending about 2.2 s in, a tap during `Voice: processing…`, `/clear` showing no change,
  `mise run launch` failing on a PIN-locked device, a press with no buzz,
  `No link` gone too soon for Right's hold on a paired Mac,
  and `mise run launch` finding no device on USB

## ADDED Requirements

### Requirement: USB is written down
`docs/guide/usb.md` SHALL say when USB can be taken, how, what the Mac asks, what USB changes, and how it returns.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it says USB is a fallback for a Mac with no Bluetooth link,
  that a paired Mac in range takes forgetting the device or turning Bluetooth off,
  the hold of Right on `No link`, the cable, the Mac's alert for a new accessory,
  that the keys and `keybindings.json` stay the same,
  that the device has no console until leaving USB,
  that the hold of Right on `No link` with the cable out returns to Bluetooth,
  and that the next launch opens on Bluetooth

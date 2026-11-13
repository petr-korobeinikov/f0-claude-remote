# user-guide Specification

## Purpose

Takes a user from nothing to a working remote through Home, Quick start and the Guide.

## Requirements

### Requirement: Home says what the app is
`docs/index.md` SHALL show `asset/screenshot/main.png`, the tagline, the README's Why and Features,
and lead to Quick start.

#### Scenario: Home
- **WHEN** the built Home page is read
- **THEN** it shows all four, and its first link is Quick start

### Requirement: Quick start reaches a dictated prompt
`docs/quick-start.md` SHALL take a reader to a prompt dictated and sent from the device,
each step linking its Guide page.

#### Scenario: Steps
- **WHEN** the page is read
- **THEN** it holds, in order, the install commands, pairing, the `keybindings.json` block with `/voice hold`,
  and a try: hold OK, speak, release, wait out `Voice: processing…`, tap OK

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

### Requirement: Pairing is written down
`docs/guide/pairing.md` SHALL hold the pairing steps, moved from the README, and what follows them:
reconnecting, pairing again, and the radio, held only while the app is on Bluetooth.

#### Scenario: Moved
- **WHEN** `README.md` is read
- **THEN** it holds none of the page's steps

#### Scenario: Steps
- **WHEN** the page is read
- **THEN** it holds, in order, `mise run launch` not over the open Apps browser, a press if `Reconnect stopped` shows,
  Connect on `Claude Remote` in the Mac's Bluetooth settings, OK in `Verify code`
  and closing Keyboard Setup Assistant

#### Scenario: After pairing
- **WHEN** the page is read
- **THEN** it says the Mac connects by itself within a second of a drop,
  the app reconnects with no press when it opens again,
  past a minute with no link the device needs a press on `Reconnect stopped`,
  and pairing again starts by forgetting the device on the Mac

#### Scenario: The radio
- **WHEN** the page is read
- **THEN** it says the app holds the radio while on Bluetooth, so the phone app's connection waits,
  and hands it back on taking USB or leaving, the device's Bluetooth then standing as its setting says

### Requirement: Claude Code setup gives the block
`docs/guide/claude-code-setup.md` SHALL give the `keybindings.json` block, where that file lives,
and what else dictation needs on the Mac: `/voice hold` and key repeat.

#### Scenario: Block
- **WHEN** the page's block is compared with the README's before this change
- **THEN** both bind the same four keys to the same four actions

### Requirement: Using the remote covers every button and screen
`docs/guide/using-the-remote.md` SHALL give each button's tap and hold, what it sends and does in Claude Code,
the buzz and flash confirming it, and each screen.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it holds the nine rows of the keys table,
  and the main screen, `Listening`, `No link` with `Keys are not sent` and Right's hold for USB and back,
  `Voice stopped: release OK`,
  and `Reconnect stopped` with what a press does there

### Requirement: Using the remote gives the dolphin's XP
`docs/guide/using-the-remote.md` SHALL give the XP each dictation and each `/clear` earn,
the Apps menu's for a launch, and the firmware's daily cap,
naming the firmware these were read from.

#### Scenario: The dolphin
- **WHEN** the page is read
- **THEN** it gives 1 XP a dictation, 10 a `/clear` and 2 a launch from the Apps menu,
  and 20 XP a day for the Flipper Apps Catalog's apps together, by firmware 1.5.1

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

### Requirement: The README leads to the site
`README.md` SHALL keep every section but Usage and Debugging, which move to the site,
and link the site, its Quick start, its Debugging and its Contributing.

#### Scenario: README
- **WHEN** `README.md` is read
- **THEN** it links those four pages, and has no `## Usage` or `## Debugging`

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

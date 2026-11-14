## MODIFIED Requirements

### Requirement: Using the remote covers every button and screen
`docs/guide/using-the-remote.md` SHALL give each button's tap and hold, what it sends and does in Claude Code,
the buzz and flash confirming it, and each screen, below its screenshot.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it holds the eleven rows of the keys table,
  what the left arrow does on an empty prompt,
  and the main screen, `Listening`, `No link` with `Keys are not sent` and Right's hold for USB and back,
  `Voice stopped: release OK`,
  and `Reconnect stopped` with what a press does there

#### Scenario: Screenshots
- **WHEN** the built page is read
- **THEN** its images are `main.png`, `voice-bt.png`, `no-link-bt.png`, `no-link-usb.png`, `voice-cut-bt.png`
  and `stopped-bt.png` from `asset/screenshot/`, in that order,
  each directly above the paragraph of the screen it shows

### Requirement: USB is written down
`docs/guide/usb.md` SHALL say when USB can be taken, how, what the Mac asks, what USB changes, and how it returns,
and show the main screen on USB.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it says USB is a fallback for a Mac with no Bluetooth link,
  that a paired Mac in range takes forgetting the device or turning Bluetooth off,
  the hold of Right on `No link`, the cable, the Mac's alert for a new accessory,
  that the keys and `keybindings.json` stay the same,
  that the device has no console until leaving USB,
  that the hold of Right on `No link` with the cable out returns to Bluetooth,
  and that the next launch opens on Bluetooth

#### Scenario: Screenshot
- **WHEN** the built page is read
- **THEN** its one image is `asset/screenshot/main-usb.png`, directly above the paragraph naming the status row's USB mark

## ADDED Requirements

### Requirement: The README shows every screen
`README.md` SHALL show `asset/screenshot/main.png` on top
and, in `## Screens` after `## Features`, the other six screenshots, two to a row, each 256 px wide.

#### Scenario: Screens
- **WHEN** `README.md` is read
- **THEN** `## Screens` follows `## Features` and shows, row by row,
  `main-usb.png` and `voice-bt.png`, `no-link-bt.png` and `no-link-usb.png`, `voice-cut-bt.png` and `stopped-bt.png`,
  each `width="256"` with alt text naming its screen

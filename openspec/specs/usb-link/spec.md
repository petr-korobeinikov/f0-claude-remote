# usb-link Specification

## Purpose

Makes the device the Mac's USB keyboard when Bluetooth has no link,
from a hold on the no-link screen until the return or leaving.

## Requirements

### Requirement: Holding Right on the no-link screen takes USB
Holding Right on `D_disconnected`, from a press made there, until the long press fires,
SHALL end the Bluetooth link and make the device a USB keyboard,
with no buzz, no flash and no key sent.

#### Scenario: Taken
- **WHEN** Right is held on `D_disconnected` with no cable
- **THEN** `D_usb_disconnected` shows at once

#### Scenario: A press made elsewhere
- **WHEN** Right goes down on `D_stopped` and stays down past the long press
- **THEN** a frame equals `D_disconnected`, on Bluetooth

#### Scenario: Another screen
- **WHEN** Right is held on `D_disc_voice`, `D_Main` or `D_voice`
- **THEN** the application stays on Bluetooth

#### Scenario: Opened again
- **WHEN** the application ends on USB and opens again
- **THEN** a frame equals `D_disconnected` or `D_Main`

### Requirement: On USB the application is a keyboard named Claude Remote
On USB, with the cable in, the Mac SHALL list the device as `Claude Remote`
on vendor ID `046D` and product ID `C529`,
and every button SHALL do what it does over Bluetooth.

#### Scenario: Name
- **WHEN** the cable is in on USB
- **THEN** `system_profiler SPUSBHostDataType` lists `Claude Remote` on vendor ID `046D` and product ID `C529`

#### Scenario: Buttons
- **WHEN** the owner runs a Claude Code session on USB, dictation included
- **THEN** the owner reports no difference from Bluetooth

### Requirement: On USB the status follows the cable
On USB, the link SHALL come up and drop with the cable, with no press.

#### Scenario: Cable in
- **WHEN** USB is taken with the cable in, or the cable connects later
- **THEN** `D_usb_main` shows

#### Scenario: Cable out
- **WHEN** the cable comes out
- **THEN** `D_usb_disconnected` shows

### Requirement: On USB the link is awaited a minute at a time
From the hold, a drop, or the press that starts over,
the application SHALL wait for the link for 60 s,
and then SHALL stop waiting until a button is pressed.

#### Scenario: A minute with no link
- **WHEN** 60 s pass on `D_usb_disconnected` with no press
- **THEN** `D_usb_stopped` shows, and the backlight goes off

#### Scenario: Counted afresh
- **WHEN** Right is held 20 s after `D_disconnected` first showed
- **THEN** `D_usb_stopped` shows 60 s after the hold

#### Scenario: A cable past the minute
- **WHEN** the cable connects on `D_usb_stopped`
- **THEN** the screen does not change until a button is pressed

#### Scenario: Started over
- **WHEN** a button but Back is pressed on `D_usb_stopped` with the cable in
- **THEN** `D_usb_main` shows with no other press

### Requirement: USB takes the device's console while it lasts
On USB the device SHALL offer no USB console, and leaving USB SHALL return it.

#### Scenario: On USB
- **WHEN** the application is on USB with the cable in
- **THEN** no `/dev/cu.usbmodemflip_*` exists, and `mise run launch` fails

#### Scenario: Leaving
- **WHEN** Back is held until the long press fires on USB
- **THEN** the application ends, and `mise run launch` exits 0

#### Scenario: Returning
- **WHEN** the application is back on Bluetooth with the cable in
- **THEN** `mise run launch` exits 0

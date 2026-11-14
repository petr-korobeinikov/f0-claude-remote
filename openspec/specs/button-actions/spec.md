# button-actions Specification

## Purpose

Turns each tap and hold of the device's buttons into one of the remote's actions,
which confirms itself on the device.

## Requirements

### Requirement: Voice lasts as long as OK is held
Holding OK SHALL start voice, and releasing it SHALL stop voice.
While voice is on, no other button SHALL act, exit included.

#### Scenario: Release
- **WHEN** OK is released after its long press fired
- **THEN** voice stops, once, and Enter does not act

#### Scenario: Another button during voice
- **WHEN** another button is tapped or held during voice
- **THEN** nothing acts, and voice stays on

#### Scenario: Buttons after voice
- **WHEN** voice has stopped and Back is held
- **THEN** exit acts

### Requirement: An action confirms itself with one buzz
Enter, the four arrows, Esc, effort high, effort max, clear and the start of voice
SHALL each vibrate the device once, for 100 ms.

#### Scenario: An action
- **WHEN** one of them acts
- **THEN** the device vibrates once

#### Scenario: The silent rest
- **WHEN** voice stops, exit acts, or Back is tapped
- **THEN** the device does not vibrate

### Requirement: A press acts with the backlight off
A button SHALL act the same whether or not the backlight is on.

#### Scenario: Backlight off
- **WHEN** the backlight is off on `D_stopped` and Up is tapped
- **THEN** a frame equals `D_disconnected`

### Requirement: With no link only holding Back or Right acts
With no link, holding Back SHALL exit,
holding Right on `D_disconnected` or `D_usb_disconnected` SHALL switch the link, as `usb-link` and `bluetooth-link` say,
and no other press SHALL act, vibrate or flash,
so a press with no buzz was not sent;
`D_stopped` alone answers a press otherwise, as its requirements say.

#### Scenario: Every other press
- **WHEN** the link is offered with none up, every button but Back and Right is tapped and held, and Back and Right are tapped
- **THEN** nothing acts, the device does not vibrate and the screen does not change

#### Scenario: Exit
- **WHEN** there is no link and Back is held until the long press fires
- **THEN** the application ends

#### Scenario: The link returns
- **WHEN** the link returns and Up is tapped
- **THEN** the up arrow acts

### Requirement: A lost link stops voice
A link lost during voice SHALL stop voice at once, with no buzz,
and voice SHALL stay off until OK is released and held again with the link up.

#### Scenario: Released after the drop
- **WHEN** the link drops during voice and OK is released
- **THEN** nothing acts, Enter included

#### Scenario: The link returns with OK still down
- **WHEN** the link drops during voice and returns with OK still down, and OK is released
- **THEN** voice stays off and nothing acts

#### Scenario: Held again
- **WHEN** OK was released after the drop and is held again with the link up
- **THEN** voice starts

#### Scenario: Exit with OK still down
- **WHEN** the link dropped during voice, OK stays down and Back is held until the long press fires
- **THEN** the application ends

### Requirement: Exit sends no key
Exit, and a press with no action, SHALL send no key.

#### Scenario: Exit
- **WHEN** exit acts
- **THEN** no key is sent

#### Scenario: A press with no action
- **WHEN** Back is tapped
- **THEN** no key is sent

### Requirement: A press on the stopped screen starts over
The first press of any button but Back on `D_stopped` SHALL offer the link again,
with no buzz, no flash and no key sent.

#### Scenario: Any button but Back
- **WHEN** Up, Down, Left, Right or OK is pressed on `D_stopped`
- **THEN** the device does not vibrate, a frame equals `D_disconnected`, and no key is sent

#### Scenario: OK held
- **WHEN** OK is held on `D_stopped` past the long press
- **THEN** voice does not start

### Requirement: A tap or hold acts on the screen it began on
A tap, or a hold other than Back's, SHALL act only if the screen, a flash aside, has not changed since its press.

#### Scenario: Started over on a cable
- **WHEN** Up is tapped, or OK, Up, Down or Left is held, on `D_usb_stopped` with the cable in
- **THEN** `D_usb_main` shows, the device does not vibrate, voice does not start, and no key is sent

#### Scenario: The link returns during a tap
- **WHEN** Up goes down on `D_disconnected`, and the link returns before its release
- **THEN** the device does not vibrate, and no key is sent

#### Scenario: Back held through a drop
- **WHEN** Back goes down on `D_Main`, and the link drops before its long press fires
- **THEN** the application ends

### Requirement: Each button acts on a tap or a hold
With the link up, a tap, released before the device's long press fires, and a hold, kept until it fires,
SHALL take the actions in the scenarios below, once each, as far as `A tap or hold acts on the screen it began on` allows,
and nothing else SHALL act.

#### Scenario: Tap
- **WHEN** Up, Down, Left, Right or OK is tapped
- **THEN** the up, down, left or right arrow, or Enter, acts, on the release

#### Scenario: Hold
- **WHEN** Up, Down, Left, Right or Back is held
- **THEN** effort high, effort max, clear, Esc or exit acts when the long press fires

#### Scenario: A hold does not repeat
- **WHEN** Up stays down for 2 seconds after its long press fired
- **THEN** effort high has acted once

#### Scenario: Nothing acts on the press
- **WHEN** a button goes down
- **THEN** nothing acts before its release or its long press

#### Scenario: A tap with no action
- **WHEN** Back is tapped
- **THEN** nothing acts

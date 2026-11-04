# button-actions Specification

## Purpose

Turns each tap and hold of the device's buttons into one of the remote's actions,
which confirms itself on the device and does not reach the Mac yet.

## Requirements

### Requirement: Each button takes its actions
A tap, released before the device's long press fires, and a hold, kept until it fires,
SHALL take the actions in the scenarios below, once each, and nothing else SHALL act.

#### Scenario: Tap
- **WHEN** Up, Down or OK is tapped
- **THEN** the up arrow, the down arrow or Enter acts, on the release

#### Scenario: Hold
- **WHEN** Up, Down, Left or Back is held
- **THEN** effort high, effort max, clear or exit acts when the long press fires

#### Scenario: A hold does not repeat
- **WHEN** Up stays down for 2 seconds after its long press fired
- **THEN** effort high has acted once

#### Scenario: Esc acts on the press
- **WHEN** Right goes down
- **THEN** Esc acts at once, and neither keeping Right down nor releasing it acts again

#### Scenario: A tap with no action
- **WHEN** Left or Back is tapped
- **THEN** nothing acts

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
Enter, both arrows, Esc, effort high, effort max, clear and the start of voice
SHALL each vibrate the device once, for 100 ms.

#### Scenario: An action
- **WHEN** one of them acts
- **THEN** the device vibrates once

#### Scenario: The silent rest
- **WHEN** voice stops, exit acts, or Left or Back is tapped
- **THEN** the device does not vibrate

### Requirement: A press acts with the backlight off
A button SHALL act the same whether or not the backlight is on.

#### Scenario: Backlight off
- **WHEN** the backlight has timed out and Up is tapped
- **THEN** the device vibrates once

### Requirement: No action reaches the Mac
No button SHALL send a keystroke.

#### Scenario: Text field on the Mac
- **WHEN** a text field is focused on the Mac and every button is tapped and held
- **THEN** nothing is typed in the field

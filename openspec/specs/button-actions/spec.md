# button-actions Specification

## Purpose

Turns each tap and hold of the device's buttons into one of the remote's actions,
which confirms itself on the device and does not reach the Mac yet.

## Requirements

### Requirement: Each button takes its actions
With the link up, a tap, released before the device's long press fires, and a hold, kept until it fires,
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
- **WHEN** the link is up, the backlight has timed out and Up is tapped
- **THEN** the device vibrates once

### Requirement: No action reaches the Mac
No button SHALL send a keystroke.

#### Scenario: Text field on the Mac
- **WHEN** a text field is focused on the Mac and every button is tapped and held
- **THEN** nothing is typed in the field

### Requirement: With no link only holding Back acts
With no link, holding Back SHALL exit, and no other press SHALL act, vibrate or flash,
so a press with no buzz was not sent.

#### Scenario: Every other press
- **WHEN** there is no link, every other button is tapped and held, and Back is tapped
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

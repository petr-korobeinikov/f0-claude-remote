## REMOVED Requirements

### Requirement: Each button takes its actions
**Reason**: Its scenario `Esc acts on the press` no longer holds, and the archive drops no scenario from a modified requirement.
**Migration**: `Each button acts on a tap or a hold` replaces it.

## ADDED Requirements

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

## MODIFIED Requirements

### Requirement: An action confirms itself with one buzz
Enter, the four arrows, Esc, effort high, effort max, clear and the start of voice
SHALL each vibrate the device once, for 100 ms.

#### Scenario: An action
- **WHEN** one of them acts
- **THEN** the device vibrates once

#### Scenario: The silent rest
- **WHEN** voice stops, exit acts, or Back is tapped
- **THEN** the device does not vibrate

### Requirement: Exit sends no key
Exit, and a press with no action, SHALL send no key.

#### Scenario: Exit
- **WHEN** exit acts
- **THEN** no key is sent

#### Scenario: A press with no action
- **WHEN** Back is tapped
- **THEN** no key is sent

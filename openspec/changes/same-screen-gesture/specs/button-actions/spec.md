## MODIFIED Requirements

### Requirement: Each button takes its actions
With the link up, a tap, released before the device's long press fires, and a hold, kept until it fires,
SHALL take the actions in the scenarios below, once each, as far as `A tap or hold acts on the screen it began on` allows,
and nothing else SHALL act.

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

## ADDED Requirements

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

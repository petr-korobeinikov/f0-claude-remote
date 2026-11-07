# Spec Delta

## Purpose

Earns the device's dolphin XP for the work done with the remote,
on top of what the firmware pays a launch by itself.

## ADDED Requirements

### Requirement: Dictations and /clear earn XP
With the link up, each dictation SHALL earn the device's dolphin 1 XP as voice starts,
and each `/clear` 10 XP as it acts,
as far as the firmware's daily cap allows.

#### Scenario: Dictation
- **WHEN** OK is held with the link up
- **THEN** the dolphin's XP rises by 1

#### Scenario: Clear
- **WHEN** Left is held with the link up
- **THEN** the dolphin's XP rises by 10

#### Scenario: A dictation ends
- **WHEN** OK is released after voice started, or the link drops during voice
- **THEN** the dolphin's XP rises no further

#### Scenario: No link
- **WHEN** there is no link and OK or Left is held
- **THEN** the dolphin's XP stays

### Requirement: Nothing else earns XP
The application SHALL earn the dolphin no XP of its own for any other action, nor for its launch.

#### Scenario: Every other action
- **WHEN** Enter, an arrow, Esc, effort high, effort max or exit acts
- **THEN** the dolphin's XP stays

#### Scenario: Launch
- **WHEN** `mise run launch` starts the application
- **THEN** the dolphin's XP stays

# Spec Delta

## Purpose

Explains why the app works as it does,
each fact marked as documented, read from sources a bump can change, or seen.

## ADDED Requirements

### Requirement: Architecture shows the layers and a press's path
`docs/internals/architecture.md` SHALL give the three layers and the rule that includes point down,
each source file's one task,
and the files a press passes through, with the queues between threads.

#### Scenario: Files
- **WHEN** the page is read beside `git ls-files src`
- **THEN** it names every file there once, with its task in one sentence

#### Scenario: Path
- **WHEN** the press's path is read
- **THEN** it runs `screen.c`, `app.c`, `keymap_input.c`, `keymap.c`, `actions.c`, `transport_bluetooth.c`,
  and names the input and link queues

### Requirement: Bluetooth explains the app's own profile
`docs/internals/bluetooth.md` SHALL explain the app's own HID profile:
its name within the 16 characters advertised, its own address and keys file,
pairing in the firmware's `Verify code` dialog, advertising after a drop, keys released on a drop,
the radio handed back on leaving, and why the open Apps browser blocks pairing.

#### Scenario: Marks
- **WHEN** each fact is read
- **THEN** it names its kind, documented, firmware 1.5.1's sources or seen,
  and links its source: the documentation page, the file and line at tag `1.5.1`, or the archived task

### Requirement: Keys explains the chords
`docs/internals/keys-in-claude-code.md` SHALL explain why each command is a Ctrl chord,
how Claude Code matches Ctrl under a non-Latin layout, why voice is Ctrl+Q held with key repeat,
why effort max sits on `ctrl+x ctrl+x`, what each effort level keeps, and what the schema warns of.

#### Scenario: Marks
- **WHEN** each fact is read
- **THEN** it names its kind, documented, Claude Code 2.1.311's package or seen,
  and links its source: the documentation page or the archived change

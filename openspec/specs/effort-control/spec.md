# effort-control Specification

## Purpose

Sets Claude Code's effort level from the device by holding a button,
whatever the Mac's keyboard layout and whatever the prompt holds.

## Requirements

### Requirement: Holding Up or Down sets the level
With the link up and the keys bound as `README.md` says,
holding Up SHALL set effort high and holding Down effort max
in the Claude Code session focused on the Mac.

#### Scenario: Effort high
- **WHEN** Up is held until the long press fires
- **THEN** Claude Code prints `Set effort level to high (saved as your default for new sessions)`

#### Scenario: Effort max
- **WHEN** Down is held until the long press fires
- **THEN** Claude Code prints `Set effort level to max (this session only)`

### Requirement: The Mac's layout does not matter
The level SHALL be set the same under a Latin and a non-Latin layout,
and no letter SHALL be typed.

#### Scenario: Non-Latin layout
- **WHEN** a non-Latin layout is active on the Mac and Up is held
- **THEN** Claude Code prints the same line, and the prompt gains no character

### Requirement: The prompt and the running work stay
Setting the level SHALL leave the prompt's text in place, unsent,
and SHALL not interrupt Claude's turn.

#### Scenario: A draft
- **WHEN** the prompt holds text and Up is held
- **THEN** the prompt holds the same text, and no message is sent

#### Scenario: Claude working
- **WHEN** Claude is working and Down is held
- **THEN** the level becomes max with no line printed, and the turn goes on

### Requirement: Each level is two chords and nothing more
Effort high SHALL send Ctrl+X then Ctrl+F,
and effort max Ctrl+X then Ctrl+Q,
each released before the next is pressed,
with no Enter and no other key.

#### Scenario: Effort high's keys
- **WHEN** effort high acts
- **THEN** the keys are, in order, Ctrl+X down and up, Ctrl+F down and up

#### Scenario: Effort max's keys
- **WHEN** effort max acts
- **THEN** the keys are, in order, Ctrl+X down and up, Ctrl+Q down and up

### Requirement: Nothing else reaches the Mac
Every other action SHALL send no key.

#### Scenario: Every other action
- **WHEN** Enter, an arrow, Esc, clear, the start or stop of voice, or exit acts
- **THEN** no key is sent

### Requirement: Unbound keys do no harm
Without the binding, holding Up or Down SHALL leave the prompt's text in place and send nothing.

#### Scenario: Keys not bound
- **WHEN** Ctrl+X then Ctrl+F arrive with no binding for them and the prompt holds text
- **THEN** the prompt holds the same text, no message is sent and the level does not change

### Requirement: The Mac needs one block of configuration
`README.md` SHALL give the block for `keybindings.json`,
and the Mac SHALL need nothing else installed or configured for effort.

#### Scenario: The block
- **WHEN** the block from `README.md` is the only addition to `keybindings.json`
- **THEN** `Effort high` and `Effort max` hold

# prompt-control Specification

## Purpose

Drives Claude Code's prompt from the device:
Enter, the arrows, Esc and `/clear`,
whatever the Mac's keyboard layout.

## Requirements

### Requirement: The plain actions do what their keys do on the Mac's keyboard
With the link up,
Enter, the arrows and Esc SHALL act
in the Claude Code session focused on the Mac,
under a Latin and a non-Latin layout,
with nothing configured on the Mac.

#### Scenario: Enter
- **WHEN** the prompt holds text and OK is tapped
- **THEN** the message is sent

#### Scenario: The arrows
- **WHEN** the prompt is empty after a sent message, Up is tapped, then Down
- **THEN** the prompt shows that message, then is empty again

#### Scenario: A menu
- **WHEN** Claude Code shows a menu, Down is tapped, then OK
- **THEN** the next option is picked

#### Scenario: The side arrows
- **WHEN** Claude Code shows the slider of `/effort` above its lowest level, Left is tapped, then Right
- **THEN** the slider moves one level down, then back

#### Scenario: Esc
- **WHEN** Claude is working and Right is held until the long press fires
- **THEN** the turn stops

### Requirement: Each plain action is one key
Enter, the four arrows and Esc
SHALL each press and release their key once,
with no modifier.

#### Scenario: The plain keys
- **WHEN** Enter, an arrow or Esc acts
- **THEN** the keys are, in order, that key down and up, and nothing else

### Requirement: Holding Left runs /clear
With the link up and the keys bound as `docs/guide/claude-code-setup.md` says,
holding Left SHALL run `/clear` in the Claude Code session focused on the Mac.

#### Scenario: Clear
- **WHEN** Left is held until the long press fires
- **THEN** a new conversation starts, and `/resume` lists the previous one

#### Scenario: Non-Latin layout
- **WHEN** a non-Latin layout is active on the Mac and Left is held
- **THEN** a new conversation starts, and the prompt gains no character

#### Scenario: A draft
- **WHEN** the prompt holds text and Left is held
- **THEN** the prompt holds the same text, and no message is sent

#### Scenario: Claude working
- **WHEN** Claude is working and Left is held
- **THEN** the turn goes on, and `/clear` runs when it ends

### Requirement: Clear is two chords and nothing more
Clear SHALL send Ctrl+X then Ctrl+L,
each released before the next is pressed,
with no Enter and no other key.

#### Scenario: Clear's keys
- **WHEN** clear acts
- **THEN** the keys are, in order, Ctrl+X down and up, Ctrl+L down and up

### Requirement: Unbound keys do no harm
Without the binding,
holding Left SHALL leave the prompt's text and the conversation in place.

#### Scenario: Keys not bound
- **WHEN** Ctrl+X then Ctrl+L arrive with no binding for them and the prompt holds text
- **THEN** the prompt holds the same text, no message is sent and the conversation stays

### Requirement: Clear needs one line of configuration
The block for `keybindings.json` in `docs/guide/claude-code-setup.md` SHALL hold the line for `/clear`.

#### Scenario: The block
- **WHEN** the block from `docs/guide/claude-code-setup.md` is the only addition to `keybindings.json`
- **THEN** `Clear` holds

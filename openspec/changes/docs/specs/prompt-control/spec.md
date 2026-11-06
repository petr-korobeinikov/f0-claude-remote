# Spec Delta

## MODIFIED Requirements

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

### Requirement: Clear needs one line of configuration
The block for `keybindings.json` in `docs/guide/claude-code-setup.md` SHALL hold the line for `/clear`.

#### Scenario: The block
- **WHEN** the block from `docs/guide/claude-code-setup.md` is the only addition to `keybindings.json`
- **THEN** `Clear` holds

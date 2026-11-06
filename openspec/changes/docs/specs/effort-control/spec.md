# Spec Delta

## MODIFIED Requirements

### Requirement: Holding Up or Down sets the level
With the link up and the keys bound as `docs/guide/claude-code-setup.md` says,
holding Up SHALL set effort high and holding Down effort max
in the Claude Code session focused on the Mac.

#### Scenario: Effort high
- **WHEN** Up is held until the long press fires
- **THEN** Claude Code prints `Set effort level to high (saved as your default for new sessions)`

#### Scenario: Effort max
- **WHEN** Down is held until the long press fires
- **THEN** Claude Code prints `Set effort level to max (this session only)`

### Requirement: The Mac needs one block of configuration
`docs/guide/claude-code-setup.md` SHALL give the block for `keybindings.json`,
and the Mac SHALL need nothing else installed or configured for effort.

#### Scenario: The block
- **WHEN** the block from `docs/guide/claude-code-setup.md` is the only addition to `keybindings.json`
- **THEN** `Effort high` and `Effort max` hold

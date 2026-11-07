# Spec Delta

## MODIFIED Requirements

### Requirement: Holding OK dictates
With the link up and the key bound as `docs/guide/claude-code-setup.md` says,
holding OK SHALL record in the Claude Code session focused on the Mac
until OK is released,
and SHALL send no message.

#### Scenario: Held
- **WHEN** OK is held for 10 seconds with the owner speaking
- **THEN** Claude Code's footer reads `listening…` until the release,
  and the words appear in the prompt

#### Scenario: Released
- **WHEN** OK is released
- **THEN** recording stops, the transcript stays in the prompt, and no message is sent

#### Scenario: Non-Latin layout
- **WHEN** a non-Latin layout is active on the Mac and OK is held
- **THEN** recording starts, and the prompt gains nothing but the words

### Requirement: Voice needs one line of configuration
The block for `keybindings.json` in `docs/guide/claude-code-setup.md` SHALL hold the line for voice,
and that page SHALL name what else dictation needs on the Mac:
dictation on in hold mode, and key repeat on.

#### Scenario: The block
- **WHEN** the block from `docs/guide/claude-code-setup.md` is the only addition to `keybindings.json`
- **THEN** `Held` holds

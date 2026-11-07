# voice-control Specification

## Purpose

Dictates into Claude Code's prompt from the device for as long as OK is held,
whatever the Mac's keyboard layout, and leaves the sending to Enter.

## Requirements

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

### Requirement: Voice is one key held down
The start of voice SHALL press Ctrl+Q and leave it down,
and the stop SHALL release it,
with no other key.

#### Scenario: Start's key
- **WHEN** voice starts
- **THEN** the keys are Ctrl+Q down, and nothing else

#### Scenario: Stop's key
- **WHEN** voice stops
- **THEN** the keys are Ctrl+Q up, and nothing else

### Requirement: A lost link releases the key
A link lost during voice SHALL release Ctrl+Q at once,
so that no later key carries it to the Mac.

#### Scenario: The drop
- **WHEN** the link drops during voice
- **THEN** the keys are Ctrl+Q up, once

#### Scenario: OK released after the drop
- **WHEN** the link dropped during voice and OK is released
- **THEN** no key is sent

#### Scenario: Any other link change
- **WHEN** the link drops with voice off, or the link returns
- **THEN** no key is sent

### Requirement: Unbound keys do no harm
Without the binding,
holding OK SHALL leave the prompt's text in place and start no recording.

#### Scenario: Keys not bound
- **WHEN** Ctrl+Q arrives with no binding for it and the prompt holds text
- **THEN** the prompt holds the same text, and no message is sent

### Requirement: Voice needs one line of configuration
The block for `keybindings.json` in `docs/guide/claude-code-setup.md` SHALL hold the line for voice,
and that page SHALL name what else dictation needs on the Mac:
dictation on in hold mode, and key repeat on.

#### Scenario: The block
- **WHEN** the block from `docs/guide/claude-code-setup.md` is the only addition to `keybindings.json`
- **THEN** `Held` holds

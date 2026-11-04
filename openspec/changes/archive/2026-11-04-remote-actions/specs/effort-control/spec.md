# Spec Delta

## MODIFIED Requirements

### Requirement: Each level is two chords and nothing more
Effort high SHALL send Ctrl+X then Ctrl+F,
and effort max Ctrl+X twice,
each released before the next is pressed,
with no Enter and no other key.

#### Scenario: Effort high's keys
- **WHEN** effort high acts
- **THEN** the keys are, in order, Ctrl+X down and up, Ctrl+F down and up

#### Scenario: Effort max's keys
- **WHEN** effort max acts
- **THEN** the keys are, in order, Ctrl+X down and up, Ctrl+X down and up

## REMOVED Requirements

### Requirement: Nothing else reaches the Mac
**Reason**: Enter, the arrows, Esc, clear and voice now send keys.
**Migration**: `prompt-control` and `voice-control` hold their keys, and `button-actions`' `Exit sends no key` holds the rest.

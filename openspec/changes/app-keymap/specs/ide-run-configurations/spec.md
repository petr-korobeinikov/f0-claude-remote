# Spec Delta

## MODIFIED Requirements

### Requirement: Run configurations ship with the repository
The repository SHALL carry the `Build`, `Launch`, `Debug` and `Test` configurations,
and CLion SHALL list them on open.

#### Scenario: First open
- **WHEN** CLion starts through `clion .` on a clone where `mise run setup` has run
- **THEN** the configuration list holds `Build`, `Launch`, `Debug` and `Test`

#### Scenario: Build from the IDE
- **WHEN** `Build` runs
- **THEN** it exits 0 and `dist/f0_claude_remote.fap` is current

#### Scenario: Test from the IDE
- **WHEN** `Test` runs
- **THEN** it exits 0 and its console shows the `ok` lines

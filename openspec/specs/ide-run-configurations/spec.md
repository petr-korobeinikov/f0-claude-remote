# ide-run-configurations Specification

## Purpose

Gives a fresh clone CLion's build, launch and debug commands,
with the only manual step being one profile selection.

## Requirements

### Requirement: Run configurations ship with the repository
The repository SHALL carry the `Build`, `Launch` and `Debug` configurations,
and CLion SHALL list them on open.

#### Scenario: First open
- **WHEN** CLion starts through `clion .` on a clone where `mise run setup` has run
- **THEN** the configuration list holds `Build`, `Launch` and `Debug`

#### Scenario: Build from the IDE
- **WHEN** `Build` runs
- **THEN** it exits 0 and `dist/f0_claude_remote.fap` is current

### Requirement: Setup prepares the IDE state without tracking it
`mise run setup` SHALL write the IDE files a first open needs,
leave any such file already present untouched, and leave the worktree clean.

#### Scenario: Fresh clone
- **WHEN** `mise run setup` finishes on a fresh clone
- **THEN** `.idea/misc.xml` and `.idea/debug-profiles.xml` exist and `git status --short` prints nothing

#### Scenario: Existing checkout
- **WHEN** `.idea/misc.xml` differs from its template and `mise run setup` runs
- **THEN** that file is unchanged

### Requirement: The debug profile needs one selection per clone
CLion SHALL offer the `Flipper` profile on first open,
and SHALL keep the selection once the user makes it.

#### Scenario: Selecting the profile
- **WHEN** the user selects `Flipper` and CLion restarts on the same clone
- **THEN** `Flipper` is still selected

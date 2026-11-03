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

### Requirement: A clone carries only the IDE files a first open needs
The repository SHALL track `.idea/misc.xml` and `.idea/debug-profiles.xml`,
and no other path under `.idea/`.
Each tracked file SHALL end with exactly one newline.

#### Scenario: Fresh clone
- **WHEN** `git ls-files .idea` runs on a fresh clone
- **THEN** it prints exactly `.idea/debug-profiles.xml` and `.idea/misc.xml`

#### Scenario: Final newline
- **WHEN** the last two bytes of each tracked `.idea` file are read
- **THEN** the last is a newline and the one before it is not

#### Scenario: CLion's private state
- **WHEN** `.idea/workspace.xml`, `.idea/vcs.xml` and `.idea/.gitignore` exist
- **THEN** `git status --short` lists none of them

### Requirement: CLion leaves the worktree clean
Opening a fresh clone in CLion, and selecting the debug profile there,
SHALL leave no change that `git status` reports.

#### Scenario: First open
- **WHEN** CLion has opened a clone where `mise run setup` has run, finished the database import and closed
- **THEN** `git status --short` prints nothing

#### Scenario: Profile selection
- **WHEN** the user selects `Flipper` and closes CLion
- **THEN** `git status --short` prints nothing

### Requirement: The debug profile needs one selection per clone
CLion SHALL offer the `Flipper` profile on first open,
and SHALL keep the selection once the user makes it.

#### Scenario: Selecting the profile
- **WHEN** the user selects `Flipper` and CLion restarts on the same clone
- **THEN** `Flipper` is still selected

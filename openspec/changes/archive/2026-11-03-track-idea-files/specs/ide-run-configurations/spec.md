# Spec Delta

## ADDED Requirements

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

## REMOVED Requirements

### Requirement: Setup prepares the IDE state without tracking it
**Reason**: The IDE files a first open needs are now tracked, so `setup` has nothing to copy.
**Migration**: `git clone` delivers `.idea/misc.xml` and `.idea/debug-profiles.xml`; an existing checkout gets them on its next pull, replacing its local copies.

# ide-navigation Specification

## Purpose

Lets CLion understand the Flipper sources from the build's own compile commands,
with no manual toolchain setup on any machine.

## Requirements

### Requirement: The compilation database sits at the root
A build SHALL leave a current compilation database reachable as `compile_commands.json` at the root,
naming the real source paths and the compiler by absolute path.

#### Scenario: Database content
- **WHEN** a build has run
- **THEN** `compile_commands.json` lists `<root>/f0_claude_remote.c` with a compiler under `<root>/.ufbt/toolchain/arm64-darwin/bin`

#### Scenario: New source file
- **WHEN** a `.c` file is added at the root and `mise exec -- ufbt` runs
- **THEN** the database lists it with no further command

### Requirement: Generated headers exist before the IDE opens
`mise run setup` SHALL generate the headers the sources include,
so the first open shows no unresolved include.

#### Scenario: Icons header
- **WHEN** `mise run setup` finishes
- **THEN** `f0_claude_remote_icons.h` exists under `.ufbt/build/f0_claude_remote`

### Requirement: CLion navigates with no manual setup
CLion SHALL resolve the SDK from the database alone,
with no setting changed by hand and no dependence on how it was started.

#### Scenario: Started from the project shell
- **WHEN** CLion starts through `clion .` on a clone where `mise run setup` has run and CLion has never opened
- **THEN** go-to-declaration on an SDK include reaches `.ufbt/current/sdk_headers` and `f0_claude_remote.c` shows no error

#### Scenario: Started from Toolbox
- **WHEN** CLion starts from Toolbox on the same clone
- **THEN** the same navigation holds

# developer-guide Specification

## Purpose

Takes a developer from a clone to a breakpoint on the device:
the toolchain, the build, the host tests, CLion, the probe and a session.

## Requirements

### Requirement: Toolchain says what a clone installs
`docs/development/toolchain.md` SHALL say what `mise install` and `mise run setup` put where,
how ufbt keeps to its own Python and the SDK to the project,
and how the SDK and a tool are bumped.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it names mise's store in the home folder, `.ufbt/`, `dist/`, `FLIPPER_SDK_VERSION`,
  `setup`'s sandbox and its macOS-only reach, and why `ufbt vscode_dist` never runs

### Requirement: Build and run gives the daily commands
`docs/development/build-and-run.md` SHALL give `mise run build`, `mise run launch` with what it needs of the device,
and `mise exec -- ufbt lint`, and say why `ufbt flash` is no launch.

#### Scenario: As written
- **WHEN** the page's build and lint commands run in a set-up clone
- **THEN** both exit 0

### Requirement: Host tests says how the core is tested
`docs/development/host-tests.md` SHALL give `mise run test` and what it compiles with,
how a test is written against `test/test.h` and the fake transport,
and what only the device can check.

#### Scenario: As written
- **WHEN** `mise run test` runs as the page gives it
- **THEN** it prints one `ok` line per test and exits 0

### Requirement: CLion opens a clone with no plugin
`docs/development/clion.md` SHALL take a set-up clone to navigation and the run configurations:
`clion .`, the compilation database, the two tracked `.idea` files, and `Flipper` picked once.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it names `Build`, `Launch`, `Debug` and `Test`, `.idea/misc.xml`, `.idea/debug-profiles.xml` and `Flipper`

### Requirement: Development holds the moved lines
The six pages SHALL hold `CONTRIBUTING.md`'s opening and `## Toolchain`
and the README's `## Debugging`, as they stand before this change:
`### The probe` and the board's risks on Debug probe, the rest of `## Debugging` on Debugging.

#### Scenario: Moved
- **WHEN** each line of those sections is searched in the six pages
- **THEN** every line is found but headings and the links the move rewrites

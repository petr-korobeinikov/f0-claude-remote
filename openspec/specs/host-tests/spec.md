# host-tests Specification

## Purpose

Proves the remote's logic on the Mac, with no device and no Flipper SDK,
so a failure names the broken rule before anything reaches the hardware.

## Requirements

### Requirement: A task runs the tests on the host
`mise run test` SHALL build every suite under `test/` with the system `cc`,
Flipper's warnings as errors and the address and undefined-behavior sanitizers,
and run it. It is the one task that needs a compiler from outside the project.

#### Scenario: Green
- **WHEN** `mise run test` runs with no device attached
- **THEN** it exits 0 and prints one `ok <name>` line per test

#### Scenario: A failing test
- **WHEN** an assertion fails and the output is piped
- **THEN** the task exits non-zero, names the assertion's file and line,
  and the `ok` lines before it are all there

### Requirement: The logic needs no SDK
The sources under test, all of `src/core/`, SHALL include no Flipper header.

#### Scenario: Includes
- **WHEN** `git grep -E '#include <(furi|gui|input|notification)' -- src/core` runs
- **THEN** it prints nothing

### Requirement: Tests stay out of the package
`test/` SHALL build into no package, and SHALL stay under the linter.

#### Scenario: Package
- **WHEN** `mise run build` runs
- **THEN** `compile_commands.json` lists no path under `test/`

#### Scenario: Lint
- **WHEN** a `.c` file under `test/` breaks Flipper's format and `mise exec -- ufbt lint` runs
- **THEN** it exits non-zero and names that file

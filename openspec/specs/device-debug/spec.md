# device-debug Specification

## Purpose

Debugs the application on the Flipper through the Black Magic probe,
from CLion or from a terminal, stepping through our code and over the firmware's.

## Requirements

### Requirement: CLion debugs the application on the device
CLion SHALL connect to the running device through the probe,
stop when an application loads, then honor breakpoints set in our sources.

#### Scenario: Breakpoint before the application starts
- **WHEN** `Debug` connects, a breakpoint sits in `f0_claude_remote.c`, `Launch` runs and the load stop is resumed
- **THEN** CLion stops on that line and shows its local variables

#### Scenario: Stepping over a firmware call
- **WHEN** Step Over runs on a line calling the firmware
- **THEN** CLion stops on the next line of our source

#### Scenario: Load stop without firmware sources
- **WHEN** an application loads during a session
- **THEN** CLion stops in disassembly with the firmware's function name, and no source file is needed

### Requirement: A terminal debugs the application on the device
`mise run debug` SHALL open GDB on the device in the terminal, with the same symbols.

#### Scenario: Firmware matches its symbols
- **WHEN** `mise run debug` starts with the device running the pinned firmware
- **THEN** every section `compare-sections` reports reads `matched`

#### Scenario: Leaving the terminal session
- **WHEN** the user types `quit`
- **THEN** the task exits 0, the device keeps running and an application started later runs normally

### Requirement: The probe is found wherever it is reachable
A session SHALL find the probe by itself, over USB first and over Wi-Fi otherwise,
with no address stored in the repository.

#### Scenario: Another USB port
- **WHEN** the probe's cable moves to another port of the Mac
- **THEN** `Debug` connects with no setting changed

#### Scenario: Wi-Fi
- **WHEN** the probe has no USB cable and is on the Mac's network
- **THEN** `Debug` connects through `blackmagic.local`

### Requirement: A finished session leaves the device safe
Ending a CLion session SHALL leave the device running,
with no debug state that faults a later application start.

#### Scenario: After Stop
- **WHEN** Stop ends a CLion session
- **THEN** `furi_hal_debug_gdb_session_active` reads `false` on the next attach and the program counter lies in flash

#### Scenario: Starting the application afterward
- **WHEN** `mise run launch` runs after Stop, with no debugger attached
- **THEN** it exits 0 and the device's USB console still answers

### Requirement: Debugging stays inside the project
GDB SHALL run on the toolchain's interpreter, without the user's Python packages,
and write nothing outside the project and temp.

#### Scenario: User site in CLion's GDB
- **WHEN** a CLion session evaluates `python import site; print(site.ENABLE_USER_SITE)`
- **THEN** it prints `False`

#### Scenario: Terminal session under denial
- **WHEN** `ufbt blackmagic` runs with writes denied outside the project and temp, under a throwaway `HOME`
- **THEN** that `HOME` stays empty

### Requirement: The debugging steps are written down
`README.md` SHALL state the device's preconditions, the probe's setup and the steps of a session,
as commands and menu paths that work as written.

#### Scenario: Following the README
- **WHEN** a reader starts from a powered-off device and follows the section in order
- **THEN** CLion stops on a breakpoint in `f0_claude_remote.c`

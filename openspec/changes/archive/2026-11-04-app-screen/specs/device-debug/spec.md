# Spec Delta

## MODIFIED Requirements

### Requirement: CLion debugs the application on the device
CLion SHALL connect to the running device through the probe,
stop when an application loads, then honor breakpoints set in our sources.

#### Scenario: Breakpoint before the application starts
- **WHEN** `Debug` connects, a breakpoint sits in `app.c`, `Launch` runs and the load stop is resumed
- **THEN** CLion stops on that line and shows its local variables

#### Scenario: Stepping over a firmware call
- **WHEN** Step Over runs on a line calling the firmware
- **THEN** CLion stops on the next line of our source

#### Scenario: Load stop without firmware sources
- **WHEN** an application loads during a session
- **THEN** CLion stops in disassembly with the firmware's function name, and no source file is needed

### Requirement: The debugging steps are written down
`README.md` SHALL state the device's preconditions, the probe's setup and the steps of a session,
as commands and menu paths that work as written.

#### Scenario: Following the README
- **WHEN** a reader starts from a powered-off device and follows the section in order
- **THEN** CLion stops on a breakpoint in `app.c`

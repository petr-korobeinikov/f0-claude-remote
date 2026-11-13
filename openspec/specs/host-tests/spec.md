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

### Requirement: The core decides when the wait is counted
What the 60 s of `bluetooth-link` and `usb-link` are counted from, and what ends the count,
SHALL be decided in `src/core/`, which answers every event with what the count does:
it starts, it stops, or it stays as it is.
The keymap's suite SHALL pin that answer,
and the platform SHALL start and stop its timer by it alone, keeping no count of its own.

#### Scenario: Started
- **WHEN** the application opens, the link drops, a press starts over on the stopped screen, or Right's hold switches the link
- **THEN** the core answers that the count starts

#### Scenario: Stopped
- **WHEN** the link comes up, or the 60 s end
- **THEN** the core answers that the count stops

#### Scenario: Left as it is
- **WHEN** any other event comes:
  a button event with no link, a drop reported again, OK released after a cut,
  or an event with the link up or on the stopped screen that starts nothing over
- **THEN** the core answers that the count stays as it is

#### Scenario: Nothing counted in the platform
- **WHEN** `git grep -n 'wait_ms[^(]' -- src/platform` runs
- **THEN** it prints nothing

### Requirement: Every mode is asked how long it waits
The core SHALL say how long each mode waits for a link in a `switch` over the modes,
so `mise run test` fails at a mode nobody has asked.

#### Scenario: A mode not asked
- **WHEN** an enumerator is added to the enum in `src/core/mode.h`,
  and to no switch but the one that says whether a mode keeps the display lit,
  and `mise run test` runs
- **THEN** it exits non-zero and names that enumerator at the switch that says how long a mode waits

### Requirement: The core decides when the display is lit
Whether the backlight of `app-screen`'s `The backlight follows the offer` is on SHALL be decided in `src/core/`,
which says of each mode, in a `switch` over the modes, whether it keeps the display lit.
A suite of its own SHALL pin that answer for every mode,
and the platform SHALL light and darken the display by it alone, at the opening as after it,
naming no action for the light.

#### Scenario: Lit
- **WHEN** the mode holds the link or offers it: idle, voice, no link, or a voice cut by the link
- **THEN** the core answers that the display is lit

#### Scenario: Dark
- **WHEN** the mode is the one of a stopped offer
- **THEN** the core answers that the display is dark

#### Scenario: No action named for the light
- **WHEN** `git grep -n -E 'ActionGiveUp|ActionStartOver' -- src/platform` runs
- **THEN** it prints nothing

#### Scenario: The opening
- **WHEN** `git grep -n 'display_lit' -- src/platform` runs
- **THEN** it prints two lines of `app.c`: the one that opens the backlight, and the one that sets it after an event

#### Scenario: A mode not asked
- **WHEN** an enumerator is added to the enum in `src/core/mode.h`, and to no switch, and `mise run test` runs
- **THEN** it exits non-zero and names that enumerator at the switch that says whether a mode keeps the display lit

### Requirement: The core decides which link is in use
Which of the links of `bluetooth-link` and `usb-link` the remote is on SHALL be decided in `src/core/` by the keymap,
which answers it after every event, and SHALL be named there by no action and by no call on the transport.
The keymap's suite SHALL pin when the link moves and when it stays,
and the platform SHALL put the transport on that link, as it shows it on the screen,
at the opening as after an event.

#### Scenario: Moved
- **WHEN** Right's hold fires on the no-link screen, from a press made there
- **THEN** the core answers the other link, and no action

#### Scenario: Stayed
- **WHEN** any other event comes:
  another button's event on the no-link screen, Right tapped there,
  its hold fired there from no press or from one made on another screen,
  Right held on another screen, or a change of mode
- **THEN** the core answers the link it held

#### Scenario: Named by no action and no call
- **WHEN** `git grep -l -E 'Usb|Bluetooth' -- src/core` runs
- **THEN** it prints `src/core/keymap.c` and `src/core/link.h` alone

#### Scenario: The platform follows
- **WHEN** `git grep -n 'keymap_link' -- src/platform` runs
- **THEN** it prints four lines of `app.c`:
  the two that open the screen and the transport, and the two that set them after an event

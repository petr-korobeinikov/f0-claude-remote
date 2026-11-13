## ADDED Requirements

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
- **WHEN** `git grep -n -E 'ActionUse|wait_ms[^(]' -- src/platform` runs
- **THEN** it prints nothing

### Requirement: Every mode is asked how long it waits
The core SHALL say how long each mode waits for a link in a `switch` over the modes,
so `mise run test` fails at a mode nobody has asked.

#### Scenario: A mode not asked
- **WHEN** an enumerator is added to the enum in `src/core/mode.h`, and to no switch, and `mise run test` runs
- **THEN** it exits non-zero and names that enumerator at the switch that says how long a mode waits

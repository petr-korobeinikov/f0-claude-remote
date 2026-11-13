## ADDED Requirements

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

## MODIFIED Requirements

### Requirement: Every mode is asked how long it waits
The core SHALL say how long each mode waits for a link in a `switch` over the modes,
so `mise run test` fails at a mode nobody has asked.

#### Scenario: A mode not asked
- **WHEN** an enumerator is added to the enum in `src/core/mode.h`,
  and to no switch but the one that says whether a mode keeps the display lit,
  and `mise run test` runs
- **THEN** it exits non-zero and names that enumerator at the switch that says how long a mode waits

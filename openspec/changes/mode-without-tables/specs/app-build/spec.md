## ADDED Requirements

### Requirement: No table decides for a mode or an action
An enum a `switch` is over SHALL index no table,
where a row left out reads as zero with no warning:
`mise run build` SHALL fail at a mode that a link report has no answer for.

#### Scenario: No table over a mode or an action
- **WHEN** `git grep -n -E '\[(Mode|Action)[A-Z]' -- src` runs
- **THEN** it prints nothing

#### Scenario: A new mode
- **WHEN** an enumerator is added to the enum in `src/core/mode.h`, and to no switch, and `mise run build` runs
- **THEN** it exits non-zero and names that enumerator at the switch that answers a link report

### Requirement: A table is as long as its enum
A table indexed by an enum SHALL take its length from the enum's `…MAX`, never from its rows,
so no value of the enum reads past the table's end.

#### Scenario: No table sized by its rows
- **WHEN** `git grep -n '\[\] = {' -- src` runs
- **THEN** it prints nothing

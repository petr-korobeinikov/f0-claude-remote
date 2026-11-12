## ADDED Requirements

### Requirement: An undecided enumerator stops the build
A `switch` over an enum SHALL name each of its enumerators and carry no `default:`,
so `mise run build` fails at an enumerator a switch leaves out,
and no package ships one that no switch has decided.

#### Scenario: No default
- **WHEN** `git grep -n 'default:' -- src test` runs
- **THEN** it prints nothing

#### Scenario: A new action
- **WHEN** an enumerator is added to the enum in `src/core/action.h`, and to no switch, and `mise run build` runs
- **THEN** it exits non-zero and names that enumerator

# Spec Delta

## MODIFIED Requirements

### Requirement: The logic needs no SDK
The sources under test, all of `src/core/`, SHALL include no Flipper header.

#### Scenario: Includes
- **WHEN** `git grep -E '#include <(furi|gui|input|notification)' -- src/core` runs
- **THEN** it prints nothing

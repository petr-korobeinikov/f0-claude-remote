# Spec Delta

## MODIFIED Requirements

### Requirement: CLion navigates with no manual setup
CLion SHALL resolve the SDK from the database alone,
with no setting changed by hand and no dependence on how it was started.

#### Scenario: Started from the project shell
- **WHEN** CLion starts through `clion .` on a clone where `mise run setup` has run and CLion has never opened
- **THEN** go-to-declaration on an SDK include reaches `.ufbt/current/sdk_headers` and `f0_claude_remote.c` shows no error

#### Scenario: Started from Toolbox
- **WHEN** CLion starts from Toolbox on the same clone
- **THEN** the same navigation holds

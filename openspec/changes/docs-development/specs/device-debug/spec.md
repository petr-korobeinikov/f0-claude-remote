# Spec Delta

## REMOVED Requirements

### Requirement: The debugging steps are written down
**Reason**: The steps leave `README.md` for the site.
**Migration**: `The debugging steps are on the site` holds them, unchanged.

## ADDED Requirements

### Requirement: The debugging steps are on the site
`docs/development/debug-probe.md` and `docs/development/debugging.md` SHALL state the device's preconditions,
the probe's setup and the steps of a session,
as commands and menu paths that work as written.

#### Scenario: Following the pages
- **WHEN** a reader starts from a powered-off device and follows `docs/development/debugging.md` in order
- **THEN** CLion stops on a breakpoint in `app.c`

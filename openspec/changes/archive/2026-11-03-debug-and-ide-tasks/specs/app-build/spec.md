# Spec Delta

## ADDED Requirements

### Requirement: Tasks build the application and start it on the device
`mise run build` SHALL build the application,
and `mise run launch` SHALL build it, upload it to the device and start it.

#### Scenario: Build
- **WHEN** `mise run build` runs
- **THEN** it exits 0 and `dist/f0_claude_remote.fap` exists

#### Scenario: Launch
- **WHEN** `mise run launch` runs with the device unlocked on USB
- **THEN** it exits 0 and its log reads `Launching app: /ext/apps/Examples/f0_claude_remote.fap`

#### Scenario: Device locked by PIN
- **WHEN** `mise run launch` runs while a PIN locks the device, which drops its USB console
- **THEN** it exits non-zero

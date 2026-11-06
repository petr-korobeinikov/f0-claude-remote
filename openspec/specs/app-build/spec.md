# app-build Specification

## Purpose

Builds the Flipper application from a pinned SDK,
on a machine that holds nothing but mise and a clone of the repository.

## Requirements

### Requirement: The SDK version is pinned
Setup SHALL deploy the SDK version pinned in `mise.toml`,
whatever the update server's release channel offers that day.

#### Scenario: Pinned version deployed
- **WHEN** `mise run setup` finishes
- **THEN** `mise exec -- ufbt status version` prints the value of `FLIPPER_SDK_VERSION`

#### Scenario: Wrong version refused
- **WHEN** an SDK other than the pinned one is already deployed
- **THEN** `mise run setup` replaces it or exits non-zero

### Requirement: The toolchain needs no system prerequisite
The toolchain SHALL run natively on Apple Silicon,
with no Rosetta and nothing installed outside the project.

#### Scenario: Build without Rosetta
- **WHEN** a build runs on arm64 macOS with `/Library/Apple/usr/libexec/oah/libRosettaRuntime` absent
- **THEN** it exits 0 using `.ufbt/toolchain/arm64-darwin`

### Requirement: A build yields the application package
Building from the repository root SHALL produce a package for hardware target 7
against the pinned SDK's API.

#### Scenario: Successful build
- **WHEN** `mise exec -- ufbt` runs at the root
- **THEN** it exits 0, `dist/f0_claude_remote.fap` exists and the log reads `Target: 7, API: 88.2`

### Requirement: The worktree stays clean
Setup, build and lint SHALL change no tracked file and leave no untracked one.

#### Scenario: After setup, build and lint
- **WHEN** `mise run setup`, `mise exec -- ufbt` and `mise exec -- ufbt lint` have run on a fresh clone
- **THEN** `git status --short` prints nothing

### Requirement: The sources follow Flipper's format
The repository SHALL carry Flipper's `.clang-format` from the pinned SDK,
and the sources SHALL pass the SDK's linter against it.

#### Scenario: Lint
- **WHEN** `mise exec -- ufbt lint` runs
- **THEN** it exits 0

### Requirement: Tasks build the application and start it on the device
`mise run build` SHALL build the application,
and `mise run launch` SHALL build it, upload it to the device and start it.

#### Scenario: Build
- **WHEN** `mise run build` runs
- **THEN** it exits 0 and `dist/f0_claude_remote.fap` exists

#### Scenario: Launch
- **WHEN** `mise run launch` runs with the device unlocked on USB
- **THEN** it exits 0 and its log reads `Launching app: /ext/apps/Bluetooth/f0_claude_remote.fap`

#### Scenario: Device locked by PIN
- **WHEN** `mise run launch` runs while a PIN locks the device, which drops its USB console
- **THEN** it exits non-zero

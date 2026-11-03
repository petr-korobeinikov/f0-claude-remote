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

### Requirement: The template's workflow stays pinned
The repository SHALL keep the app template's workflow,
with every action pinned to a full commit, a read-only token and the audited ufbt.

#### Scenario: Static check
- **WHEN** `mise exec -- actionlint .github/workflows/build.yml` runs
- **THEN** it exits 0

#### Scenario: Pinned references
- **WHEN** the workflow's `uses:` lines are read
- **THEN** each names a 40-hex commit

#### Scenario: Read-only token
- **WHEN** the workflow's `permissions` is read
- **THEN** it grants `contents: read` alone

#### Scenario: Audited ufbt
- **WHEN** the build step's inputs are read
- **THEN** `ufbt-version` is `==0.2.6`

### Requirement: Local workflow runs stay contained
`mise run check-workflow` SHALL run the workflow's jobs with no Docker socket,
no host network, no GitHub token and nothing written to the real `HOME`.

#### Scenario: Contained job
- **WHEN** a job container is inspected during a run
- **THEN** it mounts no Docker socket and its network is `bridge`

#### Scenario: No credential
- **WHEN** `.mise/tasks/check-workflow` is read
- **THEN** it passes an empty `GITHUB_TOKEN` secret, so `act` does not call `gh auth token`

#### Scenario: Local outcome
- **WHEN** it runs with a Docker daemon up
- **THEN** both jobs end `Job succeeded`, with the upload step skipped

# Spec Delta

## MODIFIED Requirements

### Requirement: The jobs run the local checks
Three parallel jobs on Linux SHALL run the host tests;
the SDK's deploy, the build and lint;
and the hooks and the workflow linter over the whole tree, both running shellcheck,
each step a command that runs the same on the Mac.

#### Scenario: Commands
- **WHEN** the jobs' steps are read
- **THEN** `test` runs `mise run test`;
  `build` deploys the SDK at `FLIPPER_SDK_VERSION` with `ufbt update --branch`, then runs `mise run build` and `mise exec -- ufbt lint`;
  `hygiene` installs `aqua:koalaman/shellcheck`, then runs `mise exec -- prek run --all-files` and `mise exec -- actionlint -pyflakes=`

#### Scenario: Shell checked
- **WHEN** a `.sh` file, an executable task under `.mise/tasks/` or a workflow's `run` step holds `echo $1`
- **THEN** the `shellcheck` hook fails on either file, and actionlint on the step, each naming SC2086

#### Scenario: Pinned SDK
- **WHEN** the `build` job's log is read
- **THEN** it reads `Target: 7, API: 88.2`

#### Scenario: Unbuilt checkout
- **WHEN** `mise exec -- prek run --all-files` runs in a checkout that has not built
- **THEN** it exits 0

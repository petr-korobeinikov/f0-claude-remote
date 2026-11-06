# ci Specification

## Purpose

Checks every push to a base branch and every pull request on GitHub,
with the commands a developer runs and inputs pinned by the commit,
and runs the same jobs locally, contained.

## Requirements

### Requirement: Pushes and pull requests are checked
`ci.yml` SHALL run on each push to `develop` or `main` and on each pull request,
and on nothing else.

#### Scenario: Triggers
- **WHEN** the workflow's `on` is read
- **THEN** it holds `push`, limited to `develop` and `main`, and `pull_request`

#### Scenario: On GitHub
- **WHEN** a commit whose jobs pass under `check-workflow` is pushed to `develop`
- **THEN** its `ci` run ends with `test`, `build` and `hygiene` succeeded

### Requirement: The jobs run the local checks
Three parallel jobs on Linux SHALL run the host tests;
the SDK's deploy, the build and lint;
and the hooks and the workflow linter over the whole tree,
each step a command that runs the same on the Mac.

#### Scenario: Commands
- **WHEN** the jobs' `run` steps are read
- **THEN** `test` runs `mise run test`;
  `build` deploys the SDK at `FLIPPER_SDK_VERSION` with `ufbt update --branch`, then runs `mise run build` and `mise exec -- ufbt lint`;
  `hygiene` runs `mise exec -- prek run --all-files` and `mise exec -- actionlint -shellcheck= -pyflakes=`

#### Scenario: Pinned SDK
- **WHEN** the `build` job's log is read
- **THEN** it reads `Target: 7, API: 88.2`

#### Scenario: Unbuilt checkout
- **WHEN** `mise exec -- prek run --all-files` runs in a checkout that has not built
- **THEN** it exits 0

### Requirement: A run's inputs are pinned
A run SHALL fetch actions by full commit, mise at one version,
tools only as `mise.lock` records them and the SDK at `FLIPPER_SDK_VERSION`,
and SHALL restore no cache.
The runner image, with its `cc`, stays unpinned.

#### Scenario: Pinned references
- **WHEN** the workflow's `uses:` lines are read
- **THEN** each names a 40-hex commit

#### Scenario: Pinned mise, no cache
- **WHEN** each `jdx/mise-action` step is read
- **THEN** it sets `version` to `2026.11.2` and `cache` to `false`, and no step uses `actions/cache`

#### Scenario: Locked tools
- **WHEN** a job's log is read
- **THEN** tools install in its mise setup step alone, through `mise install --locked`

### Requirement: The token is read-only
A run SHALL hold no write permission and leave no credential in its checkout.

#### Scenario: Token and checkout
- **WHEN** the workflow is read
- **THEN** `permissions` grants `contents: read` alone, and each checkout sets `persist-credentials` to `false`

### Requirement: Local workflow runs stay contained
`mise run check-workflow` SHALL run `ci.yml`'s jobs with no Docker socket,
no host network, no GitHub token and nothing written to the real `HOME`.

#### Scenario: Contained job
- **WHEN** a job container is inspected during a run
- **THEN** it mounts no Docker socket and its network is `bridge`

#### Scenario: No credential
- **WHEN** `.mise/tasks/check-workflow` is read
- **THEN** it passes an empty `GITHUB_TOKEN` secret, so `act` does not call `gh auth token`

#### Scenario: Local outcome
- **WHEN** it runs with a Docker daemon up
- **THEN** `test`, `build` and `hygiene` each end `Job succeeded`

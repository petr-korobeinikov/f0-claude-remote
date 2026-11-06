# Spec Delta

## MODIFIED Requirements

### Requirement: The jobs run the local checks
Four parallel jobs on Linux SHALL run the host tests;
the SDK's deploy, the build and lint;
the hooks and the workflow linter over the whole tree;
and the site's build,
each step a command that runs the same on the Mac.

#### Scenario: Commands
- **WHEN** the jobs' `run` steps are read
- **THEN** `test` runs `mise run test`;
  `build` deploys the SDK at `FLIPPER_SDK_VERSION` with `ufbt update --branch`, then runs `mise run build` and `mise exec -- ufbt lint`;
  `hygiene` runs `mise exec -- prek run --all-files` and `mise exec -- actionlint -shellcheck= -pyflakes=`;
  `docs` runs `mise run docs-build`

#### Scenario: Pinned SDK
- **WHEN** the `build` job's log is read
- **THEN** it reads `Target: 7, API: 88.2`

#### Scenario: Unbuilt checkout
- **WHEN** `mise exec -- prek run --all-files` runs in a checkout that has not built
- **THEN** it exits 0

### Requirement: A run's inputs are pinned
A run SHALL fetch actions by full commit, mise at one version,
tools only as `mise.lock` records them, the site's packages only as `docs/.vitepress/package-lock.json` records them,
and the SDK at `FLIPPER_SDK_VERSION`,
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

#### Scenario: Locked packages
- **WHEN** the `docs` job's log is read
- **THEN** the site's packages install through `npm ci`, which refuses a lock that disagrees with `package.json`

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
- **THEN** `test`, `build`, `hygiene` and `docs` each end `Job succeeded`

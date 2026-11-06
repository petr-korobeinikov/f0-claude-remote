# Proposal

## Why

The repository's one workflow is the app template's `build.yml`.
It builds against the SDK's `dev` and `release` channels rather than the pinned `1.5.1`, daily and on every push,
runs no host test, no lint and no hook,
and its Flipper action pulls `setup-python@v5`, `cache@v4` and pip by tag.
A red run need not be the commit's fault, and a green one proves little the commit owns.
`ci` is the first of three changes before `v0.1`:
`release` will call it, and it will check `dependabot`'s pull requests.

Settled by the owner on 2026-11-06:

- **A Linux runner, `ubuntu-24.04`,** not macOS, which act cannot run locally:
  CI deploys the SDK with `ufbt update --branch`, as `setup` stays macOS-only,
  and builds the host tests with gcc.
- **`build.yml` is deleted,** reversing the keep of 2026-11-03:
  the SDK is pinned and bumped by hand with the firmware, and that bump is the check the schedule gave;
  a firmware with a new API major refuses the published `.fap` anyway.
  `check-workflow` moves to `ci.yml`.
- **`ci.yml` runs on push to `develop` and `main` and on pull requests,**
  in three parallel jobs, each step a command a developer runs:
  `test`, `build` (SDK, build, lint) and `hygiene` (hooks, actionlint).
- **A run goes red only by the commit's fault, so all it checks is pinned:**
  actions by commit, mise by version through `jdx/mise-action` with `cache: false`,
  each job installing its own tools `--locked`, and `.ufbt` uncached.
- **A read-only token,** and a checkout that keeps no credential.
- **No required checks:** nothing requires CI to pass.

Proposed here, for the owner to strike:

- **Change id and capability `ci`.**
- **`check-symlinks` skips `compile_commands.json`,**
  not `SKIP=check-symlinks` in the job:
  the link points at the build's output, so it dangles in every unbuilt checkout,
  `hygiene`'s and a fresh clone's included.
  Unlike the end-of-file rule, no source can fix it, and the hook keeps every other link.
- **actionlint runs with `-shellcheck= -pyflakes=`,**
  not as settled:
  GitHub's runner carries an unpinned shellcheck 0.9.0, act's image none,
  and the Mac a shim with no version set: one commit would lint three ways.
- **`check-workflow` runs `ci.yml` alone,**
  not a workflow named by argument:
  it is the only one, and `release` knows what its own local run needs.
- **No `workflow_call` yet:** `release` adds it when it calls `ci.yml`.
- **`CONTRIBUTING.md` gains a `## CI` section** listing each job's commands:
  a contributor runs them before pushing.

## What Changes

- New `.github/workflows/ci.yml`; `build.yml` deleted, with the Flipper action, `upload-artifact`, the schedule and the `.fap` built per push.
- `.mise/tasks/check-workflow` runs `ci.yml`.
- `prek.toml`: `check-symlinks` skips `compile_commands.json`.
- `CONTRIBUTING.md`: the `check-workflow` bullet and a `## CI` section.

## Capabilities

### New Capabilities

- `ci`: the checks GitHub runs on pushes and pull requests, their pinned inputs, and their contained local run.

### Modified Capabilities

- `app-build`: loses `The template's workflow stays pinned` and `Local workflow runs stay contained`, which `ci` restates for `ci.yml`.

## Impact

Audit of `jdx/mise-action`, 2026-11-06, against GHSA and OSV.
Snyk indexes no actions, and the owner dropped Socket.
`actions/checkout` stays at the audited v7.0.1 `3d3c42e5`.

| Dependency | Version | License | Source | Known vulnerabilities | Transitive artifacts |
|---|---|---|---|---|---|
| `jdx/mise-action` | v5.2.6 `f5be966e`, 2026-11-02, node24 | MIT | GitHub, signed commit | 0 | 83 npm packages in `dist/`; mise 2026.11.2 |

Findings:

- **2 bundled packages carry a moderate DoS advisory:**
  `minimatch` 10.2.6 (GHSA-g3hv-75wg-m249) and `glob` 13.0.6 (GHSA-3g3m-2fjr-6pw4).
  Their patterns come from the action's own code alone: accepted.
- **Bundled licenses:** 53 MIT, 12 ISC, 8 Apache-2.0, 5 BlueOak, 2 BSD, 1 Apache-2.0 AND BSD-3-Clause, 2 undeclared.
- **The mise binary is checked** against its minisign-signed `SHASUMS256.txt`
  and, from 2026.9.3 on, its sigstore packslip.

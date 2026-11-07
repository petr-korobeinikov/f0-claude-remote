# Proposal

## Why

Since `release` landed on 2026-11-07, a version tag publishes a GitHub Release,
but the Flipper Apps Catalog takes a version only as a pull request of one file,
`applications/Bluetooth/f0_claude_remote/manifest.yml`, naming the tag's 40-hex commit,
from a branch the catalog recommends naming `<owner>/f0_claude_remote_<version>`.
No update is automatic, and the catalog's CI rejects a version it already lists, so every release needs one.
`release`'s plan prepared that branch until 2026-11-07,
when the fork and the token's environment still returned 404 and its last task could not close.
`v0.1` is the first submission.

Settled by the owner:

- **`release.yml` prepares the pull request's branch in the owner's fork** (2026-11-06):
  cut from upstream `main` fetched fresh, the fork's `main` synced, the previous version's branch deleted,
  through a fine-grained token on the fork alone; the owner opens the pull request.
- **No `bundle.py` anywhere** (2026-11-06):
  the manifest passes by construction, and the catalog's pull request check runs it.
- **A change of its own, after `release`** (2026-11-07):
  `release` landed with no fork, token or environment.
- **Full names, not a bare `catalog`** (2026-11-07):
  `flipper-apps-catalog-submission` for the change and capability,
  `flipper-apps-catalog` for the job and environment, `FLIPPER_APPS_CATALOG_TOKEN` for the secret:
  a bare `catalog` says nothing in GitHub's settings and run list.

Proposed in `release`'s plan of 2026-11-06, carried over for the owner to strike:

- **The job writes `manifest.yml`,** not a template in `catalog/`:
  its one varying field is the tag's commit, which a template would fake.
- **The fork through the REST API,** not a clone:
  no git identity, and no credential on disk.
- **The token:** Contents and Workflows write on the fork alone, no expiry,
  as `FLIPPER_APPS_CATALOG_TOKEN` in an environment `flipper-apps-catalog` that only `v*` tags reach.
- **The fork's commit reads `Add Claude Remote 0.1`, then `Update Claude Remote to 0.2`,**
  as the catalog's merged pull requests read.

Proposed here, for the owner to strike:

- **A capability of its own,**
  not two requirements added to `apps-catalog`:
  `openspec archive` keeps an existing spec's Purpose, and `apps-catalog`'s covers the catalog's text alone.
- **The step reviewed on 2026-11-07, with two fixes** (see the design):
  as written, a 404's error body passes for the blob `sha` at `v0.1`,
  and the delete at `v0.2` names a path that does not exist.
- **No run before `v0.1`,** under act or against the fork:
  the step needs GitHub's environment, and it writes to the fork.
  Its two reads, the ones the fixes touch, run read-only against the fork instead.
- **The owner's setup opens the tasks,** not closes them as in `release`'s first plan:
  the change cannot close without it, so the implementation stops before writing anything,
  and the plan waits on `feature/flipper-apps-catalog-submission`.
- **Before `docs-development` and `v0.1`:**
  `docs-development` moves `CONTRIBUTING.md`'s `## Release` to the site,
  and `v0.1`'s run is the first submission.

## What Changes

- `.github/workflows/release.yml` gains the `flipper-apps-catalog` job, after `release`.
- `CONTRIBUTING.md`'s `## Release` adds the branch to what the tag runs, and the owner's setup.

## Capabilities

### New Capabilities

- `flipper-apps-catalog-submission`: the branch each release prepares in the owner's fork, and the token that reaches it.

### Modified Capabilities

None.

## Impact

No new dependency: the step runs the runner's `gh`, unpinned, as `release`'s publishing does.

The owner forks the catalog and creates the token and the environment, once, before this change applies.

`docs`'s proposal says `release`'s catalog manifest names `screenshots/main.png`:
`/opsx:update docs`, due before `docs` applies, names this change instead.

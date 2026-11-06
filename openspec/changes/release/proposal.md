# Proposal

## Why

No release can be cut yet.
`CONTRIBUTING.md`'s `## Release` sets `fap_version` and runs git-cliff by hand,
no workflow builds or publishes a Release,
and the Apps Catalog's description and changelog take a Markdown subset
that fails the README (an HTML head, 4 code blocks, 6 H3 headers, 37 inline code spans)
and `cliff.toml`'s sections (`###` groups, backticked hashes).
`release` is the second of three changes before `v0.1`,
and waited on the catalog decision of 2026-11-06.

Settled by the owner on 2026-11-06:

- **One change for the release and the catalog.**
- **`mise run release <name>` makes the release commit** after `git flow release start <name>`:
  it sets `fap_version` and runs both git-cliff configs;
  not git-flow hooks, untracked in `.git/hooks`.
- **A version tag runs `release.yml`:**
  `ci.yml`'s jobs, the tag checked against `fap_version`, the `.fap` built uncached, notes from git-cliff,
  and a Release **published by CI**, not left a draft.
- **The catalog's description is a short file of its own,** not the README.
- **The catalog's changelog is generated** by a second git-cliff config:
  every release newest first, `v<name>:` over its `feat` and `fix` subjects,
  backticks dropped, no `###`, and a fallback line for a release with neither.
- **Both live in a root `catalog/`,** not `docs/`.
- **No `bundle.py` anywhere:** the files pass the catalog's checks by construction.
- **`release.yml` prepares the pull request's branch in the owner's fork:**
  cut from upstream `main` fetched fresh, the fork's `main` synced, the previous version's branch deleted,
  through a fine-grained token on the fork alone;
  the owner opens the pull request.
- **No hooks for `ci.yml`'s checks** in this change.

Proposed here, for the owner to strike:

- **Change id `release`; capabilities `release` and `apps-catalog`.**
- **The task refuses** a name other than the next counter after the latest tag,
  a branch other than `release/<name>` and an unclean tree:
  a published tag is final.
- **The task builds before committing:**
  the build is the one check the release commit can break.
- **`fap_version` goes right after `fap_category`,** as in ufbt's template.
- **One `release` job after `ci`,** not build and publish jobs:
  no artifact actions to audit, and the tag check runs first.
- **The runner's `gh`, unpinned like its `cc`,**
  not one pinned in `mise.toml` and audited for one step.
- **The job writes `manifest.yml`,** not a template in `catalog/`:
  its one varying field is the tag's commit, which a template would fake.
- **The fork through the REST API,** not a clone:
  no git identity, and no credential on disk.
- **The token:** Contents and Workflows write on the fork alone, no expiry,
  as `CATALOG_TOKEN` in an environment `catalog` that only `v*` tags reach.
- **The catalog changelog lists `- ` items after a blank line,**
  not bare lines, which Python-Markdown, the catalog's checker, joins into one paragraph.
  The scope is dropped, and the fallback reads `Internal changes only`.
- **The fork's commit reads `Add Claude Remote 0.1`, then `Update Claude Remote to 0.2`,**
  as the catalog's merged pull requests read.
- **`release.yml` is not run under act:**
  the Release and the fork need GitHub, and the other steps are checked without it.

## What Changes

- New `.mise/tasks/release`, `.github/workflows/release.yml`, `catalog/description.md` and `catalog/cliff.toml`;
  `catalog/changelog.md` is first written by `0.1`'s release commit.
- `ci.yml` gains `workflow_call`.
- `CONTRIBUTING.md`'s `## Release` runs the task and lists the owner's setup;
  the config's release rule names the task and the second file.

## Capabilities

### New Capabilities

- `release`: the release commit the task makes, and the Release a version tag publishes.
- `apps-catalog`: the catalog's description and changelog, and the branch each release prepares in the fork.

### Modified Capabilities

- `ci`: `release.yml` calls it.

## Impact

No new dependency:
the actions stay at their audited commits, and `gh` comes with the runner image.
Before `v0.1` the owner forks `flipperdevices/flipper-application-catalog`
and creates the token and the environment.
The pull request and its AI disclosure stay the owner's.

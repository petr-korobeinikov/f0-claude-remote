# Proposal

## Why

No release can be cut yet.
`CONTRIBUTING.md`'s `## Release` sets `fap_version` and runs git-cliff by hand,
no workflow builds or publishes a Release,
and the Apps Catalog's description and changelog take a Markdown subset
that fails the README (an HTML head, 4 code blocks, 6 H3 headers, 37 inline code spans)
and `cliff.toml`'s sections (`###` groups, backticked hashes).

Settled by the owner on 2026-11-06:

- **One change for the release and the catalog's text;**
  the catalog branch in the owner's fork moves to a change of its own (2026-11-07),
  so this one waits on no fork, token or environment.
- **A version tag runs `release.yml`:**
  `ci.yml`'s jobs, the tag checked against `fap_version`, the `.fap` built uncached, notes from git-cliff,
  and a Release **published by CI**, not left a draft.
- **The catalog's description is a short file of its own,** not the README.
- **The catalog's changelog is generated** by a second git-cliff config:
  every release newest first, `v<name>:` over its `feat` and `fix` subjects,
  backticks dropped, no `###`, and a fallback line for a release with neither.
- **Both live in a root `catalog/`,** not `docs/`.
- **No `bundle.py` anywhere:** the files pass the catalog's checks by construction.
- **No hooks for `ci.yml`'s checks** in this change.
- **shellcheck lands first, as its own change** (2026-11-07), for this plan's budgets:
  the task and `release.yml` meet its checks.
- **`mise run release <name>` on a clean `develop` cuts and pushes the release** (2026-11-07), unpaused,
  not four commands by hand nor git-flow hooks, untracked in `.git/hooks`:
  its commit holds only what the name and reviewed commits generate.
  It builds first, the one check that commit can break,
  and refuses a wrong name, branch or tree, since a published tag is final.
- **A failure before the release commit undoes the run** (2026-11-07), so the fix reruns it as is;
  after it, the finish or push resumes by hand.

Proposed here, for the owner to strike:

- **Change id `release`; capabilities `release` and `apps-catalog`.**
- **`fap_version` right after `fap_category`,** as in ufbt's template.
- **One `release` job after `ci`,** not build and publish jobs:
  no artifact actions to audit, and the tag check runs first.
- **The runner's `gh`, unpinned like its `cc`,**
  not one pinned and audited for one step.
- **The catalog changelog lists `- ` items after a blank line,**
  not bare lines, which the catalog's Python-Markdown joins into one paragraph.
  The scope is dropped; the fallback reads `Internal changes only`.
- **`release.yml` is not run under act:**
  the Release needs GitHub; the other steps are checked without it.

## What Changes

- New `.mise/tasks/release`, `.github/workflows/release.yml`, `catalog/description.md` and `catalog/cliff.toml`;
  `catalog/changelog.md` is first written by `0.1`'s release commit.
- `ci.yml` gains `workflow_call`.
- `CONTRIBUTING.md`'s `## Release` runs the task;
  the config's release rule names the task and the second file.

## Capabilities

### New Capabilities

- `release`: the release commit the task makes, and the Release a version tag publishes.
- `apps-catalog`: the catalog's description and changelog.

### Modified Capabilities

- `ci`: `release.yml` calls it.

## Impact

No new dependency:
the actions stay at their audited commits, and `gh` comes with the runner image.

# Design

## Context

See `proposal.md` for why.
No app layer is touched.

| Fact | Where |
|---|---|
| `gh release create` drafts, uploads assets, then publishes, in separate requests | gh manual |
| A published immutable Release locks its tag; a deleted one's tag name never returns | GitHub docs |
| The runner image carries `gh` | runner-images' `Ubuntu2404-Readme.md` |
| `git flow finish` finishes the current branch, tags it under `tag = true` with `-m`'s message, pushes nothing and resumes with `--continue`; 2.0.0 ends it on `main` | its `--help`, git-flow.sh |
| ufbt's template keeps `# fap_version` below `fap_category` | SDK's `app_template/application.fam:11` |
| git-cliff drops a release left with no commit | `changelog.rs:176` |
| Python-Markdown opens a list only at a block's start, blocks parting at blank lines | `blockparser.py:136`, `blockprocessors.py:454` |
| The catalog's filter refuses what `apps-catalog` lists | `markdown_filter.py` |

The last four rows are sources alone: a bump can break them.

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** `dependabot`; a hotfix release; the README's install section.

## Decisions

| File | Its one task |
|---|---|
| `.mise/tasks/release`, new | Cuts a release from `develop` to `origin` |
| `catalog/cliff.toml`, new | Renders the history as the catalog's changelog |
| `catalog/description.md`, new | Tells the catalog what the app does |
| `.github/workflows/release.yml`, new | Publishes the version its tag names |
| `.github/workflows/ci.yml` | `on` gains `workflow_call:` |
| `CONTRIBUTING.md` | `## Release` runs the task; `## CI` notes the call |
| `openspec/config.yaml` | The release paragraph names the task and `catalog/changelog.md` |

**The task**, `#!/bin/sh` with `set -eu`:

1. Refuses per the spec; the next name raises `MINOR` of the first `git tag --list 'v[0-9]*' --sort=-v:refname` prints, or is `0.1`.
2. Runs `git flow release start <name>`.
3. awk drops any `fap_version=` line and prints `    fap_version="<name>",` after `fap_category=`,
   through a `mktemp -d` file `cat`ed back to keep the mode; one such line must result.
4. Runs git-cliff with `--tag v<name>` into `CHANGELOG.md`, then with `--config catalog/cliff.toml` into `catalog/changelog.md`.
5. Runs `ufbt`.
6. Commits the three files as `chore: release <name>`, through prek and the owner's signing.
7. Runs `git flow finish -m <name>`, then switches to `develop`.
8. Runs `git push --atomic origin main develop v<name>`.

A trap between steps 2 and 6 undoes a failure: the three files restored, `develop` checked out, `release/<name>` deleted.

**`catalog/cliff.toml`** keeps `cliff.toml`'s `[git]` but its parsers:
`^(feat|fix)` goes to group `shown`, the rest to `hidden`, none skipped, so no release empties.
The headerless body prints `{{ version }}:`, a blank line,
each `shown` commit as `- ` with `upper_first` and backticks replaced away, or `- Internal changes only`,
then a blank line unless `previous.version` is empty, so the file ends in one newline.

**`release.yml`** sets `on.push.tags: ["v[0-9]+.[0-9]+"]`, `permissions: contents: read` and `MISE_AUTO_INSTALL: "false"`,
pinning checkout and mise-action as `ci.yml` does:

| Job | Steps |
|---|---|
| `ci` | `uses: ./.github/workflows/ci.yml`, `contents: read` |
| `release`, after `ci`, `contents: write` | checkout, `fetch-depth: 0`; `grep -Fqx` of `fap_version="${GITHUB_REF_NAME#v}",`, else `::error::`; mise-action with `uv pypi:ufbt git-cliff`; `ci.yml`'s SDK step; `mise run build`; `git-cliff --current --strip all --output notes.md`; `gh release create "$GITHUB_REF_NAME" --verify-tag --title "$GITHUB_REF_NAME" --notes-file notes.md dist/f0_claude_remote.fap` |

- **`--verify-tag`:** else `gh` cuts a missing tag from the default branch.

**Test seam.**
A scratch clone pushing to a local bare copy cuts `0.1`, then `0.2` (tasks, group 3):
the task's and the catalog files' scenarios and the version check run there;
`gh release create` first runs at `v0.1`.

## Risks / Trade-offs

- `release.yml` breaks on a tag → nothing publishes before `ci` and the version check pass; delete the tag, fix it, cut the name again.
- `gh release create` fails between requests → delete the draft it leaves and rerun.
- A `feat` or `fix` subject holds `[`, `<` or an entity → the catalog's check fails; keep those out.

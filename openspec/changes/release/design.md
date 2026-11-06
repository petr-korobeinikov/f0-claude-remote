# Design

## Context

See `proposal.md` for why.
No app layer is touched.

The plan rests on:

| Fact | Where |
|---|---|
| `gh release create` with assets drafts, uploads, then publishes, in separate requests | gh manual |
| A published immutable Release locks its tag; a deleted one's tag name never returns | GitHub docs |
| Environment secrets reach only jobs naming the environment, once its tag policy passes | GitHub docs |
| Fork sync, refs and contents take Contents write, a new ref also Workflows; a file update needs its blob `sha`; `gh repo sync` fast-forwards | GitHub docs, gh manual |
| `@path` reads the source at `commit_sha`; `application.fam` supplies the other fields | catalog's `Manifest.md` |
| The runner image carries `gh` | runner-images' `Ubuntu2404-Readme.md` |
| ufbt's template keeps `# fap_version` below `fap_category` | SDK's `app_template/application.fam:11` |
| git-cliff drops a release left with no commit | `changelog.rs:176` |
| Python-Markdown opens a list only at a block's start, blocks parting at blank lines | `blockparser.py:136`, `blockprocessors.py:454` |
| The catalog's filter refuses what `apps-catalog` lists | `markdown_filter.py` |

The last four rows are sources alone: a bump can break them.

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** opening the pull request; hooks for `ci.yml`'s checks; `dependabot`; a hotfix release;
the README's install section.

## Decisions

| File | Its one task |
|---|---|
| `.mise/tasks/release`, new | Makes the release commit |
| `catalog/cliff.toml`, new | Renders the history as the catalog's changelog |
| `catalog/description.md`, new | Tells the catalog what the app does |
| `.github/workflows/release.yml`, new | Publishes the version its tag names |
| `.github/workflows/ci.yml` | `on` gains `workflow_call:` |
| `CONTRIBUTING.md` | `## Release` runs the task and lists the owner's setup; `## CI` notes the call |
| `openspec/config.yaml` | The release paragraph names the task and `catalog/changelog.md` |

**The task**, `#!/bin/sh` with `set -eu`:

1. Refuses per the spec; the next name raises `MINOR` of the first `git tag --list 'v[0-9]*' --sort=-v:refname` prints, or is `0.1`.
2. awk drops any `fap_version=` line and prints `    fap_version="<name>",` after `fap_category=`,
   through a `mktemp -d` file `cat`ed back to keep the mode; one such line must result.
3. Runs git-cliff with `--tag v<name>` into `CHANGELOG.md`, then with `--config catalog/cliff.toml` into `catalog/changelog.md`.
4. Runs `ufbt`.
5. Commits the three files as `chore: release <name>`, through prek and the owner's signing.

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
| `catalog`, after `release`, `environment: catalog` | one shell step, `GH_TOKEN` from `secrets.CATALOG_TOKEN` |

The catalog step, with `fork=$GITHUB_REPOSITORY_OWNER/flipper-application-catalog` and `prefix=$GITHUB_REPOSITORY_OWNER/f0_claude_remote_`:

1. Runs `gh repo sync "$fork" --branch main` and reads that `main`'s commit.
2. Deletes every ref `git/matching-refs/heads/$prefix` lists, a rerun's own included.
3. Creates `refs/heads/$prefix<name>` at that commit.
4. PUTs the manifest there: `Update Claude Remote to <name>` with the blob `sha` when `main` holds it, else `Add Claude Remote <name>`.

The manifest holds the fields `apps-catalog`'s `Manifest` scenario names, in that order,
`commit_sha` from `gh api repos/$GITHUB_REPOSITORY/commits/$GITHUB_REF_NAME`.

- **The catalog step is a job of `release.yml`,**
  not a workflow on `release: published`, which a Release made with `GITHUB_TOKEN` never starts.
- **`commit_sha` from the commits API,**
  not `GITHUB_SHA`, which the docs never say is peeled from an annotated tag.
- **`--verify-tag`:** else `gh` cuts a missing tag from the default branch.

**Test seam.**
A scratch clone, remote removed and signing off, takes this tree's changes in one `chore` commit,
readies git-flow-next per `.gitflow`, and deploys its own SDK, its tracked `.env` naming its own `.ufbt`.
It cuts `0.1`, then `0.2` after another `chore` commit, through `mise run release`:
the file scenarios, the refusals and the version check run there.
The Release and the catalog step first run at `v0.1`.

## Risks / Trade-offs

- `release.yml` breaks on a tag → nothing publishes before `ci` and the version check pass; delete the tag, fix it, cut the name again.
- `gh release create` fails between requests → delete the draft it leaves and rerun.
- The catalog step fails after the Release → rerun it, or commit the file in the fork by hand.
- A `feat` or `fix` subject holds `[`, `<` or an entity → the catalog's check fails; keep those out.
- A reviewer rejects the screenshot as not qFlipper's → it bundles to the same 128×64 picture; recoloring it costs a version.
- A token unused for a year is revoked → a new one before the next release.

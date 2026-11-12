# Design

## Context

See `proposal.md`, Why.
No app layer is touched:
the change lands in the release task and the Flipper Apps Catalog's git-cliff config.

| Fact | Where |
|---|---|
| `76ab21a` changed `.github/workflows/release.yml` alone; the file's three earlier commits are `ci(release)` | `git log -- .github/workflows/release.yml` |
| A parser's `message` regex reads the type and scope; a match ends the search unless `continue = true` | git-cliff docs, `configuration/git` |
| `-p, --prepend [<PATH>]` prepends the entries to the given changelog file | `git-cliff --help`, 2.14.2 |
| `--unreleased --tag v0.3` prints `v0.3:`'s section ending in a blank line, as `previous.version` is `v0.2` | seen on `develop`, 2026-11-12 |
| git-cliff drops a release left with no commit | `release`'s design, `changelog.rs:176` |

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** hiding app commits whose subjects read as build steps, as `v0.1:`'s do;
`CHANGELOG.md`, still regenerated whole from `cliff.toml`.

## Decisions

| File | Its one task | What changes |
|---|---|---|
| `catalog/cliff.toml` | Renders a release's section for the Flipper Apps Catalog | A parser ahead of `shown` |
| `.mise/tasks/release` | Cuts a release from `develop` to `origin` | Line 62 |
| `openspec/config.yaml` | States the project's rules | Lines 40-41 |
| `docs/contributing/release.md` | Tells a contributor how a release is cut | Line 15 and `## Changelog` |

- **`{ message = '^(feat|fix)\(release\)', group = "hidden" }` ahead of the `shown` parser,**
  not `skip = true`: git-cliff drops a release left with no commit, and `- Internal changes only` with it.
  Its comment gives the why: the release machinery ships nothing in the `.fap`.
- **Line 62 runs `git-cliff --config catalog/cliff.toml --unreleased --tag "v$name" --prepend catalog/changelog.md`,**
  not a render the task splices onto the file:
  git-cliff writes the section and keeps the rest,
  and the section's closing blank line (`catalog/cliff.toml:14-15`) parts it from the one below.

`## Changelog` describes each file apart:
`CHANGELOG.md` as today, from `cliff.toml`, grouped and written whole;
`catalog/changelog.md` as gaining each release's section on top, from `catalog/cliff.toml`,
one line per `feat` or `fix` but those scoped `release`, ungrouped.
`openspec/config.yaml:40-41` and `docs/contributing/release.md:15`
have the release commit regenerate the one and add to the other.

Test seam: git-cliff's range argument, and `--prepend` at a copy in a `mktemp -d` directory,
render a release without cutting one.

## Risks / Trade-offs

- A tooling `feat` or `fix` under another scope shows, and its section stays → `mise exec -- git-cliff --config catalog/cliff.toml --unreleased --tag v<name>` prints the section before the cut.
- A failed run leaves the section prepended → the undo's hard reset restores the tracked file, so the rerun adds it once.

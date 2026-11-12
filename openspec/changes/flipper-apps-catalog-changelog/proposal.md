# Proposal

## Why

The Flipper Apps Catalog's changelog lists a fix to the release workflow:
`catalog/changelog.md:29`, `- Look up the tag's commit by its full ref`,
is `76ab21a`, `fix(release)`, which changed one line of `.github/workflows/release.yml`
and nothing the app ships.
`catalog/cliff.toml:30` takes every `feat` and `fix`, whatever its scope.
The review of 2026-11-10 found it;
the catalog's text ships with every release, so its change lands among the first.

Settled by the owner on 2026-11-10:

- **The rule acts from `0.3`, and a section stays as its release wrote it:**
  a release puts its own section on top of the file,
  not a regeneration of the whole file, which would take line 29 out of `v0.1:` too.
  `v0.1:` keeps its 17 lines.

Proposed here, for the owner to strike:

- **`feat` and `fix` scoped `release` stay out,**
  not a list of every scope outside the app:
  `release` is the one such scope these types have taken,
  and the history gives the rest of the tooling `ci`, `docs` or `chore`.
- **`docs/contributing/release.md`'s `## Changelog` gives the catalog file a sentence of its own,**
  not a patch on the shared one:
  that sentence calls both files `cliff.toml`'s, grouped and written whole,
  and this change makes "written whole" false for the catalog file too.
  It closes the review's finding on lines 83-86, which then needs no change of its own.

## What Changes

- `catalog/cliff.toml` puts `feat` and `fix` scoped `release` in its hidden group.
- `.mise/tasks/release` puts the release's section on top of `catalog/changelog.md`
  instead of writing the file whole; `CHANGELOG.md` is still regenerated.
- `openspec/config.yaml`'s release paragraph and `docs/contributing/release.md` say so.
- `apps-catalog`'s `First release` scenario reads the file at `v0.1`, which keeps `76ab21a`'s line.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `apps-catalog`: a release puts its section on top and keeps the ones below; `release`-scoped commits stay out.
- `release`: the task adds the release's section to `catalog/changelog.md` instead of regenerating it.

## Impact

`catalog/cliff.toml`, `.mise/tasks/release`, `openspec/config.yaml` and `docs/contributing/release.md`.
`catalog/changelog.md` stays as it is until `0.3`;
`CHANGELOG.md`, `cliff.toml` and the app stay as they are.

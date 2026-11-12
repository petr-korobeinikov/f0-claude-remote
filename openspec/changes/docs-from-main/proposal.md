# Proposal

## Why

The site describes `develop`, while the latest Release is `0.1`.
`.github/workflows/docs.yml:5` publishes it on each push to `develop`,
and `docs/guide/installation.md:19-20` sends a user to the latest Release.
USB, the dolphin's XP and `Reconnect stopped` came after `v0.1` (`d421d5b`, `c0d89e6`, `6be8b40`),
so whoever installs `0.1` reads of the `Switch to` plate (`docs/guide/using-the-remote.md:60-62`) their app lacks.
Once the Flipper Apps Catalog lists `0.1`, built from its tag, its users reach the site too.
The review of 2026-11-10 found it, for `README.md` as well.

Settled by the owner on 2026-11-11:

- **The site shows the latest Release, from `main`,**
  not `develop` with each newer part marked `since 0.2`, nor a line saying a Release can lag behind:
  a release merges `develop` into `main`, so `main` holds that Release's pages, and no file is kept twice.
  A docs fix reaches the site with the next release.
- **GitHub's default branch stays `develop`, as the owner set it on purpose,**
  so the repository's page, `README.md` and `SECURITY.md` with it, keeps showing `develop`.
- **A version selector on the site is a change of its own,** planned once this one lands.

Decisions:

- **The site publishes on a push to `main`,**
  not when `release.yml` publishes the Release:
  its `publish` job does so with `github.token` (`.github/workflows/release.yml:120-122`),
  and an event that token causes starts no workflow.
- **The `github-pages` environment changes only if it refuses `main`,**
  not limited to `main` alone: the owner keeps repository settings as they are unless the fix needs one.
- **The site is published from `main` now,** by a manual run of `docs.yml` on `main`,
  not left until `0.3`'s release:
  the run proves `github-pages` publishes from `main` before a release relies on it.
  `0.2`, released 2026-11-11, put its text on the site already, from `develop`'s push (`c1a2c26`).
- **`docs-deploy`'s Purpose is edited in its main spec directly,**
  as OpenSpec's spec instructions direct for an existing capability,
  not by a new capability replacing `docs-deploy`:
  sync and archive ignore a delta's Purpose where the spec has one, and this one says `from develop`.
  The new Purpose names no branch, so the version selector changes requirements alone.

## What Changes

- `docs.yml` publishes the site on each push to `main` instead of `develop`.
- The `github-pages` environment admits `main`, if it does not yet.
- A manual run of `docs.yml` on `main` puts `v0.2`'s pages on the site.
- `docs/contributing/` says the site comes from `main` and that a release publishes it.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `docs-deploy`: the site publishes from `main` instead of `develop`.

## Impact

`.github/workflows/docs.yml`, `docs/contributing/hooks-and-ci.md` and `docs/contributing/release.md`;
on GitHub, the `github-pages` environment, if it refuses `main`.
`README.md`, `SECURITY.md`, the guide and the app stay as they are,
and the site shows the guide as `develop` holds it from `0.2` on.

# Proposal

## Why

The app's pictures lie in three places at the repository root:
`images/` (13 files), `screenshots/` (1) and `f0_claude_remote.png`.
Two of those names are plural, beside the singular `src/`, `test/`, `debug/` and `catalog/`,
and the one rule on number, `openspec/config.yaml:129`, binds types alone, so nothing holds a folder to it.
The owner asked for this on 2026-11-09, with no change in flight.

Decisions, settled by the owner:

- `asset/icon/`, `asset/image/` and `asset/screenshot/`, not three entries at the root: one folder per purpose, under one parent.
- `asset/image/`, not `asset/img/`, the name first asked for: `config.yaml`'s "spelled in full" leaves a short form to a domain's own name.
- `docs/` stays whole, `docs/assets/` included, not `docs/asset/`: this change is about the root.
- Folder names in the singular become a rule, with `docs/` and the names a tool fixes as its exceptions.

## What Changes

| From | To |
|---|---|
| `f0_claude_remote.png` | `asset/icon/f0_claude_remote.png` |
| `images/*.png` (13) | `asset/image/*.png` |
| `screenshots/main.png` | `asset/screenshot/main.png` |

- `application.fam`, `README.md`, the Flipper Apps Catalog manifest in `release.yml`
  and the symlink `docs/assets/main.png` point at the new paths.
- `openspec/config.yaml` gains the layout and the folder rule.
- The `.fap` stays the same, byte for byte.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-build`: the pictures live under `asset/`, the package takes them from there, and a missing one stops the build.
- `docs-site`: the site's screenshot and logo trace to the new paths.
- `flipper-apps-catalog-submission`: the manifest names `asset/screenshot/main.png`.
- `user-guide`: Home shows `asset/screenshot/main.png`.

## Impact

15 renames, `application.fam`, `README.md`, `.github/workflows/release.yml`, `docs/assets/main.png` and `openspec/config.yaml`.
`src/`, `test/`, `zensical.toml` and the site's pages stay as they are.

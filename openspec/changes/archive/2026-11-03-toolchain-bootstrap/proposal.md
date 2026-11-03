# Proposal

## Why

The repository has no Flipper toolchain,
and ufbt's defaults reach outside any project:
state falls back to `~/.ufbt` (`ufbt/bootstrap.py:41`),
its toolchain carries a second CPython with `pip` beside the compiler,
and a `ccache` on `PATH` writes under `$HOME`.
Every later change builds on this one.
A dry run in a disposable container with no host mount
changed 0 paths outside `$HOME`, temp and the project,
and wrote 0 files outside the project during a full build.
Scope: steps 1-7 of the untracked start brief;
steps 8-9 go to `debug-and-ide-tasks`.

Decisions, settled with the owner on 2026-11-03:

- **App `f0_claude_remote` at the root,**
  not `app/`:
  ufbt builds the `application.fam` in its working directory.
- **SDK pinned to `1.5.1` in `mise.toml`** (API 88.2, toolchain 39),
  not ufbt's default `release` channel:
  a clean machine gets the verified SDK.
  The device runs firmware 1.5.1 as well.
- **"Outside the project" excludes mise's stores and temp** (listed in the design),
  not `MISE_DATA_DIR` in the repository:
  `mise.toml` cannot set it, and no interpreter reads those stores.
  Removal takes `rm -rf` plus `mise prune`.
- **A fresh clone adds `mise run setup` to the brief's `mise install` and `clion .`,**
  not an install hook:
  that would run package code at install.
- **`setup` deploys the SDK under a write-deny sandbox,**
  not a one-off check:
  each SDK bump can add a write.
- **The template's `.github/workflows/build.yml` stays, pinned,**
  not deleted:
  the owner keeps it, and `mise run check-workflow` runs it through `act`.

## What Changes

- `mise.toml` gains uv, ufbt on a private Python, actionlint, act, guards and a strict-install policy.
- New: app template, pinned workflow, two tasks (the design lists the files).
- `README.md` documents the fresh-clone commands and a pin bump.

## Capabilities

### New Capabilities

- `toolchain-isolation`: confines interpreters, packages and state to the project.
- `app-build`: builds the app from a pinned SDK on a clean machine.
- `ide-navigation`: lets CLion read the sources through compile commands.

### Modified Capabilities

None.

## Impact

Audit, 2026-11-03, against OSV, GHSA and Snyk.
The owner dropped Socket.
Stage 4, no host mount: 3 wheels, no install-time code.

| Dependency | Version | License | Source | Known vulnerabilities | Transitive artifacts |
|---|---|---|---|---|---|
| ufbt | 0.2.6, 2024-07 | GPL-3.0, not linked | PyPI `ee3b5788`, equals tag | 0 | oslex, mslex, SDK, toolchain |
| oslex, mslex | 2.0.0, 1.3.0 | MIT, Apache-2.0 | PyPI `f1c29440`, `c7074b34` | 0 | none |
| uv | 0.13.7 | MIT OR Apache-2.0 | aqua | 0 | none |
| CPython | 3.14.10 | PSF-2.0 | mise core `20261026` | not indexed | `pip`, unexposed |
| Flipper SDK | 1.5.1 | GPL-3.0 | Flipper server, `dfde39df` | not indexed | toolchain 39 |
| Flipper toolchain | 39 `arm64-darwin` | mixed | Flipper server, `d6c6fc35` | 148, below | GCC 12.3, CPython 3.11, 43 packages |
| actionlint, act | 1.7.12, 0.2.95 | MIT | aqua | 0 | runner image |
| `catthehacker/ubuntu` | `act-24.04` `c58e2b36` | MIT | Docker Hub | not scanned | none |
| `actions/checkout`, `actions/upload-artifact` | `v7.0.1` `3d3c42e5`, `v7.0.2` `cf430e03` | MIT | GitHub, signed | 0 | none |
| `flipperzero-ufbt-action` | `v0.1.4` `e7cd3491`, 2024-03 | none declared | GitHub, signed | 0 | below |

Findings:

- **Toolchain 39 carries 148 advisories in 12 of its 43 Python packages**
  (pillow 82, urllib3 16, pip 12, cryptography 11), unpatchable here.
  The owner accepts them: the project exposes nothing outward.
  Only pillow takes input, our own PNGs;
  `requests` is reached by `devboard_flash` alone, for `debug-and-ide-tasks` to weigh.
- **Nothing checks integrity past `mise.lock`:**
  ufbt and `fbtenv.sh` trust TLS; accepted.
- **The Flipper action floats beneath its pin:**
  `setup-python@v5` (Node 20, removed by GitHub on 2026-09-23), `cache@v4` and pip
  stay unpinned, on GitHub's runner only;
  both jobs pass under `act` on Node 24, and GitHub itself shows after a first push.

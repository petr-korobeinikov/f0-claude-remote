# Tasks

No task changes business logic, so none opens with a failing test: group 3's dry run pins the behavior.
Nothing touches the device.

## 1. Tool

- [x] 1.1 Add the design's two `mise.toml` lines, the offline one with its comment, and lock them as `CONTRIBUTING.md`'s tool bump does. Check: `git diff mise.lock` adds only `[[tools.git-cliff]]`, on `aqua:orhun/git-cliff`, 7 platforms; `macos-arm64` reads sha256 `a0be97e237d440d3…` or sha512 `6ec30660233fcd73…`, `linux-x64` `fa21e2fc70046fcf…` or `95b0c55ab3e351ca…`; `mise install` exits 0 with `[tool_config]` back.
  Deviation: `linux-x64` pins the gnu asset, sha256 `24f397c733add539…`; `fa21e2fc…` is `linux-x64-musl`.
  Both gnu assets checked on 2026-11-06: sha256 as locked, the release's `.sha512` OK, GPG good from `4A92FA17B6619297`, the key from `github.com/orhun.gpg`.
- [x] 1.2 Run the spec's network scenarios at the root. Check: `Offline in the project environment` and `No update check` hold, `git-cliff --unreleased` exiting 0; the file under `~/.cache/update-informer-rs`, dated 2026-08-01, is past the check's daily interval, so a check would have rewritten it.
  The cache file stayed at 2026-08-01 15:07.

## 2. Configuration and docs

- [x] 2.1 Write `cliff.toml` per the design. Check: `mise exec -- git-cliff --tag v0.1` prints `# Changelog`, a blank line and one `## 0.1 — <today>` section with as many `- ` lines as the spec's `Entry count` gives, 31 on the plan's `develop`; `mise exec -- prek run --files cliff.toml` exits 0.
  31 entries, as planned.
- [x] 2.2 Add `CONTRIBUTING.md`'s `## Release` section per the design. Check: it sits before `## Hooks`; `mise exec -- prek run --files CONTRIBUTING.md` exits 0; group 3 runs its commands as written, `-c` aside.
- [x] 2.3 Replace the config's sentence per the design. Check: `git grep -nF 'nothing else writes that file' -- openspec/config.yaml` finds one line; `mise exec -- prek run --files openspec/config.yaml` exits 0.

## 3. Two releases in a scratch clone

- [x] 3.1 Clone into a `mktemp -d` folder, remove its remote, ready git-flow-next there as `.gitflow` sets it, and cut `0.1` per `## Release`, editors off, git-cliff run through this project's mise with `-c` on this tree's `cliff.toml`. Check: before the finish, `Rerun on a release commit`, `First release` and `Hooks pass` hold; after it, `v0.1` exists.
  `fap_version` added by hand, `git flow release finish 0.1 --no-fetch -m`; signing off in the clone.
- [x] 3.2 Add an empty `feat(test): add a check` on the clone's `develop`, then cut `0.2` the same way. Check: before the finish, `Rerun on a release commit` holds again; after it, `Next release`, `Entry count` and `Nothing skipped shows` hold for both sections, `0.2` with 1 entry.
  `0.1` section unchanged from `v0.1`'s file; 31 and 1 entries.
- [x] 3.3 Run the notes command at `v0.2`, `v0.1` and the clone's `develop`, then delete the clone. Check: `Notes on a tag` and `Same entries as the file` hold at `v0.2`; at `v0.1` they exit with `No suitable tags found`, and with `v0.2` deleted their `- ` lines equal that section's; on `develop`, `Untagged commit` holds.
  Check revised with the owner on 2026-11-06, after the run: the first tag's notes fail once a second tag exists; accepted, `0.2` has no date.
  31 entries at `v0.1` alone.

## 4. Close

- [x] 4.1 Check: `mise exec -- prek run --all-files` and `mise exec -- openspec validate changelog --strict` exit 0; `git status --short` lists only `mise.toml`, `mise.lock`, `cliff.toml`, `CONTRIBUTING.md`, `openspec/config.yaml` and this change's `tasks.md`; no `CHANGELOG.md` exists yet; `wc -w` holds the budgets.
  `git status` also lists this change's other files, untracked: the plan came over from `develop` uncommitted.
  Words: proposal 655, design 873, spec 534, tasks 570 before notes.

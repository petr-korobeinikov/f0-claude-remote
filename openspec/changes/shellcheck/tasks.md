# Tasks

No task changes business logic, so none opens with a failing test:
each check is a command, a read of a file or a line of an act log.
Nothing touches the device, and nothing is pushed.
The code is already in the tree, and task 1.1 cuts it out of `release`'s work.
After `git flow feature finish`, `feature/release` fast-forwards to `develop` and pops the stash, per the design;
`git status --short` then reads as 1.1 recorded it, less `mise.toml`, `mise.lock` and `prek.toml`.

## 1. The tree

- [ ] 1.1 On `feature/shellcheck`, record `git status --short`, run `git stash push --include-untracked`, then bring this change's hunks back from `stash@{0}` per the design's table. Check: `git diff 'stash@{0}'` shows only `release`'s hunks, reversed; `git status --short` lists the five files alone.

## 2. The pin and the hook

- [ ] 2.1 Check the pin: `mise exec -- shellcheck --version` prints `version: 0.11.0`, and `mise exec -- sh -c 'command -v shellcheck'` a path under mise's installs; each checksum in `mise.lock`'s shellcheck entries equals itte's and the `digest` that `gh api repos/koalaman/shellcheck/releases/tags/v0.11.0` lists for its asset.
- [ ] 2.2 Write an executable `.mise/tasks/probe` and a root `probe.sh`, each `#!/bin/sh` over `echo $1`. Check: `mise exec -- prek run shellcheck --files .mise/tasks/probe probe.sh` fails on both, and a workflow piped to `mise exec -- actionlint -pyflakes= -` with `run: echo $1` fails, each naming SC2086, so `Shell checked` holds. Delete the probes.
- [ ] 2.3 Check: `mise exec -- prek run --all-files` and `mise exec -- actionlint -pyflakes=` exit 0, prek's `shellcheck` line reading `Passed`; read off the files, `Commands` holds, and `CONTRIBUTING.md`'s `## Hooks` and `## CI` match `prek.toml` and `ci.yml`.

## 3. Under act

- [ ] 3.1 Start colima, run `mise run check-workflow`, then `colima stop` and `docker context use default`. Check: `Local outcome` and `Unbuilt checkout` hold; `hygiene`'s setup step installs `aqua:koalaman/shellcheck@0.11.0` through `mise install --locked`, and its prek log reads `shellcheck` `Passed`.

## 4. Close

- [ ] 4.1 Check: `mise exec -- openspec validate shellcheck --strict` exits 0; `git status --short` lists only the five files and this change; `git stash list` still holds `release`'s work; `wc -w` holds the budgets.

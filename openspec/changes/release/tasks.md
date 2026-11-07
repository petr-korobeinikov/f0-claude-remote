# Tasks

No task changes business logic, so none opens with a failing test:
group 3's scratch clone pins the behavior, and each check is a command or a read of a file.
Nothing touches the device.
No task pushes, tags or publishes on GitHub:
tags and pushes reach only group 3's bare copy.

## 1. Catalog files

- [x] 1.1 Write `catalog/description.md` per `apps-catalog`, from the README's Features and its keys table. Check: `Content` holds; `No refused construct` holds through one `grep -nE` for the spec's list; `mise exec -- prek run --files catalog/description.md` exits 0.
- [x] 1.2 Write `catalog/cliff.toml` per the design. Check: `mise exec -- git-cliff --config catalog/cliff.toml --tag v0.1` prints `v0.1:`, a blank line and as many `- ` lines as `git log --format=%s | grep -cE '^(feat|fix)'` counts, 16 on the plan's `develop`, the first `- Show the remote's one screen`.

## 2. Task and workflows

- [x] 2.1 Write `.mise/tasks/release` per the design, executable. Check: `sh -n .mise/tasks/release` and `mise exec -- prek run --files .mise/tasks/release` exit 0, the `shellcheck` hook included, and `mise tasks` lists it. It never runs here, where it would push to GitHub: group 3's clone runs it.
- [x] 2.2 Add `workflow_call:` to `ci.yml` and write `release.yml` per the design. Check: `mise exec -- actionlint -pyflakes=` exits 0; read off the files, `release.yml` holds the design's two jobs alone, and both specs' `Triggers` and `Read off the workflow` hold.
  The `ci` job's `uses: ./.github/workflows/ci.yml` names no commit: a local call runs at the tag's own.
  The version check greps the line as the task writes it, four spaces included, since `-x` matches whole lines.

## 3. Two releases in a scratch clone

Every command runs through this project's mise, `mise exec -- sh -c 'cd <clone> && …'`, the task as `sh .mise/tasks/release`.

- [x] 3.1 Clone this repository's path bare into a `mktemp -d` folder, then clone it with `-b develop`; `git remote -v` must name the copy alone before any other step. Turn signing off, run `prek install`, commit this tree's changes as one `chore` commit, ready git-flow-next per `.gitflow`, run `ufbt update --branch "$FLIPPER_SDK_VERSION"`. Check: `ufbt status version` prints `1.5.1`.
- [x] 3.2 Run the task as `0.1.1`, on `main`, with a file edited, and as `0.1` over a commit appending `#error` to `src/platform/main.c`, then dropped. Check: `Nothing changed` holds for each.
  The first broken build left `CHANGELOG.md` untracked: it, too, is new before `0.1`, so the undo cleans both generated files.
  Each run took a copy of the task from outside the tree, since `main` holds none before `0.1`.
- [x] 3.3 Cut `0.1`, then run `release.yml`'s version check with `GITHUB_REF_NAME` at `v0.1`, then `v0.2`. Check: `First release` holds, and `apps-catalog`'s `First release`, `Rerun on a release commit`, `No refused construct` and `Hooks pass`; `v0.1` passes, and `v0.2` holds `Version check`.
  `main` fast-forwards to the release commit; `develop` gains git's `Merge branch 'main' into develop`, which both git-cliff configs skip.
- [x] 3.4 Add a `chore` commit, run the task as `0.3`, then cut `0.2` with `origin`'s push URL at a missing path; restore it, rerun the push, delete the folder. Check: `Nothing changed` for `0.3`, `Push fails`, `Next release` and `Release with neither` hold, and `CHANGELOG.md` still meets `changelog`'s `Next release`.
  The failed push exits 128 with git's `Could not read from remote repository`; `git flow finish` fetches `origin` first.

## 4. Docs

- [x] 4.1 Rewrite `CONTRIBUTING.md`'s `## Release` and `## Toolchain`'s `release` line for the one command, adding the finish to resume and the push to rerun. Check: the commands match the task and the workflows, and no line names the fork, its token or the `catalog` environment; prek passes on the file.
- [x] 4.2 Rewrite the config's release paragraph for the one command. Check: it names `mise run release 0.1` on `develop`, `git grep -nF 'git flow release finish' -- openspec/config.yaml` finds nothing; prek passes on the file.

## 5. Close

- [x] 5.1 Check: `mise exec -- prek run --all-files`, `mise exec -- actionlint -pyflakes=`, `mise run test` and `mise exec -- openspec validate release --strict` exit 0; `git status --short` lists only the design's files and this change; `wc -w` holds the budgets. `On GitHub` awaits `v0.1`.

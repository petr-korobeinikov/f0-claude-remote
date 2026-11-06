# Tasks

No task changes business logic, so none opens with a failing test:
group 3's scratch clone pins the behavior, and each check is a command, a read of a file or an API read.
Nothing touches the device.
No task pushes, tags or publishes on GitHub, or writes to the fork:
tags exist only in group 3's clone, which has no remote.

## 1. Catalog files

- [ ] 1.1 Write `catalog/description.md` per `apps-catalog`, from the README's Features and its keys table. Check: `Content` holds; `No refused construct` holds through one `grep -nE` for the spec's list; `mise exec -- prek run --files catalog/description.md` exits 0.
- [ ] 1.2 Write `catalog/cliff.toml` per the design. Check: `mise exec -- git-cliff --config catalog/cliff.toml --tag v0.1` prints `v0.1:`, a blank line and as many `- ` lines as `git log --format=%s | grep -cE '^(feat|fix)'` counts, 16 on the plan's `develop`, the first `- Show the remote's one screen`.

## 2. Task and workflows

- [ ] 2.1 Write `.mise/tasks/release` per the design, executable. Check: `sh -n .mise/tasks/release` exits 0 and `mise tasks` lists it. It never runs in this repository: group 3's clone runs it.
- [ ] 2.2 Add `workflow_call:` to `ci.yml` and write `release.yml` per the design. Check: `mise exec -- actionlint -shellcheck= -pyflakes=` exits 0; read off the files, both specs' `Triggers`, `Read off the workflow` and the workflow half of `Environment` hold.

## 3. Two releases in a scratch clone

Every command runs through this project's mise, `mise exec -- sh -c 'cd <clone> && …'`, the task as `sh .mise/tasks/release`.

- [ ] 3.1 Clone this repository's path into a `mktemp -d` folder and remove `origin` first; `git remote` must print nothing before any other step. Turn signing off, run `prek install`, commit this tree's changes as one `chore` commit, ready git-flow-next per `.gitflow`, run `ufbt update --branch "$FLIPPER_SDK_VERSION"`. Check: `ufbt status version` prints `1.5.1`.
- [ ] 3.2 Try `0.1.1`, `develop` and an edited file, then `git flow release start 0.1` and the task. Check: those three of `Refusals`, `First release` and `Built first` hold, and `apps-catalog`'s `First release`, `Rerun on a release commit`, `No refused construct` and `Hooks pass`.
- [ ] 3.3 Finish with `git flow release finish 0.1 --no-fetch -m 0.1`, then run `release.yml`'s version check with `GITHUB_REF_NAME` set to `v0.1` and to `v0.2`. Check: `v0.1` passes, and `v0.2` holds `Version check`.
- [ ] 3.4 Add a `chore` commit on the clone's `develop`, refuse `0.3` on `release/0.3` and drop that branch, then cut `0.2` the same way, and delete the clone. Check: the last `Refusals` case, `Next release` and `Release with neither` hold, and `CHANGELOG.md` still meets `changelog`'s `Next release`.

## 4. Docs

- [ ] 4.1 Rewrite `CONTRIBUTING.md`'s `## Release`: start, the task, finish, then `git push origin main develop v<name>`; what the tag runs; the notes command; the owner's setup — the fork, the token's scopes, the environment, its tag policy and secret. Note `release.yml`'s call under `## CI`. Check: the commands match the task and the workflows; prek passes on the file.
- [ ] 4.2 Name `mise run release` and `catalog/changelog.md` in the config's release paragraph. Check: `git grep -nF 'catalog/changelog.md' -- openspec/config.yaml` finds it; prek passes on the file.

## 5. The owner's setup

- [ ] 5.1 The owner, not the session, forks `flipperdevices/flipper-application-catalog` and creates the token and the environment per `CONTRIBUTING.md`. Check, by `gh api` reads alone: the fork's `parent.full_name` is upstream; the environment's one deployment policy is `v*` of type `tag`; its secret is `CATALOG_TOKEN`.

## 6. Close

- [ ] 6.1 Check: `mise exec -- prek run --all-files`, `mise exec -- actionlint -shellcheck= -pyflakes=`, `mise run test` and `mise exec -- openspec validate release --strict` exit 0; `git status --short` lists only the design's files and this change; `wc -w` holds the budgets. `On GitHub`, `First submission`, `Manifest` and `Next submission` await `v0.1` and `v0.2`.

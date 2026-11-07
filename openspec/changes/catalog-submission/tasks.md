# Tasks

No task changes business logic, so none opens with a failing test:
each check is a command, a read of a file or a read-only `gh api` call.
Nothing touches the device.
No task pushes, tags or writes to the fork: the step first runs at `v0.1`, per the design.

## 1. Preconditions

- [ ] 1.1 The owner, not the session, makes the design's setup. Check, by `gh api` reads alone: the fork's `parent.full_name` is `flipperdevices/flipper-application-catalog`; `catalog`'s one deployment policy is `v*` of type `tag`, its one secret `CATALOG_TOKEN`; the repository's secrets list none of that name. Else stop.

## 2. The job

- [ ] 2.1 Add the `catalog` job to `release.yml` per the design. Check: `mise exec -- actionlint -pyflakes=` exits 0; read off the file, the design's six steps and `Environment`'s workflow half hold, and `release`'s `Read off the workflow` still does.
- [ ] 2.2 Run the step's two reads against the fork by hand, `GITHUB_REPOSITORY_OWNER` set, through the owner's `gh`. Check: the refs read exits 0 and prints nothing; the manifest's read on `main`'s commit exits 1, so `v0.1` takes `Add`.
- [ ] 2.3 Add the `catalog` job, its rerun and the owner's setup to `CONTRIBUTING.md`'s `## Release` per the design. Check: its names and paths match `release.yml` and the setup 1.1 checked; `mise exec -- prek run --files CONTRIBUTING.md` exits 0.

## 3. Close

- [ ] 3.1 Check: `mise exec -- prek run --all-files`, `mise exec -- actionlint -pyflakes=`, `mise run test` and `mise exec -- openspec validate catalog-submission --strict` exit 0; `git status --short` lists only the design's files and this change; `wc -w` holds the budgets. `First submission` and `Manifest` await `v0.1`, `Next submission` `v0.2`.

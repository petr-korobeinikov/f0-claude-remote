# Tasks

No task changes business logic, so none opens with a failing test:
each check is a command, a read of a file, or a line of an act or GitHub log.
Nothing touches the device.

## 1. The open question

- [x] 1.1 Start colima, then run the suites in the image `check-workflow` pins, the tree mounted read-only, with no network: `docker --context colima run --rm --network none -v "$PWD:/w:ro" -w /w <image> sh .mise/tasks/test`. Check: one `ok` line per test, and exit 0. Otherwise stop and raise it, as the design's first risk says.
  gcc 13.3.0 on aarch64, 44 `ok` lines, ASan and UBSan on.

## 2. Workflow

- [x] 2.1 Write `ci.yml` per the design, delete `build.yml`, and add `prek.toml`'s exclusion with its comment. Check: `mise exec -- actionlint -shellcheck= -pyflakes=` and `mise exec -- prek run --all-files` exit 0; read off the files, the spec's `Triggers`, `Commands`, `Pinned references`, `Pinned mise, no cache` and `Token and checkout` hold.
  `prek run --all-files` skips untracked files, so `ci.yml` also passed `prek run --files`.
- [x] 2.2 Rewrite `check-workflow` per the design, and run it, inspecting a job container mid-run. Check: `Local outcome`, `Contained job`, `No credential`, `Pinned SDK` and `Locked tools` hold, each setup step warning of no token; `Unbuilt checkout` holds through `hygiene`, as act's copy has no `.vscode/`. Then `colima stop` and `docker context use default`.
  Containers mount only act's toolcache, env and workdir volumes. `check-symlinks` reads `(no files to check)Skipped` in `hygiene`.
  Setup steps took 21 s (`test`, `hygiene`) and 29 s (`build`); `mise run build` 55 s.

## 3. Docs

- [x] 3.1 Point `CONTRIBUTING.md`'s `check-workflow` bullet at `ci.yml`, and add the `## CI` section the design describes after `## Hooks`. Check: its commands match `ci.yml`'s steps; `mise exec -- prek run --files CONTRIBUTING.md` exits 0; `git grep -n -F build.yml -- ':!openspec'` prints nothing.

## 4. Close

- [x] 4.1 Check: `mise exec -- prek run --all-files` and `mise exec -- openspec validate ci --strict` exit 0; `git status --short` lists only `ci.yml`, the deleted `build.yml`, `check-workflow`, `prek.toml`, `CONTRIBUTING.md` and this change; `wc -w` holds the budgets.
  Words: proposal 622, design 870, spec 530, tasks 298 before notes.

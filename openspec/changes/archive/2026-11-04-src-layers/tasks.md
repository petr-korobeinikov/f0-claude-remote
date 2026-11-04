# Tasks

A rename changes no rule, so no task opens with a failing test: the 17 tests stand as the check.
No task presses a button, so nothing buzzes; 1.1 and 2.4 need the device unlocked on USB.

## 1. Baseline

- [x] 1.1 On `develop`'s tip, run `mise run launch`; keep one frame over RPC, `.ufbt/build/f0_claude_remote/src/` and `dist/f0_claude_remote.fap` outside the repository. Check: `git ls-files src` lists the design table's 14 files; `mise run test` prints 17 `ok` lines, `feedback` 5, `keymap` 12.
  Frame read 3 s after the launch: 13 frames in a second, 1 distinct. FAP 9,908 bytes.

## 2. Rename

- [x] 2.1 Run `git mv` per the design's table; rewrite its five include lines and `.mise/tasks/test`. Check: `mise run test` prints 17 `ok` lines; `mise run build` reads `Target: 7, API: 88.2`; `mise exec -- ufbt lint` exits 0.
  Applied as a patch from the planning trial's clone instead of `git mv`, the owner's call; blobs and modes equal the clone's.
- [x] 2.2 Stage the moves. Check: `git diff --cached -M --name-status` lists 14 `R` lines and `.mise/tasks/test` as `M`, nothing else; `git diff --cached --name-only -- test` prints nothing.
- [x] 2.3 Compare each of the 7 objects with 1.1's, section by section, script outside the repository. Check: only `.debug_info`, `.debug_line` and `.debug_str` differ; `dist/f0_claude_remote.fap` stays 9,908 bytes.
- [x] 2.4 Run `mise run launch`, no press, and read one frame. Check: the log reads `Launching app: /ext/apps/Examples/f0_claude_remote.fap`; 0 of 8,192 cells differ from 1.1's frame.
  Frame read 3 s after the launch: 13 frames in a second, 1 distinct, 0 cells differ.
- [x] 2.5 Change a line in `src/core/action.h` and build, then restore it. Check: the log compiles `src/ui/view.c` and `src/platform/screen.c`, so the build follows a `../` include.
  Six of the 7 compile; `main.c` includes nothing from the core.

## 3. Rules

- [x] 3.1 Replace the config's line per the design. Check: `git grep -nF 'one folder per layer' -- openspec/config.yaml` finds one line; `git grep -nF ', tests in' -- openspec/config.yaml` prints nothing.
- [x] 3.2 Run `Paths`, `The core`, `The UI` and `Includes` as written. Check: each prints nothing; a stray `src/x.c`, a `../` include in the core and a `../platform/` include in the UI each make theirs print, then come back out.
  `Includes` also printed on a `<gui/gui.h>` in the core. The index's tree equals its state before the mutations.
- [x] 3.3 Observe `Database content` and `New source file`, removing the file after. Check: both hold as written.
- [x] 3.4 Read `README.md` against the tree. Check: no statement names a moved file; `git grep -nF 'src/' -- README.md` prints nothing.
- [x] 3.5 Owner, in CLion, after a build: observe `Started from the project shell` on the moved tree. Check: `src/platform/app.c` shows no error; go-to-declaration on `<furi.h>` reaches `.ufbt/current/sdk_headers`.
  Owner: both hold.

## 4. Close

- [x] 4.1 Search for the old paths. Check: `git grep -nE 'src/(action|app|feedback|keymap|keymap_input|main|screen|view)\.(c|h)|-Isrc |"src/\$name' -- ':!openspec/changes'` prints only `host-tests` line 28 and `ide-navigation` lines 16 and 36, which `/opsx:sync` replaces.
- [x] 4.2 Check: `mise run test`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate src-layers --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.
  Lint: 35 folder-name warnings, all under `openspec/`, none under `src/`. After every rebuild, the 7 objects still differ only in the 3 `.debug_*` sections.

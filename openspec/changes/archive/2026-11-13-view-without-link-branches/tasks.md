# Tasks

No task takes the owner's hands, sends a key or buzzes.
Tasks 1.1 and 2.2 take the device, on the cable and unlocked, with the Mac in range and its Bluetooth on:
frames are read over RPC and kept outside the repository.
Six boards are not read, `D_voice`, `D_disc_voice`, `D_stopped` and their USB twins:
they call the functions the four read boards call (`design.md`, Test seams).
Nothing is looked at on USB itself:
`app.c` and `screen.c`, which hand the view its link, do not change.

## 1. The boards before

- [x] 1.1 In `screen_draw_callback`, hand `view_draw` `ModeIdle` and `ModeNoLink`, each on `LinkBluetooth` and on `LinkUsb`, in place of the screen's own; for each pair, `mise run launch` and keep a frame. Take the line out and launch. Check: four frames, no two equal; `git diff --exit-code -- src/platform/screen.c` passes; the last launch's frame equals the first pair's.
  - MD5 of the 1024-byte frames: Idle/Bluetooth `f6866ca8…`, Idle/USB `b7cfe097…`, NoLink/Bluetooth `7880e8d4…`, NoLink/USB `feb798d7…`; the last launch `f6866ca8…`.

## 2. The table

- [x] 2.1 Add `ViewLink` and `view_links` to `src/ui/view.c`, and have the four functions read their row, per the design. Check: `mise run build` and `mise exec -- ufbt lint` exit 0; `mise run test` prints 93 `ok` lines; `git grep -n -e LinkBluetooth -e LinkUsb -- src/ui` prints `view_links`' two rows alone.
- [x] 2.2 Keep the four frames again, as in 1.1, the line out and a launch after them. Check: each frame equals 1.1's of its pair, pixel for pixel; `git diff --exit-code -- src/platform/screen.c` passes; the last launch's frame equals the first pair's.
  - `cmp` finds all five frames byte for byte equal to 1.1's.

## 3. The docs against the tree

- [x] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.
  - None: no statement says how `view.c` picks by link; `docs/internals/architecture.md:31` gives its task alone.

## 4. Close

- [x] 4.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate view-without-link-branches --strict` exit 0; `git grep -n -E '(==|!=) Link' -- src` prints nothing; `git status --short` lists `src/ui/view.c` and this change's files alone; `wc -w` holds the budgets.

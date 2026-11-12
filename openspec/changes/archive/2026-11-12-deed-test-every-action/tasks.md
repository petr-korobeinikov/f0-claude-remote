# Tasks

No task takes the device or the owner's hands: no code the app runs changes.

## 1. The test

- [x] 1.1 Save `mise run test`'s `ok` lines in a `mktemp -d` folder, then add the design's two asserts. Check: `mise run test` exits 0 with the same `ok` lines; with `deed_earned` giving `DeedClear` for `ActionGiveUp`, then for `ActionStartOver`, it exits non-zero naming `test/deed_test.c` at that action's line; undone, `git diff --exit-code -- src` passes.

  80 `ok` lines before and after, no diff.
  Mutated: exit 134, `deed_test.c, line 22` for `ActionGiveUp`, `line 23` for `ActionStartOver`.

## 2. The docs against the tree

- [x] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

  None: no doc names `deed_test.c`'s cases; `README.md:28`, `docs/index.md:37-38`, `docs/guide/using-the-remote.md:81-85` and `docs/internals/architecture.md:28,52` hold as before.
  `docs/development/host-tests.md:39`'s "the dolphin's XP" is the firmware's hand-off in `app.c`, still device-only.

## 3. Close

- [x] 3.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate deed-test-every-action --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

  All five exit 0; `git status --short` lists `test/deed_test.c` and this `tasks.md`.
  `wc -w`: proposal 288, design 223, tasks 213 before this note.

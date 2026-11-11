# Tasks

No task takes the device or the owner's hands: no code the app runs changes.

## 1. The test

- [ ] 1.1 Save `mise run test`'s `ok` lines in a `mktemp -d` folder, then add the design's two asserts. Check: `mise run test` exits 0 with the same `ok` lines; with `deed_earned` giving `DeedClear` for `ActionGiveUp`, then for `ActionStartOver`, it exits non-zero naming `test/deed_test.c` at that action's line; undone, `git diff --exit-code -- src` passes.

## 2. The docs against the tree

- [ ] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 3. Close

- [ ] 3.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate deed-test-every-action --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

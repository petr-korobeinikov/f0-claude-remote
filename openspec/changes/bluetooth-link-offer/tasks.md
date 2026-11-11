# Tasks

No task takes the device or the owner's hands:
the code already follows the minute, which `reconnect-timeout`'s device checks saw,
and no line of it changes.

## 1. The spec

- [ ] 1.1 Set `openspec/specs/bluetooth-link/spec.md`'s Purpose to the proposal's. Check: `git diff` changes the Purpose alone; `mise exec -- openspec validate bluetooth-link --type spec --strict` exits 0.
- [ ] 1.2 Read the code against the delta. Check: `git grep -n '_advertising(' -- src` lists `transport_bluetooth.c`'s start at the opening and in `offer`, its stop in `withdraw`, and nothing else; `mise run test` exits 0.

## 2. The docs against the tree

- [ ] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 3. Close

- [ ] 3.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate bluetooth-link-offer --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

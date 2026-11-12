# Tasks

No task takes the device or the owner's hands:
the code already follows the minute, which `reconnect-timeout`'s device checks saw,
and no line of it changes.

## 1. The spec

- [x] 1.1 Set `openspec/specs/bluetooth-link/spec.md`'s Purpose to the proposal's. Check: `git diff` changes the Purpose alone; `mise exec -- openspec validate bluetooth-link --type spec --strict` exits 0.
- [x] 1.2 Read the code against the delta. Check: `git grep -n '_advertising(' -- src` lists `transport_bluetooth.c`'s start at the opening and in `offer`, its stop in `withdraw`, and nothing else; `mise run test` exits 0.
  - `git grep`: three lines, `transport_bluetooth.c:47` (alloc), `:74` (offer), `:79` (withdraw); `actions.c:46-49` calls the last two.
  - `mise run test`: 80 ok.

## 2. The docs against the tree

- [x] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.
  - None made false or true again: `docs/guide/pairing.md:10,20,31`, `troubleshooting.md:10` and `docs/internals/bluetooth.md:40,74` hold the minute or the setting; no edit.
  - Outside the change: `pairing.md:36`, "While the app is open it holds the radio", is false on USB since `usb-transport` (`transport_hid.c:74-75`); put on the roadmap.

## 3. Close

- [x] 3.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate bluetooth-link-offer --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.
  - `wc -w`: proposal 327, spec 121, tasks 251 of 600.

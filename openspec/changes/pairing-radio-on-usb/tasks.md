# Tasks

`Moved`, `Steps` and `After pairing` hold on the tree as it stands (`README.md:29,44`, `docs/guide/pairing.md:16-34`),
and close in task 1.1 with `The radio`.
No task takes the device or the owner's hands:
the code already hands the radio back on taking USB, as `bluetooth-link` requires, and no line of it changes.

## 1. The page

- [ ] 1.1 Set `docs/guide/pairing.md:36-39` to the proposal's four lines. Check: `git diff` changes lines 36 and 38 alone; `README.md` and the page meet `user-guide`'s four scenarios; `mise run docs-build` and `mise exec -- prek run --files docs/guide/pairing.md` exit 0.
- [ ] 1.2 Read the code against the new lines. Check: `git grep -n 'transport_use(' -- src/core/actions.c` lists the cases of `ActionUseUsb` and `ActionUseBluetooth`; `transport_use` frees the live link before it opens the other (`src/platform/transport_hid.c:35-37`, `:68-72`); Bluetooth's `free` restores the default key path and profile (`src/platform/transport_bluetooth.c:53-62`).

## 2. The docs against the tree

- [ ] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 3. Close

- [ ] 3.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate pairing-radio-on-usb --strict` exit 0; `git status --short` lists `docs/guide/pairing.md` and this change's files alone; `wc -w` holds the budgets.

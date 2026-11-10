# Tasks

Group 1 is test-first and host-only.
Group 3 takes the owner's hands and eyes: on USB the console, and frames over RPC with it, are gone.
`The link returns during a tap` and `Back held through a drop` rest on the host tests alone.

## 1. The rule

- [ ] 1.1 In `test/keymap_test.c`, send the `Press` first wherever a `Short` or `Long` other than Back's comes without one: `keymap_test_fresh`, `keymap_test_alloc_voice`, `test_hold_ok_turns_voice_on`, `test_hold_ok_after_cut_starts_voice_again`, `test_buttons_act_after_link_returns`. Check: `mise run test` exits 0, 77 checks, as before.
- [ ] 1.2 Pin the spec's three scenarios: on `ModeStopped` a `Press`, the link up, Up's `Short` or OK's, Up's, Down's or Left's `Long`; on `ModeNoLink` Up's `Press`, the link up, its `Short`; on `ModeIdle` Back's `Press`, the link down, its `Long`. Check: `mise run test` fails on the first two only; implement the design: exit 0, 80 checks.
- [ ] 1.3 Build for the device. Check: `mise run build` and `mise exec -- ufbt lint` exit 0; `git grep -n right_pressed -- src test` prints nothing.
- [ ] 1.4 In `docs/internals/usb.md`, the paragraph on Right's hold (lines 26-29) gives the rule for every tap and hold but Back's, and the one on starting over (lines 119-125) says that press's tap or hold sends nothing. Check: `mise run docs-build` exits 0.

## 2. The docs against the tree

- [ ] 2.1 Read `README.md` and `docs/` against the tree for statements the rule makes false, or true again, as `docs/guide/using-the-remote.md:73-74`; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 3. With the owner

- [ ] 3.1 With the Mac's Bluetooth off, `mise run launch`. The owner holds Right, pulls the cable, waits for `D_usb_stopped`, plugs the cable back, focuses a terminal on an empty prompt with history, and taps Up. Observe by eye: `D_usb_main`, no buzz, the prompt still empty: `Started over on a cable`. Then the owner holds Back.

## 4. Close

- [ ] 4.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate same-screen-gesture --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

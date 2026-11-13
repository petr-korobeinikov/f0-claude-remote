# Tasks

`The platform follows` closes in task 1.1, `Moved` and `Stayed` in 2.1, `Named by no action and no call` in 3.1,
and `Nothing counted in the platform`, which holds as it is, in 6.1.
Group 5 takes the device, paired and unlocked on the cable, and the owner's hands and eyes:
a paired Mac in range reconnects within a second (`docs/guide/usb.md:8`), so its Bluetooth goes off to reach `No link`,
and on USB the console, and frames over RPC with it, are gone, as in `link-from-the-keymap`'s task 6.1.
Leaving stays unseen there: `transport_hid_free` does not change.

## 1. The transport follows the link

- [x] 1.1 Note `mise run test`'s count of `ok` lines, 95 expected. Add `transport_hid_set_link` and `app_settle`'s line per the design, `transport_use` calling it until 3.2. Check: build and lint exit 0; `The platform follows` holds; `git grep -c 'if(' -- src/platform/transport_hid.c` prints `src/platform/transport_hid.c:1`; 95 `ok` lines.
- [x] 1.2 Rewrite `docs/internals/architecture.md:54` as the design gives it. Check: `mise run docs-build` exits 0; `git grep -c 'that link' -- docs/internals/architecture.md` prints `docs/internals/architecture.md:1`.

## 2. The hold's answer

- [x] 2.1 Rewrite the twelve tests of `test/keymap_test.c` per the design. Check: `mise run test` fails at the sweep's assert on the hold's answer; with the design's case in `keymap.c` and `keymap_use_actions` gone, 95 `ok` lines; with `keymap_forget_presses`' call out of `keymap_set_screen`, it fails at `test_right_held_on_no_link_takes_usb_once`'s last assert; with it back, 95.

## 3. The two actions go

- [x] 3.1 Take `ActionUseUsb` and `ActionUseBluetooth` out of `src/core/action.h`. Check: `mise run test` fails to build on the undeclared names; with their cases, their asserts and `actions_test.c`'s two tests out, 93 `ok` lines, expected; build and lint exit 0; `Named by no action and no call` holds.
- [x] 3.2 Take `transport_use` out of `transport.h`, with its include, of `transport_hid.c` and of the fake, with `transport_fake_link` and the link it reads; cut `docs/development/host-tests.md:30-32`'s last clause. Check: 93 `ok` lines; build, lint and docs-build exit 0; `git grep -n -e ActionUse -e transport_use -e transport_fake_link -- src test docs` prints nothing.

## 4. The docs against the tree

- [x] 4.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.
  None false beyond tasks 1.2 and 3.2's lines; none true again.
  Still true: `docs/internals/usb.md:77-78`, the screen's link before the switch; `docs/guide/using-the-remote.md:41`, no buzz on the switch.

## 5. With the owner

- [x] 5.1 With the cable in, `mise run launch`. The owner turns the Mac's Bluetooth off, holds Right, taps Up in a shell, turns Bluetooth back on, pulls the cable, holds Right, taps Up. Observe by eye: `D_Main`; `D_disconnected`; `D_usb_main`; a command recalled; `D_usb_disconnected`; `D_disconnected`, then `D_Main`; a command recalled.
  All eight seen by the owner on 2026-11-13: the switch both ways, Up recalled over each link.

## 6. Close

- [x] 6.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate transport-from-the-link --strict` exit 0; `Nothing counted in the platform` holds; `git diff --name-only develop` lists the proposal's Impact and this change's files alone; `wc -w` holds the budgets.
  93 `ok` lines; the diff lists the Impact, `tasks.md` and the archive alone.

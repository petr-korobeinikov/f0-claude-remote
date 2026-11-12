# Tasks

Tasks 1.1 and 4.1 open with a failing test on the host.
Group 6 takes the owner's hands and eyes:
a paired Mac in range reconnects within a second (`docs/guide/usb.md:8`), so its Bluetooth goes off to reach `No link`,
and on USB the console, and frames over RPC with it, are gone.

## 1. The keymap

- [x] 1.1 Add the design's two tests and three asserts to `test/keymap_test.c`. Check: `mise run test` fails to build on the undeclared `keymap_link`; declare it in `keymap.h` and define it over `on_usb`: exit 0, 82 `ok` lines.

  Red: `call to undeclared function 'keymap_link'`, `keymap_test.c:146`; green: 82 `ok` lines, 80 before.
  `test_link_stays_through_every_mode` walks Idle, Voice, VoiceCut, Stopped, then NoLink by a press.

- [x] 1.2 Add `LinkMAX`; turn `on_usb` into `Link link` and move the hold into `ModeNoLink`'s case, over the design's tables. Check: `mise run test` exits 0 with 1.1's 82 lines; `mise run build` exits 0; `git grep -n on_usb -- src` prints nothing.

  82 `ok` lines; build exits 0; `git grep -n on_usb -- src` prints nothing.


## 2. The screen

- [x] 2.1 Change `screen_alloc`, `app_alloc`'s call of it, `app_settle` and `app_perform` per the design. Check: `mise run build` and `mise exec -- ufbt lint` exit 0; `git grep -n -e LinkUsb -e LinkBluetooth -- src/platform/app.c src/platform/screen.c` prints nothing.

  Build and lint exit 0; the `git grep` prints nothing.
  `ufbt format` breaks `screen_alloc`'s signature after `Screen*`, in `screen.h` and `screen.c`.

- [x] 2.2 Rewrite the sentence after a press's path in `docs/internals/architecture.md`, lines 52-53, as the design gives it. Check: `mise run docs-build` exits 0.

  `docs-build` exits 0.


## 3. The link's interface

- [x] 3.1 Add `transport_link.h`, make `transport_bluetooth` and `transport_usb` its instances and route `transport_hid.c` through them, per the design, `transport_hid_alloc` taking the link; until 4.1 `transport_use_usb` and `transport_use_bluetooth` call its `transport_use`, `static` as `transport_hid_use`. Check: build and lint exit 0; `git grep -n 'if(' -- src/platform/transport_hid.c` prints nothing; `git grep -c '\.\([a-z]*\) = transport_[a-z]*_\1,' -- src/platform` prints 6 per link.

  Build and lint exit 0; no `if(` in `transport_hid.c`; the slot count prints 6 for `transport_bluetooth.c` and 6 for `transport_usb.c`.
  Each link's state struct moved from its header into its `.c`, the header keeping the `extern const` alone.

- [x] 3.2 Add `transport_link.h`'s row to `docs/internals/architecture.md`, per the design. Check: `mise run docs-build` exits 0; the Files table names every file of `git ls-files src` once.

  `docs-build` exits 0; each of the 32 files under `src/`, `transport_link.h` counted untracked, is named once.


## 4. The switch

- [x] 4.1 In `test/actions_test.c`, read the link through `transport_fake_link`. Check: `mise run test` fails to build on the undeclared name; change `transport.h`, `actions.c`, the fake, `transport_hid.c` and `docs/development/host-tests.md:32` per the design: 82 `ok` lines; build, lint and docs-build exit 0; `git grep -n -e transport_use_ -e fake_on_usb -- src test docs` prints nothing.

  Red: `call to undeclared function 'transport_fake_link'`, `actions_test.c:132`; green: 82 `ok` lines.
  Build, lint and docs-build exit 0; the `git grep` prints nothing.


## 5. The docs against the tree

- [x] 5.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

  None beyond 2.2, 3.2 and 4.1's: `docs/internals/usb.md:60-62,79,120`, `architecture.md:17,26,48-51,69` and `docs/guide/usb.md:48` hold as before.
  `docs-build` exits 0.


## 6. With the owner

- [x] 6.1 With the cable in, `mise run launch`. The owner taps Up in a shell, turns the Mac's Bluetooth off, holds Right, taps Up, turns Bluetooth back on, pulls the cable, holds Right, taps Up, then holds Back. Observe by eye: `D_Main`; a command recalled; `D_disconnected`; `D_usb_main`; a command recalled; `D_usb_disconnected`; `D_disconnected`, then `D_Main`; a command recalled; the desktop.

  All nine seen by the owner as listed; `D_disconnected` read as "Reconnecting..." with the hold-Right legend to USB.


## 7. Close

- [x] 7.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate link-from-the-keymap --strict` exit 0; `git grep -n -e LinkUsb -e LinkBluetooth -- src/platform` prints `transport_hid.c`'s table alone; `git status --short` lists only this change's files; `wc -w` holds the budgets.

  All six exit 0, 82 `ok` lines; the `git grep` prints `transport_hid.c:26-27` alone; `git status --short` lists 21 changed files and the new `transport_link.h`.
  `wc -w`: proposal 652, design 863, tasks 540 as planned.

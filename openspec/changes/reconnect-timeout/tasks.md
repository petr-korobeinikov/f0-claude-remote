# Tasks

Group 2 is test-first and host-only;
the device build stays red from 2.1 until `view.c` meets `ModeStopped` in 3.2.
Every check on the device needs it unlocked on USB, on firmware 1.5.1,
paired, launched from its desktop, with frames read over RPC
and the device's Backlight Time at its default 30 s.
Holding the link down takes forgetting the device on the Mac,
so group 4 pairs twice more; the owner presses.
`A paired Mac comes back` stays unproven, since only leaving the room holds a bonded link down;
`A press does not extend it`, `Stopped with OK still down` and `OK held` rest on the host tests,
and `Forgotten on the Mac` on `ble-transport`'s task 4.3.

## 1. Tree

- [x] 1.1 Check that the canvas's `D_stopped` stands at the design's version, and that the docs changes have left `user-guide` and `internals-guide` in `openspec/specs/`; else stop and raise it. Check: the board's version equals the design's; `ls openspec/specs/user-guide/spec.md openspec/specs/internals-guide/spec.md` lists both.

## 2. The rules

- [x] 2.1 In `test/keymap_test.c`, pin the design's table: the wait's end, Back's press and any other in `ModeStopped`, link reports there, and `keymap_wait_ms` in every mode and fresh. Check: `mise run test` fails on undeclared `ModeStopped`; declare the names: it fails on an assertion; implement: exit 0, count noted, 44 before.
  61 checks; 47 before, not the plan's 44: `dolphin-deeds` landed after the plan and added 3.
- [x] 2.2 In `test/actions_test.c`, pin that `ActionGiveUp` withdraws and `ActionStartOver` offers, neither sending a key. Check: `mise run test` fails on undeclared `transport_fake_offered`; add it, `transport_offer` and `transport_withdraw` to `transport.h` and the fake: it fails on an assertion; extend `actions_perform`: exit 0.
- [x] 2.3 In `test/feedback_test.c`, add both actions to the silent and the flashless. Check: `mise run test` fails on `feedback_vibrates`; fix it: exit 0.
  63 checks.

## 3. The device side

- [x] 3.1 Implement `transport_offer` and `transport_withdraw` in `transport_bluetooth.c`, per the design. Check: `mise run test` exits 0; `git grep -E '#include <(furi|gui|input|notification|bt)' -- src/core` prints nothing.
- [x] 3.2 Draw `D_stopped` in `view.c` from its table. Check: `mise run build` and `mise exec -- ufbt lint` exit 0.
- [x] 3.3 Add `backlight.h`, `.c` and the wait's timer in `app.c`, per the design, moving the backlight out of `app.c`. Check: test, build and lint exit 0; `git grep -n backlight_enforce -- src` finds only `backlight.c`.
  Slip fixed in the design: `app.c` tracks the wait's turn itself, as `furi_event_loop_timer_is_running` reads stale with a stop queued; a link flap would lose the wait.
  `git grep` misses the untracked `backlight.c`; `rg` found it there alone.
- [x] 3.4 Edit the site's pages the proposal names: `pairing.md`'s slow-advertising caveat becomes a press on `D_stopped` first; `using-the-remote.md` gains the screen; `troubleshooting.md`'s Connect row its new fix; `bluetooth.md` the stop, marked; `architecture.md` both new files. Check: `mise run docs-build` exits 0; `git grep -n 'advertised for a minute' -- docs` prints nothing; `architecture.md` names `backlight.h` and `backlight.c`.
  `architecture.md`'s `keymap`, `actions` and `transport.h` rows follow the design's tasks; its path section gains the wait.

## 4. With the owner

- [x] 4.1 With frames open, the owner forgets the device on the Mac. Observe, with no press: `D_disconnected`, then `D_stopped` 60 s after the drop, dark: `Stopped`, `Off on stopping`. The owner tries Connect on `Claude Remote` in the Mac's settings, if listed: no dialog within 30 s, so `A minute with no link`.
  `D_stopped` 60.01 s after `D_disconnected`, 0 pixels off the board; dark and no dialog by the owner's word.
- [x] 4.2 The owner taps Up on the dark screen, connects from the Mac at once, presses OK in the dialog, then nothing for 60 s. Observe: no buzz, `D_disconnected`, then `D_Main`, lit throughout: `Backlight off`, `Any button but Back`, both `Started over`, `Held on`.
  Frames after the press: `D_disconnected`, the dialog, `D_disconnected`, `D_Main`, each hash equal to the first run's; `D_Main` came 23.2 s after the press. No buzz and lit by the owner's word.
- [x] 4.3 The owner forgets the device again; on `D_stopped`, presses Back; waits 60 s on the desktop; then launches the app from it and pairs again. Observe: the app ends at once, the desktop goes dark, `D_Main` at the end: `Stopped screen`, `Leaving after starting over`.
  Not run: the owner struck it as needless, Back always closing the app. Both scenarios rest on reading `app.c` and `backlight.c`: the lock's takes and gives balance.

## 5. Close

- [x] 5.1 Check: `mise run test`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate reconnect-timeout --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

# Tasks

`Started`, `Stopped` and `Left as it is` close in task 2.1,
`Nothing counted in the platform` in 2.2, `A mode not asked` in 1.2.
Group 4 takes the device, paired and unlocked on the cable, and two clicks of the owner's:
a paired Mac in range reconnects within a second (`docs/guide/usb.md:8`),
so its Bluetooth goes off to hold the link down, as in `link-from-the-keymap`'s task 6.1.
Frames are read and Up is tapped over RPC, on Bluetooth, where the console stays;
a tap with no link sends nothing and does not buzz.
Two things stay unseen on the device:
both `Counted afresh`, as the hold takes USB, where the console goes,
and the timer's stop, as the keymap answers nothing to the end of a wait it no longer runs (`src/core/keymap.c:149-151`).

## 1. The wait a mode runs

- [ ] 1.1 Note `mise run test`'s count of `ok` lines, 82 expected. Put `ModeProbe` after `ModeStopped` in `src/core/mode.h`, run `mise run test`, then take it out. Check: it exits non-zero naming `ModeProbe` at two switches of `keymap.c`, `keymap_wait_ms` at neither; `git diff --exit-code -- src/core/mode.h` passes.
- [ ] 1.2 Add `keymap_mode_wait_ms` per the design, `keymap_wait_ms` returning it. Check: `mise run test` exits 0 with 1.1's count; with `ModeProbe` back, it exits non-zero naming it at `keymap_mode_wait_ms`'s switch too; with it out, `git diff --exit-code -- src/core/mode.h` passes.

## 2. The wait's answer

- [ ] 2.1 Add the design's eleven tests to `test/keymap_test.c`. Check: `mise run test` fails to build on the undeclared `keymap_wait`; with the design's `keymap.h` and `keymap.c` without the three resets, it fails at an assert of `KeymapWaitKeep`; with them, it exits 0 with 93 `ok` lines, expected.
- [ ] 2.2 Rewrite `app_follow_wait` per the design; `App.wait_ms`, `app_perform`'s `if` on the switch actions and both comments go. Check: `mise run build` and `mise exec -- ufbt lint` exit 0; `Nothing counted in the platform` holds; `mise run test` prints 93 `ok` lines.
- [ ] 2.3 Rewrite `docs/internals/architecture.md:63` and `docs/internals/usb.md:115` as the design gives them. Check: `mise run docs-build` exits 0; `git grep -n -e 'zeroes its wait' -e 'which follows' -- docs/internals` prints nothing.

## 3. The docs against the tree

- [ ] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 4. With the owner

- [ ] 4.1 With the cable in, `mise run launch`, then frames open. The owner turns the Mac's Bluetooth off. 30 s after `D_disconnected` first shows, with a frame equal to it, tap Up over RPC. Observe: `D_stopped` 60 s after `D_disconnected` first showed, within a second.
- [ ] 4.2 The owner turns the Mac's Bluetooth on. Tap Up over RPC. Observe: `D_disconnected`, then `D_Main` once the Mac connects.

## 5. Close

- [ ] 5.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate wait-from-the-keymap --strict` exit 0; `git diff --name-only develop` lists the proposal's Impact and this `tasks.md` alone; `wc -w` holds the budgets.

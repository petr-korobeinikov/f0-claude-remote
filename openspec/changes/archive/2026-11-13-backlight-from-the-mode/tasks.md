# Tasks

`Lit` and `Dark` close in task 1.1, both `A mode not asked` in 1.2,
`No action named for the light` and `The opening` in 2.1.
Group 4 takes the device, paired and unlocked on the cable, its Backlight Time at 30 s, the default,
and two clicks and three glances of the owner's:
a paired Mac in range reconnects within a second (`docs/guide/usb.md:7`),
so its Bluetooth goes off to hold the link down, as in `wait-from-the-keymap`'s task 4.1,
and no frame shows the light, so the owner says what the display does.
Frames are read and Up is tapped over RPC, on Bluetooth, where the console stays;
the tap on `D_stopped` sends nothing and does not buzz (`src/core/actions.c:48-50`, `src/core/feedback.c:18`).
A second click later than a minute after the tap meets `D_stopped` again, and takes another tap.
One thing stays unseen on the device: `Leaving after starting over`, as `backlight_free` does not change.

## 1. The display's rule

- [x] 1.1 Note `mise run test`'s count of `ok` lines, 93 expected. Add `test/display_test.c` per the design. Check: `mise run test` fails to build on the missing `display.h`; with `display.h` and a `display.c` whose switch has `ModeStopped` among the lit cases, it fails at `test_dark_once_the_offer_stops`' assert; with the design's switch, it exits 0 with 95 `ok` lines, expected.
  - 93 before. The first red was `no such file or directory: 'src/core/display.c'`, the driver stopping before the include;
    with `display.c` alone in place it was `'display.h' file not found`. Then `display_test.c:12`'s assert, then 95.
- [x] 1.2 Put `ModeProbe` after `ModeStopped` in `src/core/mode.h` and run `mise run test`; add `case ModeProbe:` to `display.c`'s switch and run it again; take both out. Check: the first run names `ModeProbe` at `display.c` alone, the second at `keymap.c`'s three switches; then `git grep --untracked -n ModeProbe -- src test` prints nothing and `mise run test` 95 `ok` lines.
  - Run 1: `display.c:4`; run 2: `keymap.c:38`, `:114`, `:157`.

## 2. The platform follows

- [x] 2.1 Give `backlight_alloc` its `lit` and rewire `app.c`, per the design. Check: build and lint exit 0; `No action named for the light` and `The opening` hold; `git grep -n 'action ==' -- src/platform` prints `app.c`'s line on `ActionExit` alone; `git grep -c backlight_enforce_on -- src` prints `src/platform/backlight.c:1`; `mise run test` prints 95 `ok` lines.
- [x] 2.2 Edit `docs/internals/architecture.md` as the design gives it: `display`'s row, and who says the display is dark. Check: `mise run docs-build` exits 0; `git grep -c 'display\.c' -- docs/internals/architecture.md` prints `docs/internals/architecture.md:2`.

## 3. The docs against the tree

- [x] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.
  - None false. True again: `docs/internals/architecture.md:11-13`, the rules in `src/core/` and only the firmware's part in `src/platform/`.

## 4. With the owner

- [x] 4.1 With the cable in, `mise run launch`, then frames open and no press for 40 s. The owner says whether the display is lit, then turns the Mac's Bluetooth off. Observe: lit; `D_stopped` 60 s after `D_disconnected` first showed, within a second; then dark, by the owner's word.
  - Lit at 40 s on `D_Main`, by the owner. Frames: `D_disconnected` at 115.168 s, `D_stopped` at 175.199 s, 60.031 s after; then dark, by the owner.
- [x] 4.2 Tap Up over RPC. 40 s later the owner says whether the display is lit, then turns the Mac's Bluetooth on. Observe: `D_disconnected` after the tap; lit, by the owner's word; `D_Main` once the Mac connects.
  - `D_disconnected` 0.072 s after the tap; lit at 40 s, by the owner. The minute ran out first: `D_stopped`, then a start over not sent over RPC,
    `D_disconnected` at 87.121 s and `D_Main` at 88.315 s.

## 5. Close

- [x] 5.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate backlight-from-the-mode --strict` exit 0; `git diff --name-only develop` lists the proposal's Impact and this change's files alone; `wc -w` holds the budgets.

# Tasks

Every check on the device needs it unlocked on USB, on firmware 1.5.1, with Vibro on in its settings.
A press over RPC buzzes the device like a real one, so a check that presses waits for the owner's word.
Group 1 is test-first; the rest holds no rule.

## 1. The link's rules

- [x] 1.1 Add `src/core/mode.h`; point `test/keymap_test.c` at `keymap_mode` and `keymap_set_connected`, with the design's tests. Check: `mise run test` fails on the undeclared names. Add stubs: it fails on an assertion. Write the rules, keeping `keymap_is_voice_on` for `app`. Check: exit 0, count noted; `mise run build` exits 0.
  30 tests, 17 before.

## 2. The boards

- [x] 2.1 Move the idle board to `src/ui/view_idle.*` per the design, changing no drawing. Check: build and lint exit 0; launched with no press, a frame read over RPC equals `D_Main`.
  24 frames at `1793794331-d8ca`, 0 of 8,192 cells differ. `view.c` 34 lines, `view_idle.c` 112.
- [x] 2.2 Write `images/bluetooth_off.png` from the design's grid, through the toolchain's Pillow, the script outside the repository. Check: the PNG read back equals the grid.
- [x] 2.3 Carry `Mode` through `view_draw`, `screen` and `app`, draw the no-link boards, remove `keymap_is_voice_on` and `screen_set_voice`. Check: test, build, lint exit 0; `arm-none-eabi-nm -u dist/debug/f0_claude_remote_d.elf` names no `__atomic`; launched with no press, a frame equals `D_disconnected`.
  30 tests. 24 frames at `1793794331-d8ca`, 0 of 8,192 cells differ.

## 3. The link

- [x] 3.1 Add `src/platform/transport_bluetooth.*` and `fap_libs`; have `app` feed `link_queue` to `keymap` and `screen`, per the design. Check: build and lint exit 0, package size and imports noted. Launch. Observe: `Name`, `Opened with no link`.
  17.7 KB, 70 imports. Owner, seeing `Claude <device name>`, asked for `Claude Remote` and the device's name; 21 characters overrun `adv_name`'s 16, so the owner chose `Claude Remote` alone.
  The Mac's list kept the old name until pairing; `system_profiler` then reads `Claude Remote`. 0 of 8,192 cells differ from `D_disconnected`.
- [x] 3.2 With the app open, run `mise run launch`. Check: it exits 4 with `has to be closed manually`; anything else goes to the owner. Add the signal callback per the design, close the app, launch twice. Observe: `Launch over a running copy`. Note the rounds and the radio's closing time.
  Before the callback: the message as predicted; `runfap.py` returned 4, scons printed `Error 4`, and `mise run launch` itself exited 2.
  After it, 4 launches over a running copy: exit 0, 2 rounds each, 458-464 ms from close to open, under 2 s of budget.

## 4. With the owner

- [x] 4.1 Note the spec's two MD5s. Connect from the Mac, press Cancel. Observe: `Canceled`.
  Both taken after this session's first launches. Ran after 4.2, as the first dialog got OK and Cancel needs no bond: dialog for 20 s, 0 cells differ from `D_disconnected` after it, and the Mac lists no device.
  First try, launched over the Apps browser: no dialog, the Mac's request failed after 30 s (error 4807), and closing hung. Owner's Back from the browser, launch from the desktop: the dialog shows.
- [x] 4.2 Connect from the Mac, press OK. Observe: `Confirmed`, `Keys apart`. Note what Keyboard Setup Assistant asks. Close and open the app. Observe: `Opened again`.
  0 of 8,192 cells differ from `D_Main`. Key file 1,340 B, `hid_ble`'s MD5 unchanged. Link back at most 7 s after `mise run launch` began, 2 rounds.
  The assistant opened on first pairing, asked for the key right of Shift; the owner closed it, and no layout was chosen.
- [x] 4.3 From the Mac's settings, disconnect the device. Observe: `Disconnected from the Mac`, `The link drops`. Note the delay. Forget it, connect 2 minutes later. Observe: `Forgotten on the Mac`.
  Plan changed with the owner: the Mac reconnects by itself (`BTLEServer`, `reconnecting: Y`), so only forgetting holds a link down. `D_disconnected` lasted 866 ms.
  Forgotten at 19:42:30, connected at 19:49:34: the screen held, the device stayed listed.
- [x] 4.4 With the device forgotten, press every button, then hold Back. Observe: `Every other press`, `Exit`, `The link returns`. Disconnect from the Mac during `/clear`'s flash. Observe: `The link drops during a flash`.
  Owner: no buzz; the screen held through every press. Hold Back ended the app, silent by `feedback.c:4`. Up after pairing: one buzz, a flash, nothing on the Mac, as planned.
  `The link drops during a flash` not observed, by the owner's call: hitting a 400 ms window by hand is not worth it, and `view_draw` draws no flash on a no-link board.
- [x] 4.5 Hold OK, forget the device on the Mac, release; hold OK, disconnect from the Mac, release once it is back, hold; hold OK, forget it, hold Back. Observe: `The link drops during voice`, `OK released`, `Released after the drop`, both `The link returns with OK still down`, `Held again`, `Exit with OK still down`.
  0 cells differ from `D_disc_voice`, `D_disconnected`, `D_Main`. Owner: no buzz at the drop or the release. With OK down the Mac's reconnect gave `D_Main` in 876 ms, voice staying off.
  `Exit with OK still down` not observed, by the owner's call: `test_voice_cut_acts_only_on_held_back` pins the rule. A connect 1 minute after a drop took 28 s, once timing out at 31 s: slow advertising.
- ~~4.6 With the link up and a text field focused on the Mac, walk every button with frames open. Observe: `The board`, `Voice on`, `Voice off`, `The long flash`, `The short flash`, `Backlight off`, `Text field on the Mac`, `Nothing clipped`, frames against the design's version.~~
  Struck by the owner. Seen on the way: `D_Main` 0 cells differ, `Listening`, flashes on Up, nothing typed (`git grep` finds no `ble_profile_hid_kb` in `src`). Flash and voice frames not compared with their boards.
- ~~4.7 Hold Back. Observe: `Setting on`. With Bluetooth off in Settings, open the app, connect, hold Back. Observe: `Bluetooth off in the device's settings`, `Setting off`. Restore the setting.~~
  Struck by the owner. Seen on the way, setting on: after hold Back the desktop shows the Bluetooth mark and `/int/.bt.keys` keeps its MD5. `Bluetooth off in the device's settings` and `Setting off` unproven.
- ~~4.8 With the board attached as `README.md` says, run `Debug` and `Launch`. Observe: `Breakpoint before the application starts`, `Starting the application afterward`.~~
  Struck by the owner: `Breakpoint before the application starts` and `Starting the application afterward` unproven with the radio open.
- [x] 4.9 Forget the device on the Mac, connect. Observe: `Pairing again`. Write `README.md`'s pairing section. Check: its steps are the ones just run.
  Dialog, then `D_Main` 0.4 s after OK, with no assistant the second time. `README.md`'s steps are the ones run, from the desktop.

## 5. Close

- [x] 5.1 Check: `mise run test`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate ble-transport --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

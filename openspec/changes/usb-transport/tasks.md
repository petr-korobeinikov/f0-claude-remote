# Tasks

The branch holds groups 2-4's code up to the return; reopened tasks finish it.
Every check on the device needs it unlocked on USB, on firmware 1.5.1, launched from its desktop.
`A press made elsewhere` and `Another screen`, on both links, rest on `keymap`'s tests;
`Nothing jumps` on the boards' tables;
`A paired Mac comes back` stays unproven, as only leaving the room holds a bonded link down;
`Taken again` on real USB rests on `hid_init`'s sources.

## 1. The boards

- [x] 1.1 Read the canvas's no-link boards. Check: their tables hold the plates and one grid; record the version here.
  Version `1794224713-20ca`. Both plates are `Switch to` and the target's mark, 64×11 at (32, 47); every no-link board has its lines on baselines 28 and 41.

## 2. The router

- [x] 2.1 Split `transport_bluetooth` per the design, with no rule changed. Check: test, build and lint exit 0; launched, a frame equals `D_Main` with no press.

## 3. The rules

- [x] 3.1 In `test/keymap_test.c`, pin the design's tests, replacing `test_right_held_again_on_usb_does_nothing`. Check: `mise run test` fails on undeclared `ActionUseBluetooth`; add it and the rule: exit 0.
- [x] 3.2 In `test/actions_test.c`, pin that `ActionUseBluetooth` switches back, sending nothing. Check: fails on undeclared `transport_use_bluetooth`; add it: exit 0. For now `transport_hid.c`'s return empties the queue and allocates Bluetooth: build exits 0.
  Red on the test's assertion, not on the undeclared name: the test calls only `actions_perform`.
- [x] 3.3 In `test/feedback_test.c` and `test/deed_test.c`, add `ActionUseBluetooth` to the silent, the flashless and the deedless. Check: `mise run test` fails on `feedback_vibrates`; fix it: exit 0.

## 4. The screen

- [x] 4.1 Redraw `D_disconnected`, `D_disc_voice` and their USB twins from their tables; change `app_perform` per the design. Check: test, build and lint exit 0.
- [x] 4.2 With frames open, the owner forgets the device on the Mac. 20 s on, hold Right over RPC. Observe `D_disconnected`, `D_usb_disconnected`, then `D_usb_stopped` 60 s after the hold: `Taken`, `Counted afresh`, `A minute with no link`.
  Board at 0.053 s after the long press; `D_usb_stopped` 60.43 s after the hold.
- [x] 4.3 Tap Up: `D_usb_disconnected`. Hold Right: `D_disconnected`, the Mac's settings listing `Claude Remote`, then `D_stopped` 60 s after the hold: `Returned`, `Counted afresh`. Tap Up, hold Right: `D_usb_disconnected`: `Taken again`. Hold Back: `/int/.bt.keys` keeps its MD5: `USB taken`.
  Board at 0.055 s; `D_stopped` 60.49 s after the hold; `Taken again` at 0.053 s. The owner saw the listing, and a real hold returned too. Bluetooth does start again within one run.

## 5. The USB keyboard

- [x] 5.1 Add `transport_usb.h`, `.c` per the design; `transport_hid.c` routes to them. Check: test, build and lint exit 0; `git grep -E '#include <(furi|gui|input|notification|bt|dolphin)' -- src/core` prints nothing.
- [x] 5.2 Write `guide/usb.md` and `internals/usb.md` with their nav entries; edit `using-the-remote.md`, `troubleshooting.md`, `installation.md`, `architecture.md`, `host-tests.md`, `README.md`'s and `index.md`'s Features, `catalog/description.md`, `fap_description`. Check: `mise run docs-build` exits 0; the docs specs' scenarios read true.

## 6. With the owner

- [x] 6.1 With the cable in, launch; the owner holds Right, allows the Mac's alert, closes Keyboard Setup Assistant if it opens. Observe: `system_profiler SPUSBHostDataType` lists `Claude Remote`; no `/dev/cu.usbmodemflip_*`; `mise run launch` fails; the owner sees `D_usb_main`: `Name`, `On USB`, `Cable in`, `Boards`.
  Listed on 046D:C529 at 12 Mb/s; `SPUSBDataType` prints nothing on this macOS, so the plan's command changed. `launch` exits 2: `Failed to find connected Flipper`. The owner dictated a message over USB, and allowed the Mac's USB accessory alert, which did show.
- [x] 6.2 Hand the device over: the owner runs a scratch Claude Code session from it, dictation and chords included. Observe the owner's word and the session log's commands: `Buttons`, `A flash`.
  The owner: no difference from Bluetooth, chords work. The scratch session's log holds `/clear` and effort `high` and `max`.
- [x] 6.3 The owner pulls the cable and plugs it back; then pulls it past a minute, plugs it back, and taps Up. Observe by eye: `D_usb_disconnected`, `D_usb_main`; dark `D_usb_stopped` through the plug; `D_usb_main`: `Cable out`, `A cable past the minute`, `Started over`.
- [x] 6.4 The owner holds Back; launch. Observe: exit 0, `D_disconnected`: `Leaving`, `Opened again`.
- [x] 6.5 Hold Right over RPC. The owner pulls the cable, holds Right, pairs again, taps Up in a shell, plugs it back; launch. Observe by eye the plate, `D_disconnected`, `D_Main`, the key; exit 0: `Returned`, `The link returns`, `Returning`. Add what 6.1-6.5 saw to `internals/usb.md`, marked seen, and to `guide/usb.md`.
  The hold over an open RPC session switched with no `furi_check`. Launched over the paired app: `D_Main` 1.4 s on, `/int/.bt.keys` kept its MD5.

## 7. Close

- [x] 7.1 Check: `mise run test`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate usb-transport --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

# Tasks

Group 3 is test-first and host-only.
Every check on the device needs it unlocked on USB, on firmware 1.5.1, launched from its desktop.
Frames and presses over RPC need the console, so group 4 reads USB's boards before USB lands;
from 5.1 on they rest on the owner's eye.
Holding the link down takes forgetting the device on the Mac, in 4.2; 6.4 pairs again.
`A press made elsewhere` and `Another screen` rest on `keymap`'s tests.

## 1. The boards

- [ ] 1.1 Check the canvas holds `D_disconnected` with the plate, `D_usb_main`, `D_usb_voice`, `D_usb_disconnected`, `D_usb_disc_voice`, `D_usb_stopped`, `I_usb` and `I_usb_off`; else stop and raise it. Check: `list_files` shows them; record the version here.

## 2. The router

- [ ] 2.1 Split `transport_bluetooth` per the design: `Transport` and the key table move to `transport_hid.h`, `.c`, and `app.c` calls them. No rule changes, so no test opens it. Check: test, build and lint exit 0; launched, a frame equals `D_Main` with no press.

## 3. The rules

- [ ] 3.1 In `test/keymap_test.c`, pin the design's tests. Check: `mise run test` fails on undeclared `ActionUseUsb`; add it and the rule: exit 0.
- [ ] 3.2 In `test/actions_test.c`, pin that `ActionUseUsb` switches, sending nothing. Check: fails on undeclared `transport_fake_on_usb`; add it and `transport_use_usb`: exit 0. For now `transport_hid.c`'s switch frees Bluetooth, empties the queue, and guards the calls: build exits 0.
- [ ] 3.3 In `test/feedback_test.c` and `test/deed_test.c`, add `ActionUseUsb` to the silent, the flashless and the deedless. Check: `mise run test` fails on `feedback_vibrates`; fix it: exit 0.

## 4. The screen

- [ ] 4.1 Add `link.h`, `screen_set_link` and `view_draw`'s `Link`; draw the marks, the line and the plate from the boards' tables; change `app_perform` per the design. Check: test, build and lint exit 0.
- [ ] 4.2 With frames open, the owner forgets the device on the Mac. 20 s on, hold Right over RPC. Observe `D_disconnected`, `D_usb_disconnected`, then `D_usb_stopped` 60 s after the hold: `Taken`, `Counted afresh`, `A minute with no link`. Tap Up: `D_usb_disconnected`. Hold Right: no change: `Held again`. Hold Back: `/int/.bt.keys` keeps its MD5: `USB taken`.

## 5. The USB keyboard

- [ ] 5.1 Add `transport_usb.h`, `.c` per the design; `transport_hid.c` routes to them. Check: test, build and lint exit 0; `git grep -E '#include <(furi|gui|input|notification|bt|dolphin)' -- src/core` prints nothing.
- [ ] 5.2 Write `guide/usb.md` and `internals/usb.md` with their nav entries; edit `using-the-remote.md`, `troubleshooting.md`, `installation.md`, `architecture.md`, `host-tests.md`, `README.md`'s and `index.md`'s Features, `catalog/description.md`, `fap_description`. Check: `mise run docs-build` exits 0; the docs specs' scenarios read true.

## 6. With the owner

- [ ] 6.1 With the cable in, launch; the owner holds Right, allows the Mac's alert, closes Keyboard Setup Assistant if it opens. Observe: `system_profiler SPUSBDataType` lists `Claude Remote`; no `/dev/cu.usbmodemflip_*`; `mise run launch` fails; the owner sees `D_usb_main`: `Name`, `On USB`, `Cable in`, `Boards`.
- [ ] 6.2 Hand the device over: the owner runs a scratch Claude Code session from it, dictation and chords included. Observe the owner's word and the session log's commands: `Buttons`, `A flash`.
- [ ] 6.3 The owner pulls the cable and plugs it back; then pulls it past a minute, plugs it back, and taps Up. Observe by eye: `D_usb_disconnected`, `D_usb_main`; dark `D_usb_stopped` through the plug; `D_usb_main`: `Cable out`, `A cable past the minute`, `Started over`.
- [ ] 6.4 The owner holds Back; launch, and the owner pairs again. Observe: exit 0, `D_disconnected`, then `D_Main`: `Leaving`, `Opened again`. Add what 6.1-6.3 saw to `internals/usb.md`, marked seen, and to `guide/usb.md`.

## 7. Close

- [ ] 7.1 Check: `mise run test`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate usb-transport --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

# Tasks

Every check on the device needs it unlocked on USB, on firmware 1.5.1.
No task opens with a failing test:
`view` only renders, and the exit rule moves under a test in `app-keymap`.

## 1. Icons and view

- [x] 1.1 Write the six PNGs into `images/` from the design's grids, mode `1`, through the toolchain's Pillow, with the script outside the repository; remove `images/.gitkeep`. Check: each PNG read back equals its grid; `mise run build` exits 0; the generated `f0_claude_remote_icons.h` declares the six `I_*`.
- [x] 1.2 Write `src/view.h` and `src/view.c` per the design's draw list. Check: `mise run build` and `mise exec -- ufbt lint` exit 0.

## 2. Application

- [x] 2.1 Replace `f0_claude_remote.c` with `src/app.c` per the design; set `name` and `entry_point` in `application.fam`. Check: `mise run build` and `mise exec -- ufbt lint` exit 0; `compile_commands.json` lists `src/app.c` and `src/view.c`, and no `f0_claude_remote.c`.
  FAP 5.0 KB. The design leaves the queue's put timeout open: 0, so the GUI thread never blocks and an overflow drops the event.
- [x] 2.2 Run `mise run launch`. Read a frame off the device over USB, with the script outside the repository; compare it with board `D_Main` (design, Context) pixel by pixel. With the owner, read the screen on the device. Observe: scenarios `Start`, `Status row`, `Legend`, `Hold and press read apart`, `Nothing clipped`. Note any deviation.
  Frame against `D_Main` version `1793726706-f24f`: 0 of 8,192 cells differ. Owner accepted the screen. Replaces variant A, whose plate meant hold only by an unwritten agreement.
- [x] 2.3 With a text field focused on the Mac, press and hold Up, Down, Left, Right and OK; press Back. Observe: scenarios `Any other button`, `Short press`. Hold Back. Observe: `Long press`; `Apps menu`; opened from that list, a held Back returns to it.
- [x] 2.4 With the application open, run `mise run launch`. Check: exit 0; the log reads `App closed`, then `Launching app: /ext/apps/Examples/f0_claude_remote.fap`; the screen returns.
- [x] 2.5 Turn Bluetooth off in the device's settings; open the application. Observe: scenario `Bluetooth off`. Turn it back on.
  Not run on the device, by the owner's decision: `Connected` is a literal in `view.c`; `src/` names Bluetooth only in `I_bluetooth`, and none of the ELF's 27 imports is a Bluetooth call.

## 3. The renamed file in CLion

- [x] 3.1 Reload the compilation database in CLion. Observe: `src/app.c` and `src/view.c` show no error; go-to-declaration on `gui/gui.h` reaches `.ufbt/current/sdk_headers`.
- [x] 3.2 Attach the board per `README.md`, break on `app.c`'s `gui_add_view_port` line, run `Debug`, then `Launch`, resume the load stop. Observe: CLion stops there with `view_port` in its locals; Step Over reaches the `furi_event_loop_run` line. Press Stop; hold Back; take the board off.
  Stopped at `app.c:39`; stepping went past the loop to line 43, where `view_port->gui` equals `gui`.

## 4. Close

- [x] 4.1 Check: `git grep -n 'f0_claude_remote\.c' -- ':!openspec'` prints nothing; `README.md` is unchanged; `mise exec -- prek run --all-files` and `mise exec -- openspec validate app-screen --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

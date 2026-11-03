# Tasks

Every check on the device needs it unlocked on USB, on firmware 1.5.1.
No task opens with a failing test:
`view` only renders, and the exit rule moves under a test in `app-keymap`.

## 1. Icons and view

- [ ] 1.1 Write the six PNGs into `images/` from the design's grids, mode `1`, through the toolchain's Pillow, with the script outside the repository; remove `images/.gitkeep`. Check: each PNG read back equals its grid; `mise run build` exits 0; the generated `f0_claude_remote_icons.h` declares the six `I_*`.
- [ ] 1.2 Write `view.h` and `view.c` per the design's draw list. Check: `mise run build` and `mise exec -- ufbt lint` exit 0.

## 2. Application

- [ ] 2.1 Replace `f0_claude_remote.c` with `app.c` per the design; set `name` and `entry_point` in `application.fam`. Check: `mise run build` and `mise exec -- ufbt lint` exit 0; `compile_commands.json` lists `app.c` and no `f0_claude_remote.c`.
- [ ] 2.2 Run `mise run launch`. With the owner, hold the device against board `Main` (design, Context). Observe: scenarios `Start`, `Status row`, `Legend`, `Hold and press read apart`, `Nothing clipped`. Note any deviation.
- [ ] 2.3 With a text field focused on the Mac, press and hold Up, Down, Left, Right and OK; press Back. Observe: scenarios `Any other button`, `Short press`. Hold Back. Observe: `Long press`; `Apps menu`; opened from that list, a held Back returns to it.
- [ ] 2.4 With the application open, run `mise run launch`. Check: exit 0; the log reads `App closed`, then `Launching app: /ext/apps/Examples/f0_claude_remote.fap`; the screen returns.
- [ ] 2.5 Turn Bluetooth off in the device's settings; open the application. Observe: scenario `Bluetooth off`. Turn it back on.

## 3. The renamed file in CLion

- [ ] 3.1 Reload the compilation database in CLion. Observe: `app.c` and `view.c` show no error; go-to-declaration on `gui/gui.h` reaches `.ufbt/current/sdk_headers`.
- [ ] 3.2 Attach the board per `README.md`, break on `app.c`'s `gui_add_view_port` line, run `Debug`, then `Launch`, resume the load stop. Observe: CLion stops there with `view_port` in its locals; Step Over reaches the `furi_event_loop_run` line. Press Stop; hold Back; take the board off.

## 4. Close

- [ ] 4.1 Check: `git grep -n 'f0_claude_remote\.c' -- ':!openspec'` prints nothing; `README.md` is unchanged; `mise exec -- prek run --all-files` and `mise exec -- openspec validate app-screen --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

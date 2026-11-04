# Tasks

Every check on the device needs it unlocked on USB, on firmware 1.5.1, with Vibro on in its settings.
A press over RPC buzzes the device like a real one, so a check that presses waits for the owner's word.
Group 1 is test-first; `view`, `screen` and `app` hold no rule, so group 2 opens with no failing test.

## 1. Flash durations

- [x] 1.1 Replace the two flash tests in `test/feedback_test.c` with the durations: arrows 100, Enter and commands 400, the rest 0. Check: `mise run test` fails on undeclared `feedback_flash_ms`. Add it returning 0. Check: it fails on the arrows' assertion. Write its body. Check: exit 0, 17 `ok` lines.
  Ran in a planning dry run the owner chose to keep, with a real red: undeclared, then exit 134 on the arrows' assertion. The count grew from 16 to 17.
- [x] 1.2 Give `screen_flash` its duration and `app_perform` the call per the design; remove `feedback_flashes` and `SCREEN_FLASH_MS`. Check: `mise run test`, `mise run build`, `mise exec -- ufbt lint` exit 0; `git grep -E 'feedback_flashes|SCREEN_FLASH_MS' -- src test` prints nothing.

## 2. Screen

- [x] 2.1 Write `images/enter.png`, `images/slash.png`, `images/microphone_9x13.png` from the design's grids, mode `1`, through the toolchain's Pillow, with the script outside the repository. Check: each PNG read back equals its grid.
- [x] 2.2 Rewrite `src/view.c` per the design. Check: build and `ufbt lint` exit 0; `arm-none-eabi-nm -u dist/debug/f0_claude_remote_d.elf` names no `__atomic`.
  FAP 9.9 KB, 38 imports, none atomic. All its sections equal the dry run's, so the checks below carry over; the file differs past them, at the debug link.
- [x] 2.3 Run `mise run launch`. Keep the RPC stream open, script outside the repository, and send each action. Observe: `The board`, `The long flash`, `The short flash`, `Only the half that acted flashes`, `Acting again restarts it`, `Voice on`, `Voice off`, `Voice takes over`; each frame against its board at the design's version. Note the durations measured.
  On the dry run, over RPC: nine boards at `1793790945-7273`, 0 of 8,192 cells differ. Arrows 96-97 ms, Enter and commands 396-408 ms, on the host's clock.
- [x] 2.4 With the owner, press every button. Observe: `Status row`, `Legend`, `Hold and press read apart`, `Nothing clipped`; each action's flash on the real display; `An action`, `The silent rest`.
  On the dry run. Owner: the buttons and the buzz work. Frames logged during the owner's presses all equal a board.
- [x] 2.5 With the owner, tap Up and Down in series, slow to fast. Observe: `A series of arrow taps`. Note the rate at which flashes merge; a different duration goes back to the owner.
  Owner, on the dry run: the flashes read apart; no merge rate noted, and 100 ms stays. Over RPC, 6 taps at 3, 5 and 8 a second gave 6 of 6 flashes in frames.
- [x] 2.6 With the application open, run `mise run launch`. Observe: `Launch over a running copy`. Then hold Back. Observe: `Long press`.
  Log reads `App closed`, then `Launching app`; 24 of 24 frames equal `D_Main`. Back was not held: the owner vouched for it.
  After one earlier launch a frame differed by 324 cells, the area of `/clear`'s flash, with no press and not saved; 6 launches and 167 s idle then gave 0 input events.

## 3. Close

- [x] 3.1 Check: `mise run test`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate screen-redesign --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

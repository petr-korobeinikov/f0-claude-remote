# Tasks

Every check on the device needs it unlocked on USB, on firmware 1.5.1, with Vibro on in its settings.
Groups 1 and 2 are test-first; `view`, `screen`, `keymap_input` and `app` hold no rule, so group 3 opens with no failing test.

## 1. Keymap and the host tests

- [x] 1.1 Write `.mise/tasks/test`, `test/test.h`, `src/action.h`, `src/keymap.h` and `test/keymap_test.c`'s layout tests. Check: `mise run test` fails linking `keymap_alloc`. Write `src/keymap.c`'s table. Check: exit 0, one `ok` line per test.
  Written with its code in a planning dry run the owner chose to keep, so no red came first; 34 of 34 hand-made mutants of `keymap.c` and `feedback.c` were killed instead. Replayed while committing: with no `src/keymap.c` the task fails on the missing file, not at linking as planned; an empty file gives the `_keymap_alloc` link error.
- [x] 1.2 Add the voice tests. Check: they fail on their assertions. Add the voice state. Check: `mise run test` exits 0.
  No red of its own, as in 1.1: removing the voice state fails `test_hold_ok_turns_voice_on` and `test_release_ok_stops_voice_once`.
- [x] 1.3 Break `keymap_is_voice_on` for a moment. Observe, piped through `cat`: scenario `A failing test`. Restore it. Run the grep of scenario `Includes`.
  Exit 134, with `keymap_test.c`'s line and every earlier `ok` line. The grep prints nothing.
- [x] 1.4 Add `sources` to `application.fam`. Check: `mise run build` exits 0; scenario `Package`. Add an unformatted `test/probe.c`. Observe: scenario `Lint`. Remove it. Check: `mise exec -- ufbt lint` exits 0.
  The database holds 8 entries, none under `test/`. The probe ran in the dry run's clone: lint exited 2 and named the file.
- [x] 1.5 Copy `.run/Build.run.xml` to `.run/Test.run.xml`, running `mise run test`. In CLion observe: scenarios `First open`, `Test from the IDE`; `git status --short` lists nothing unplanned.
  The owner ran `Test` in CLion: 16 `ok` lines, exit code 0. The worktree stayed clean.
- [x] 1.6 In `README.md` list `test` and state the `cc` exception; in `openspec/config.yaml` replace the "undecided" line and name the new modules under Platform. Check: `README.md` read against the tree; `git grep 'are undecided' openspec/config.yaml` prints nothing.
  Nothing else in `README.md` went stale.

## 2. Feedback

- [x] 2.1 Write `src/feedback.h` and `test/feedback_test.c`. Check: `mise run test` fails linking. Write `src/feedback.c`. Check: exit 0, the count of `ok` lines grows by the new suite.
  Written with its code, as in 1.1. With no `src/feedback.c` the task fails on the missing file, not at linking. The count grew from 12 to 16.

## 3. Screen and application

- [x] 3.1 Write `images/microphone_large.png` from the design's grid, mode `1`, through the toolchain's Pillow, with the script outside the repository. Extend `src/view.h`, `src/view.c` per the design. Check: PNG read back equals the grid; build and `ufbt lint` exit 0.
- [x] 3.2 Write `src/keymap_input.*`, `src/screen.*`, `src/app.h`, `src/main.c`; move `src/app.c` onto them per the design; point `entry_point` in `application.fam` at `main_entry`. Check: build and `ufbt lint` exit 0; `arm-none-eabi-nm -u dist/debug/f0_claude_remote_d.elf` names no `__atomic`.
  The owner split `main.c` off `app.c` during review, reversing `app-screen`'s "no `app.h`". FAP 8.8 KB, 38 imports, none atomic.
- [x] 3.3 Run `mise run launch`. Keep the RPC stream open, script outside the repository, while the owner holds OK and fires each command. Observe: `Voice on`, `Voice off`, `The flash`; each frame against its board at the design's version. Note any deviation.
  Inputs went over RPC rather than by hand, for exact timing. `D_voice`, `D_Main` and the four `D_sent_*` at `1793780081-d235`: 0 of 8,192 cells differ. The flash lasts 398-399 ms.
- [x] 3.4 With the owner, press every button. Observe: `Tap`, `Hold`, `A hold does not repeat`, `Esc acts on the press`, `A tap with no action`, `An action`, `The silent rest`, `Only the held half flashes`, `Acting again restarts it`, `Arrows and Enter do not flash`; Back's `Long press`, `Short press`.
  Real buttons: the frames logged meanwhile equal the same boards, and the owner felt each buzz. The owner found that arrow taps give no visible response and ordered a redesign for the next plan.
- [x] 3.5 Hold OK; tap and hold every other button, Back included. Observe: `Release`, `Another button during voice`, `Buttons after voice`. Press OK and, before its long press fires, Right; keep OK down. Observe: `Voice takes over`.
  Over RPC, Esc's flash gave way to voice, with no idle frame until the release. With the real OK the owner felt the buzz at the start and none at the release.
- [x] 3.6 With the owner, try slow taps and quick holds on OK and Left. Note whether 300 ms fires `clear` too easily or a slow tap starts voice. Change nothing; the finding goes to the next plan.
  Owner: 300 ms is comfortable; a slow tap gives Enter, and voice does not start unasked.
- [x] 3.7 Let the backlight time out; tap Up. Observe: `Backlight off`.
  Owner: the buzz comes.
- [x] 3.8 With a text field focused on the Mac, tap and hold every button. Observe: `Text field on the Mac`.
  Not run on the device, by the owner's decision: none of the ELF's 38 imports is a Bluetooth, HID or USB call.
- [x] 3.9 With the application open, run `mise run launch`. Check: exit 0; the log reads `App closed`; the screen returns.
  The log reads `App closed`, then `Launching app`; the next frame read equals `D_Main`.

## 4. Close

- [x] 4.1 Check: `mise run test`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate app-keymap --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.
  All exit 0, with 16 `ok` lines and a clean worktree. Words as planned: proposal 654, design 867, tasks 624, specs 515, 456, 249, 104.

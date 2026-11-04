# Tasks

Every check on the device needs it unlocked on USB, on firmware 1.5.1, paired, launched from its desktop.
A hold buzzes the device, so a check that presses waits for the owner's word, and the owner presses.
A chord acts on the focused session, this one included: finish at `high`.
Group 3 is test-first; the rest holds no rule.

## 1. The Mac's side

- [x] 1.1 With text in the prompt and `ctrl+x ctrl+f` unbound, the owner presses it on the Mac's keyboard. Observe: `Keys not bound`. Add the line from the design. Under a non-Latin layout, press it again. Observe: `Effort high`, `A draft`. Note `claude --version`. If it fails, stop and raise it: the route falls.
  Claude Code 2.1.311. The owner saw all three.
- [x] 1.2 Write `README.md`'s section after Pairing: the design's block, the config directory `CLAUDE_CONFIG_DIR` overrides, the version tried, and that an editor marks the two lines. Check: the block equals the owner's lines; `The block`.

## 2. The transport

- [x] 2.1 Add `src/core/transport.h`; have `transport_bluetooth` implement it and keep the profile, per the design, replacing `TransportBluetooth`. Check: `mise run test`, build and lint exit 0; launched with no press, a frame over RPC equals `D_Main`.
  30 tests. 0 of 8,192 cells differ at `1793794331-d8ca`.

## 3. The keys

- [x] 3.1 Add `test/transport_fake.*` and `test/actions_test.c` with the design's tests; link the fake in `.mise/tasks/test`. Check: `mise run test` fails on undeclared `actions_perform`. Add a stub: it fails on an assertion. Write `src/core/actions.*`. Check: exit 0, count noted, 30 before; `Effort high's keys`, `Effort max's keys`, `Every other action`.
  33 tests. Both reds as planned; the undeclared one needed an `actions.c` and `actions.h` holding no function, since the task names the source.
- [x] 3.2 Call `actions_perform` from `app_perform`, per the design. Check: test, build and lint exit 0; package size noted, 17.7 KB before; `git grep -E '#include <(furi|gui|input|notification|bt)' -- src/core` prints nothing.
  18,768 B, 18.3 KB. The grep also ran over the untracked sources: nothing.

## 4. With the owner

- [x] 4.1 With text in the prompt, under a non-Latin layout, the owner holds Up, then Down, on the device. Observe: `Effort high`, `Effort max`, `Non-Latin layout`, `A draft`.
  The owner's screenshot: both lines, the non-Latin draft in place and unsent.
- ~~4.2 During a long command in this session, the owner holds Down, then Up. Observe: `Claude working`.~~
  Struck by the owner: effort sets, as 4.1 shows. In two runs the flashes came during the command and the turn went on, with no line on screen or in the session's log. The log's per-request effort went from `max` at 17:01:45Z to `high` at 17:01:48Z, inside a turn, again with no line.

## 5. Close

- [x] 5.1 Check: `mise run test`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate effort-actions --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.
  33 tests. prek also ran over the untracked files. Words: proposal 575, design 747, tasks 476, specs 34 and 511, within a tenth of 500.
- [x] 5.2 At `/opsx:sync`, take `does not reach the Mac yet` out of `button-actions`' Purpose. Check: `git grep 'does not reach the Mac' openspec/specs` prints nothing.

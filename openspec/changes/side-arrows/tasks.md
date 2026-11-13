# Tasks

`app-screen`'s first requirement closes in task 2.1, `The plain keys` and `An action` in 3.1,
`Tap`, `Hold`, `Nothing acts on the press`, `A tap with no action`, `The silent rest` and `A press with no action` in 4.1,
`Content` in 4.2, and `The side arrows`, `Esc`, `The long flash`, `The short flash`
and `Only the half that acted flashes` in 6.1.
The other scenarios the deltas carry hold as written, on tests and lines the change leaves alone.
Groups 1, 2 and 6 take the device, unlocked on the cable, with the paired Mac in range:
frames are read over RPC, kept outside the repository, and compared with the boards at `1794586781-3562`.
No press goes over RPC (`proposal.md`, Why), and task 6.1's presses buzz, so it waits for the owner's word.
Nothing is tried on USB: both links take their usages from one table (`src/platform/transport_hid.c:14-23`).
The guide's line on agent view rests on Claude Code's documentation alone.

## 1. The fill

- [ ] 1.1 Keep a frame of `D_Main` from `mise run launch`. Replace `view_idle_draw_split` with `view_idle_draw_fill` per the design, and keep a frame again. Check: `mise run build`, `mise exec -- ufbt lint` and `mise run clang-tidy` exit 0; the two frames are equal, byte for byte; `git grep -n view_idle_draw_split -- src` prints nothing.

## 2. The plates

- [ ] 2.1 Draw Left's and Right's plates and move `/clear`'s and Esc's flashes per the design. Check: build, lint and clang-tidy exit 0; launched, a frame equals `D_Main`, 0 of 8,192 cells apart.

## 3. The two arrows

- [ ] 3.1 Add the design's tests of `actions`, `feedback` and `deed`, then both enumerators, both keys with their rows, and both arrows' cases in the five switches. Check: `mise run test` fails on undeclared `ActionArrowLeft`, then at `actions.c`'s switch, then prints 95 `ok` lines, expected; build, lint and clang-tidy exit 0; `git grep -c ARROW -- src/platform/transport_hid.c` ends in `:4`.

## 4. The keymap

- [ ] 4.1 Rewrite `test/keymap_test.c`'s tests per the design. Check: `mise run test` fails at `test_tap_acts`' assert on Left; with the design's two rows in and the comment at `keymap.c:17` out, it prints 94 `ok` lines, expected; build, lint and clang-tidy exit 0.
- [ ] 4.2 Edit the four pages per the design. Check: `mise run docs-build` and `mise exec -- prek run --files catalog/description.md` exit 0; `git grep -n -E 'Right, press|Right goes down|Tapping Left|tap on Left|no hold of its own' -- docs catalog` prints nothing.

## 5. The docs against the tree

- [ ] 5.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 6. With the owner

- [ ] 6.1 With frames open, the owner focuses a scratch Claude Code session, types `/effort`, taps Left, taps Right, holds Right; sends a prompt, holds Right mid-turn; holds Left. Observe: the slider down a level, back, closed; the turn stopped; a new conversation; a buzz each; frames `D_sent_left`, `D_sent_right`, `D_sent_esc` twice, `D_sent_clear`, each then `D_Main`.
- [ ] 6.2 The owner saves qFlipper's screenshot of `D_Main` as `asset/screenshot/main.png`. Check: it is 512×256 in black and `(254, 138, 44)` alone, and its cells, every fourth each way, equal task 2.1's frame; `mise run docs-build` exits 0.

## 7. Close

- [ ] 7.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run clang-tidy`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate side-arrows --strict` exit 0; `git diff --name-only develop` lists the proposal's Impact and this change's files alone; `wc -w` holds the budgets.

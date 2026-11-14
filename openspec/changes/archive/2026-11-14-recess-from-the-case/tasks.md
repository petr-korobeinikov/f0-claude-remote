# Tasks

The delta's six scenarios close in task 2.1,
which also puts the device back on the `D_Main` the other specs name.
`app-screen`'s flash scenarios, left as written, hold on the redrawn `D_sent_*` boards in 3.1.
Groups 2 and 3 take the device, unlocked on the cable, with the paired Mac in range:
frames are read over RPC, kept outside the repository, and compared with the boards at `1794652954-5503`.
No press goes over RPC, and nothing is tried on USB (`proposal.md`, Why);
task 3.1's presses buzz, so it waits for the owner's word.

## 1. The image

- [x] 1.1 Write `asset/image/recess.png` from the design's grid, mode `1`, through the toolchain's Pillow, with the script outside the repository. Check: the PNG read back equals the grid, 206 black cells of 4,094; `mise run build` exits 0, and `.ufbt/build/f0_claude_remote/f0_claude_remote_icons.h` declares `I_recess`.
  89×46, 1-bit grayscale, 206 black of 4,094 read back; the grid equals the one `canvas-check` kept from the canvas.

## 2. The board

- [x] 2.1 Redraw `src/ui/view_idle.c` per the design. Check: `mise run build`, `mise exec -- ufbt lint` and `mise run clang-tidy` exit 0; launched, a frame equals `D_Main`, 0 of 8,192 cells apart; `git grep -n -E 'view_idle_draw_recess|ring runs' -- src` prints nothing.
  The canvas still at `1794652954-5503`; the probe's boards moved to it, the older kept in `1794586781-3562/`.

## 3. With the owner

- [x] 3.1 With frames logged, the owner, on a scratch Claude Code session, taps OK, Up, Down, Left and Right and holds Up, Down, Left and Right, in any order. Observe: a buzz each; frames `D_sent_enter`, `D_sent_up`, `D_sent_down`, `D_sent_left`, `D_sent_right`, `D_sent_high`, `D_sent_max`, `D_sent_clear` and `D_sent_esc`, each then `D_Main`.
  The owner, 2026-11-14: all works. Logged: `D_sent_up`, `_down`, `_right`, 89-90 ms; `D_sent_clear`, 382 ms; each then `D_Main`.
  `D_sent_enter`, `_left`, `_high`, `_max` and `_esc` were not logged: the owner's word stands for them.
- [x] 3.2 The owner saves qFlipper's screenshot of `D_Main` as `asset/screenshot/main.png`. Check: it is 512×256 in black and `(254, 138, 44)` alone, and its cells, every fourth each way, equal task 2.1's frame; `mise run docs-build` exits 0.
  0 cells apart, the old screenshot 2,176 from it; every 4×4 block one color; 50,368 black pixels, 80,704 orange, as before.

## 4. Close

- [x] 4.1 Check: `mise run test` prints 94 `ok` lines, as before; `mise run build`; `mise exec -- ufbt lint`; `mise run clang-tidy`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate recess-from-the-case --strict` exit 0; `git status --short` lists the proposal's Impact and this change's files alone; `wc -w` holds the budgets.
  Words: proposal 478, design 760, specs 354, tasks 432 as planned.

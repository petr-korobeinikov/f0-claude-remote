# Design

## Context

See `proposal.md` for why.
Firmware facts below come from SDK 1.5.1's headers and `runfap.py`,
and from the disassembly of its `firmware.elf`.

The draw list and icons were compared pixel by pixel with board `Main`
of the wireframe, version `1793695107-9c5b`:
all 7,025 cells outside the text boxes match but one, named under Risks.
Text needs the firmware's fonts, so it is left to the check on the device.

## Goals / Non-Goals

**Goals:** board `Main` on the device; a way out; `mise run launch` still working over a running copy.

**Non-Goals:** the other `A_*` boards; any button but Back; vibration; Bluetooth; the lock; a host test setup.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `view.h` | UI | Declares the screen's draw call | Compiled by `app.c` and `view.c` | The header |
| `view.c` | UI | Draws the screen | By eye against the board, on the device | `Canvas*`, all it touches |
| `app.c` | Platform | Runs the application's event loop | Buttons and `mise run launch`, on the device | The device's buttons and `ufbt launch` |
| `images/*.png` | UI | Hold one bitmap each | Read back against the grids below | `I_*` in the generated header |

`view` holds no logic, so it has no test of its own and no red-green cycle.
`app` is platform code because `ViewPort`, `Gui`, `FuriEventLoop` and `InputEvent` exist only in the firmware.

- **`ViewPort` on a `FuriEventLoop`,**
  not a `ViewDispatcher`:
  one screen has no view stack to dispatch,
  and `gui/view.h` would put the SDK's `view_*` beside ours in `app.c`.
- **`FuriEventLoop`,**
  not a blocking read of a queue:
  `ufbt launch` runs `loader close` first,
  which answers `has to be closed manually` for a thread with no signal callback,
  so `runfap.py` returns 4,
  while `furi_event_loop_run` installs a callback that stops it on `FuriSignalExit`.
- **Input reaches the loop through a queue of 8,**
  not `furi_event_loop_stop` in the input callback:
  that callback runs on the GUI thread,
  and `event_loop.h` binds its functions to the loop's own thread.
- **`GuiLayerFullscreen`,**
  not `GuiLayerWindow`:
  the status bar would cover the board's top row.
- **`view.h` declares `void view_draw(Canvas* canvas)` alone,**
  not a `View` with `alloc` and `free`:
  nothing holds state until `ble-transport` adds the argument,
  and `gui/view.h` already owns those names.
- **Literal coordinates from the board,**
  not widths from `canvas_string_width`:
  the display is a fixed 128×64, and each call reads against a row of the board.

`app_main` allocates the loop and the queue and subscribes,
sets up the `ViewPort` and adds it, then runs the loop.
It tears down in reverse:
`gui_remove_view_port`, the record, the `ViewPort`,
the subscription, the queue, the loop.

`view.c` assembles the screen from static components:
`view_draw_status_row`, `view_draw_legend`, `view_draw_ok_button`,
and `view_draw_hold_label`, which all four plated labels share.

The draw list, in order, black unless noted.
`hold` is `canvas_draw_rbox(x, y, w, 9, 2)`,
then white `FontSecondary` text at `(x + 3, y + 8)`.
A `str`'s y is its baseline.

```text
icon   bluetooth          2  1
str    "Connected"       12  9        FontPrimary
icon   back              91  2
hold   "EXIT"           101  1  26
line   0,11 to 127,11
icon   up                48 19
hold   "high"            58 16  23
rframe                   48 28  32 17 r3
rbox                     50 30  13 13 r2
icon   microphone        53 32        white
icon   enter             67 33
hold   "clear"           15 32  27
str    "Esc"             86 40        FontSecondary
icon   down              47 51
hold   "max"             57 48  24
```

Icons are 1-bit PNGs in `images/`, `<name>.png` for `I_<name>`, with `#` black;
a black pixel compiles to a set bit (`I_up` is `08 1c 3e 7f`).
Only the six this board draws ship; later boards bring theirs.

```text
bluetooth 7x9
...#...
...##..
.#.#.#.
..###..
...#...
..###..
.#.#.#.
...##..
...#...

back 8x7
..#.....
.##.....
#######.
.##....#
..#....#
.......#
.######.

up 7x4
...#...
..###..
.#####.
#######

down 7x4
#######
.#####.
..###..
...#...

microphone 7x10
..###..
.#####.
.#####.
.#####.
#.###.#
#.....#
.#...#.
..###..
...#...
.#####.

enter 9x7
........#
........#
..#.....#
.##.....#
#########
.##......
..#......
```

## Risks / Trade-offs

- Nothing has run on a device → task 2.2 is the first run; a failure stops the apply.
- `loader close` is read from the firmware, not observed → task 2.4 checks for `App closed`.
- The `g` of `high` notches its plate's lower edge at (71, 24) → the board shows the same; it is the font's descender.
- `view.h` shares its name with the SDK's `gui/view.h` → `"view.h"` resolves to ours and `app.c` never includes the other; adopting `ViewDispatcher` would force a rename.
- A button event arrives after the loop stops → `gui_remove_view_port` runs before the queue goes, and eight slots outlast the teardown.
- `Connected` shows with no radio behind it → the spec says so, and `ble-transport` replaces it.

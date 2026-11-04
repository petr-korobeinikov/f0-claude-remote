# Design

## Context

See `proposal.md` for why.
Firmware facts below come from SDK 1.5.1's headers and `runfap.py`,
and from the disassembly of its `firmware.elf`.

The draw list and icons come from board `D_Main`
of the wireframe, version `1793726706-f24f`.
Text needs the firmware's fonts, so task 2.2 compares a frame read off the device with the board's pixels.

## Goals / Non-Goals

**Goals:** board `D_Main` on the device; a way out; `mise run launch` still working over a running copy.

**Non-Goals:** D's other states, not drawn yet; any button but Back; vibration; Bluetooth; the lock; a host test setup.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/view.h` | UI | Declares the screen's draw call | Compiled by `app.c` and `view.c` | The header |
| `src/view.c` | UI | Draws the screen | Against the board's pixels, on the device | `Canvas*`, all it touches |
| `src/app.c` | Platform | Runs the application's event loop | Buttons and `mise run launch`, on the device | The device's buttons and `ufbt launch` |
| `images/*.png` | UI | Hold one bitmap each | Read back against the grids below | `I_*` in the generated header |

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
- **`exit` at the lower left,**
  not by Back on the case:
  stock remotes put it there, its icon names the button, and on the right it merges with `max`.
- **`application.fam` keeps fbt's default `sources`,**
  not an explicit `src/` list:
  `*.c*` is recursive, so `src/` needs no manifest change.

`app_main` allocates the loop and the queue and subscribes,
sets up the `ViewPort` and adds it, then runs the loop.
It tears down in reverse:
`gui_remove_view_port`, the record, the `ViewPort`,
the subscription, the queue, the loop.

`view.c` assembles the screen from static components:
`view_draw_status_row`, `view_draw_ring`, `view_draw_ok_button`, `view_draw_side_actions`,
and `view_draw_split`, which the legend, Up and Down share.

The draw list, in order, black unless noted, `FontSecondary` from `TAP` on.
`split x y w h` is a white `box(x, y, w, 11)`, which clears the ring under Up and Down,
then `rframe(x, y, w, 11, 2)`, `rbox(h, y, x + w - h, 11, 2)`, `box(h, y, 2, 11)`.
A `str`'s y is its baseline.

```text
icon   bluetooth          2  1
str    "Connected"       12  9        FontPrimary
split                    74  0  53 97
str    "TAP"             77  9
str    "HOLD"           100  9        white
line   0,12 to 127,12
circle                   60 38 r20
split                    54 14  35 66
icon   up                57 17
str    "high"            69 22        white
split                    54 52  36 66
icon   down              57 55
str    "max"             69 61        white
rframe                   45 28  30 21 r2
rbox                     45 38  30 11 r2
box                      45 38  30  2
str    "Enter"           48 37
str    "voice"           49 47        white
rbox                      3 33  34 11 r2
str    "clear"            6 42        white
icon   left              30 35        white
rframe                   84 33  28 11 r2
icon   right             87 35
str    "Esc"             94 42
rbox                      3 52  33 11 r2
icon   back               6 54        white
str    "exit"            17 61        white
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

left 4x7
...#
..##
.###
####
.###
..##
...#

right 4x7
#...
##..
###.
####
###.
##..
#...
```

## Risks / Trade-offs

- In stock remotes a fill means pressed now, here hold → settled before `app-keymap` draws its sent state.
- `view.h` shares its name with the SDK's `gui/view.h` → `"view.h"` resolves to ours and `app.c` never includes the other; adopting `ViewDispatcher` would force a rename.
- A button event arrives after the loop stops → `gui_remove_view_port` runs before the queue goes.
- The recursive glob would link `test/*.c` into the FAP → `app-keymap` excludes `test`, as fbt excludes `lib` (`"!test"`).

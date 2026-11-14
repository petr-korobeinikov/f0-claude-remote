# Design

## Context

See `proposal.md`, Why.
All of it lands in the UI: `view_idle.c` draws the board, and one image holds the outline.
No rule moves, so the core, platform-specific code and the host suite stay.

| Fact | Where |
|---|---|
| `D_Main` is its board of `1794586781-3562` with rows 0-13 as they were, the outline at (38, 15) and each element moved whole: the four plates and OK 12 px left, Back's plate 14 left and 4 up | The canvas, `1794652954-5503`, against the board kept beside the repository: 0 of 8,192 cells apart |
| Each `D_sent_*` board is `D_Main` with its flash, and `D_usb_main` is `D_Main` under the USB mark | Their frames against `D_Main`'s: the flash's cells alone apart, and 19 in the mark's box |
| The outline is 206 black cells of 4,094, closed; 71 lie under the ring's four plates, none under OK or Back's plate | `D_Main`'s 89×46 picture, the grid below |
| An icon paints its whole box, the clear cells in the other color: the bitmap mode is off, but around the status bar's own drawing | `canvas.c:310-316`, `u8g2_setup.c:91`, `gui.c:92-94`, firmware 1.5.1; `canvas.h:408-413` documents the mode, not its default |
| The canvas decodes an icon into 1,024 bytes and halts on one that does not fit; this one takes 552 | `canvas_i.h:15`, `canvas.c:19`, `compress.c:68-85`, firmware 1.5.1 |

## Goals / Non-Goals

**Goals:** the specs', with every board `view.c` draws as before.

**Non-Goals:** Back's second action and its mark;
flash boxes derived from the plates, as each stays one row of its board
(`openspec/changes/archive/2026-11-14-side-arrows/design.md:22`).

## Decisions

`view_idle.c` keeps its task (`docs/internals/architecture.md:33`) and `view_idle_draw` its five components,
their calls in the order of `D_Main`'s rows; `recess.png` holds one bitmap.

- **The recess is drawn first,**
  not under `canvas_set_bitmap_mode(canvas, true)`, a state to set and to restore:
  drawn before what it holds, it gives one frame in both modes,
  so nothing rests on the undocumented default.
- **Its call stands in `view_idle_draw_ring`, where `view_idle_draw_recess` was called, and that function goes,**
  not kept around one call:
  the image's name says what is drawn, and the comment on the order stands at the line it binds.

```c
static void view_idle_draw_ring(Canvas* canvas) {
    /* Comes first: an icon paints its whole box, the clear cells too. */
    canvas_draw_icon(canvas, 38, 15, &I_recess);
    view_idle_draw_outline(canvas, 3, 14, 65);
    view_idle_draw_fill(canvas, 3, 14, 53);
    view_idle_draw_white_icon(canvas, 6, 15, &I_slash);
    view_idle_draw_white_str(canvas, 10, 22, "effort high");
    canvas_draw_icon(canvas, 58, 17, &I_up);
    view_idle_draw_outline(canvas, 2, 52, 66);
    view_idle_draw_fill(canvas, 2, 52, 54);
    view_idle_draw_white_icon(canvas, 5, 54, &I_slash);
    view_idle_draw_white_str(canvas, 9, 61, "effort max");
    canvas_draw_icon(canvas, 58, 55, &I_down);
}
```

```c
static void view_idle_draw_side_actions(Canvas* canvas) {
    view_idle_draw_outline(canvas, 3, 33, 40);
    view_idle_draw_fill(canvas, 3, 33, 31);
    view_idle_draw_white_icon(canvas, 6, 35, &I_slash);
    view_idle_draw_white_str(canvas, 10, 42, "clear");
    canvas_draw_icon(canvas, 36, 35, &I_left);
    view_idle_draw_outline(canvas, 80, 33, 30);
    view_idle_draw_fill(canvas, 89, 33, 21);
    canvas_draw_icon(canvas, 83, 35, &I_right);
    view_idle_draw_white_str(canvas, 92, 42, "Esc");
    canvas_draw_rbox(canvas, 78, 46, 33, 11, 2);
    view_idle_draw_white_icon(canvas, 81, 48, &I_back);
    view_idle_draw_white_str(canvas, 92, 55, "exit");
}
```

In `view_idle_draw_ok_button` the frame stands at `46, 28`, each row runs from `47` to `101 - y`,
the microphone stands at `49, 30` and the Enter arrow at `64, 39`.
The flashes: ▲ `56, 15, 11, 9`, ▼ `56, 53, 11, 9`, ◀ `34, 34, 8, 9`, ▶ `81, 34, 8, 9`,
`/effort high` `4, 15, 51, 9`, `/effort max` `3, 53, 52, 9`, `/clear` `4, 34, 29, 9`, Esc `90, 34, 19, 9`,
and Enter's rows from `102 - y` to `74`.
The comment on `view_idle_draw_outline` becomes
`/* Clears first: the recess runs under the plates that lie on it. */`.

`recess.png` is this grid, `#` black:

```text
...................#########.............................................................
...............####.........####.........................................................
.............##.................##.......................................................
...........##.....................##.....................................................
..........#.........................##...................................................
.........#............................##.................................................
.......##...............................##...............................................
......#...................................##.............................................
......#.....................................##...........................................
.....#........................................##.........................................
....#...........................................##.......................................
...#..............................................##.....................................
...#................................................##...................................
..#...................................................##.................................
..#.....................................................##...............................
.#........................................................##.............................
.#..........................................................##...........................
.#............................................................##.........................
.#..............................................................##.......................
#.................................................................##.....................
#...................................................................##...................
#.....................................................................##.................
#.......................................................................##...............
#.........................................................................##.............
#...........................................................................##...........
#.............................................................................##.........
#...............................................................................##.......
#.................................................................................##.....
.#..................................................................................##...
.#....................................................................................#..
.#....................................................................................#..
.#.....................................................................................#.
..#....................................................................................#.
..#....................................................................................#.
...#....................................................................................#
...#....................................................................................#
....#...................................................................................#
.....#..................................................................................#
......#.................................................................................#
......#................................................................................#.
.......##..............................................................................#.
.........#.............................................................................#.
..........#...........................................................................#..
...........##.........................................................................#..
.............##.....................................................................##...
...............#####################################################################.....
```

Test seams:

- `recess.png`: read back against the grid.
- `view_idle.c`, which only renders: the frame over RPC, equal to `D_Main`,
  then to each `D_sent_*` board as the owner acts.

## Risks / Trade-offs

- A firmware paints or decodes an icon otherwise than 1.5.1's sources read → task 2.1's frame, 0 cells apart.
- A flash left at its old `x` inverts cells beside its element → task 3.1's nine frames.
- A cell of the image under a plate shows in no frame → task 1.1's read-back.
- The canvas moves past `1794652954-5503` before the apply → the tasks name the version; a newer one stops the change for a replan.

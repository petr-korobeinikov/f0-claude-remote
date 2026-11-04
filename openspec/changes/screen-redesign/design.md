# Design

## Context

See `proposal.md` for why.
The boards are `D_Main`, `D_sent_*` and `D_voice` of the wireframe,
version `1793790945-7273`;
read off their frames, each `D_sent_*` differs from `D_Main` in its flash alone,
and `D_voice` stays as `app-keymap` left it.
In SDK 1.5.1 `canvas_draw_triangle` only outlines,
and `furi_event_loop_timer_start` starts a timer "or restart[s] it with a new interval".

## Goals / Non-Goals

**Goals:** the boards' layout, cell for cell; seven flashes with two durations.

**Non-Goals:** `transport`; D's disconnected board; any change to what a button does or to its buzz.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/feedback.h`, `.c` | Core | Says how an action confirms itself | `test/feedback_test.c` | `Action` in, a value out |
| `src/view.c` | UI | Draws the screen | Frames against the boards | `Canvas*`, all it touches |
| `src/screen.h`, `.c` | Platform | Keeps what the display shows | Frames, on the device | `view_draw` |
| `src/app.c` | Platform | Runs the application's event loop | Buttons, on the device | `screen.h`, `feedback.h` |

No code moves to the platform side:
`screen` and `app` stay there for `FuriEventLoopTimer` and `NotificationApp`, as `app-keymap` named.

```c
uint32_t feedback_flash_ms(Action action);
void screen_flash(Screen* screen, Action action, uint32_t duration_ms);
```

- **`app` hands the duration to `screen_flash`,**
  not `screen` calling `feedback`:
  `app` already asks `feedback`, and `screen` keeps holding no rule.
- **One timer, restarted with each flash's interval,**
  not one per duration:
  the header promises the restart, and two timers would race over one word.
- **A loop over rows for each half of the diagonal,**
  not a bitmap per state:
  three lines each, with the other elements' primitives, and in XOR each row touches a cell once.
- **`view_draw_outline` clears and frames, and `view_draw_split` builds on it,**
  not a clear before every element:
  Esc and both splits lie on the ring.
- **No clear under the `/clear` plate,**
  against row 46 of the board's table:
  the fill covers every cell of the ring there, and the frame still equals the board.
- **`view.c` stays one file, at 136 lines,**
  not split now:
  23 of them are the flash's `switch`, and `ble-transport`'s third board shows where the seam lies.

The idle board, in drawing order, after the status row `app-keymap` left; baselines as y:

```text
legend  split 74 0 53, tap at 104   str HOLD 77 9   str TAP 106 9
ring    circle 73 38 23
up      split 15 14 65, tap at 68   slash 18 15   str "effort high" 22 22   up 70 17
down    split 14 52 66, tap at 68   slash 17 54   str "effort max" 21 61    down 70 55
ok      rframe 58 28 30 21, y 29-47: line 59 y to 113-y y
        microphone_9x13 61 30   enter 76 39
clear   rbox 17 33 38 11   slash 20 35   str clear 24 42   left 48 35
esc     outline 92 33 28   right 95 35   str Esc 102 42
exit    rbox 94 52 33 11   back 97 54    str exit 108 61
```

The flash, in XOR, last on the idle board:

```text
up     68 15 11 9
down   68 53 11 9
enter  y 29-47: line 114-y y to 86 y
high   16 15 51 9
max    15 53 52 9
clear  18 34 36 9
esc    93 34 26 9
```

The new icons, 1-bit PNGs like the rest:

```text
slash   enter       microphone_9x13
..#     ........#   ...###...
..#     ........#   ..#####..
.#.     ..#.....#   ..#####..
.#.     .##.....#   ..#####..
.#.     #########   ..#####..
#..     .##......   #.#####.#
#..     ..#......   #.#####.#
                    #..###..#
                    .#.....#.
                    ..#####..
                    ....#....
                    ....#....
                    ..#####..
```

## Risks / Trade-offs

- The LCD's lag washes out a 100 ms flash → the owner's device check; the number is one line in `feedback.c`.
- During Enter's flash OK reads all black, so tap and hold look alike → the board's accepted weak spot, and the buzz confirms.
- Voice stopped within Enter's 400 ms shows the flash's remainder → under 100 ms, only after a tap and a hold back to back; accepted.
- CD revises a board after this plan → the version above pins them, and a newer one is a change of its own.

# Design

## Context

See `proposal.md`, Why.
The change lands in the UI, in `src/ui/view.c` alone:
no rule moves between layers, and no header changes.

| Fact | Where |
|---|---|
| The primary font is `u8g2_font_helvB08_tr` | `canvas.c:159-160`, firmware 1.5.1 |
| `canvas_draw_str_aligned` starts a string half its width left of `x` under `AlignCenter`, leaves `y` the baseline under `AlignBottom`, and then draws as `canvas_draw_str` does | `canvas.c:178-227` |
| A string's width is the sum of its glyphs' advances, the last one's replaced by its width and offset | `lib/u8g2/u8g2_font.c:1016-1051` |
| `Reconnecting...` advances 72 px up to its last dot, 2 px wide: 74 px, and `64 - 74 / 2` is 27 | The font's glyphs, in the SDK's `firmware.elf` |
| `Reconnect stopped` is 93 px wide: centered by that call, it would move from 17 to 18 | The same glyphs; `src/ui/view.c:62` |
| The marks are one size by pair: 7×9 with a link, 10×9 with none | `asset/image/bluetooth.png`, `usb.png`, `bluetooth_off.png`, `usb_off.png` |
| `canvas_draw_icon` stops the application at an icon that is `NULL` | `canvas.c:401` |
| `screen_draw_callback` holds the one call of `view_draw` | `src/platform/screen.c:17-20` |

## Goals / Non-Goals

**Goals:** the proposal's, with every board as before.

**Non-Goals:** the strings no link picks, each of which keeps its `x`:
centering them by the same call would move `Reconnect stopped`.

## Decisions

| File | Layer | Its one task | What changes |
|---|---|---|---|
| `src/ui/view.c` | UI | Draws a mode's board | `ViewLink`, `view_links`; four functions read their row |

The table stands above the first function:

```c
typedef struct {
    const Icon* mark;
    const Icon* no_link_mark;
    const char* no_link_message;
    const Icon* switch_mark;
} ViewLink;

static const ViewLink view_links[LinkMAX] = {
    [LinkBluetooth] =
        {
            .mark = &I_bluetooth,
            .no_link_mark = &I_bluetooth_off,
            .no_link_message = "Reconnecting...",
            .switch_mark = &I_usb,
        },
    [LinkUsb] =
        {
            .mark = &I_usb,
            .no_link_mark = &I_usb_off,
            .no_link_message = "Connect the cable",
            .switch_mark = &I_bluetooth,
        },
};
```

`view_draw_status_row` draws `view_links[link].mark`,
`view_draw_no_link_status_row` `view_links[link].no_link_mark`
and `view_draw_switch_plate` `view_links[link].switch_mark`,
each where its `?:` stood,
and `view_draw_no_link`'s `if` becomes one call:

```c
    canvas_draw_str_aligned(
        canvas, 64, 28, AlignCenter, AlignBottom, view_links[link].no_link_message);
```

Every function keeps its signature, so `view_draw` and `screen.c` stay as they are.

- **Each function reads its own field of `view_links[link]`,**
  not a row that `view_draw` finds once and hands down:
  `link` then means the `Link` in every function, as now, and the diff is the four reads.

Test seams:

- `view.c` has no host test, as it only renders the state it is handed.
  Its seam is `view_draw`, which `screen_draw_callback` alone calls:
  handed a mode and a link there in place of the screen's own,
  it shows that board on the device whatever link is up,
  so USB's boards are read as frames on Bluetooth, where the console stays.
- Tasks 1.1 and 2.2 read four boards so, before the rewrite and after.
  On each link `ModeIdle` draws the row's `mark` and `ModeNoLink` its other three fields (`view.c:72-75`, `:79-83`):
  all eight cells.
  The six boards not read call the same functions with the same link (`:17`, `:61`, `:85`).
- `mise run build` types each cell: a message in a mark's place stops it.
  Two marks that changed places build, and a frame differs.

## Risks / Trade-offs

- A cell names the wrong mark, or a message moves → a frame of task 2.2 differs from task 1.1's.
- A link added later with no row → its mark reads `NULL`, and `canvas_draw_icon` stops the application at its first frame, where the `?:` drew Bluetooth's.
- The seam's line stays in `screen.c` → tasks 1.1 and 2.2 each close on `git diff --exit-code -- src/platform/screen.c` and a launch with the line out.

# Design

## Context

See `proposal.md`, Why.
The rule lands in the core, in `keymap.c`'s table;
the keys' usages in platform-specific code, in `transport_hid.c`, as the firmware's names;
the plates in the UI, in `view_idle.c`.

| Fact | Where |
|---|---|
| A hold sends `Long` 300 ms after `Press` and no `Short` at its release | `input.c:13-14`, `:43-45`, `:146-154`, firmware 1.5.1 |
| The arrows' usages are `HID_KEYBOARD_LEFT_ARROW` and `HID_KEYBOARD_RIGHT_ARROW` | `hid_usage_keyboard.h:106-107`, SDK 1.5.1 |
| At radius 2, `canvas_draw_rframe` and `canvas_draw_rbox` leave out their four corner cells alone | `lib/u8g2/u8g2_box.c:76-200`, `u8g2_circle.c:81-112`, `:169-200`, firmware 1.5.1 |
| → takes a suggested prompt | code.claude.com/docs/en/interactive-mode, `Prompt suggestions` |
| The plates are rows 49-61 of `D_Main`'s table, and each flash row 65 of its `D_sent_*` board's | The canvas, `1794586781-3562` |

## Goals / Non-Goals

**Goals:** the specs', with every other cell of `D_Main` as before.

**Non-Goals:** flash boxes derived from the plates, as each stays one row of its board;
a way to open the slider from the device.

## Decisions

Each source file keeps its task (`docs/internals/architecture.md:23-42`),
and `view_idle_draw` its five components.

| File | What changes |
|---|---|
| `src/core/action.h` | `ActionArrowLeft`, `ActionArrowRight`, after `ActionArrowDown` |
| `src/core/transport.h`, `src/platform/transport_hid.c` | `TransportKeyArrowLeft`, `TransportKeyArrowRight`, in the enum's alphabet, each with its row of `transport_hid_keys` |
| `src/core/keymap.c` | Two rows; the comment on Esc goes with its reason |
| `src/core/actions.c`, `feedback.c`, `deed.c` | An arrow is a tap of its key, a buzz, 100 ms, no deed |
| `src/ui/view_idle.c` | The fill, both side plates, four flashes |

In `keymap.c`:

```c
    [KeymapKeyLeft] = {[KeymapEventShort] = ActionArrowLeft, [KeymapEventLong] = ActionClear},
    [KeymapKeyRight] = {[KeymapEventShort] = ActionArrowRight, [KeymapEventLong] = ActionEsc},
```

In `view_idle.c`, after `view_idle_draw_outline`, and opening `view_idle_draw_side_actions`:

```c
/* Stops short of the outline's rows, where it would square the plate's corners. */
static void view_idle_draw_fill(Canvas* canvas, int32_t x, int32_t y, size_t width) {
    canvas_draw_box(canvas, x, y + 1, width, 9);
}
```

```c
    view_idle_draw_outline(canvas, 15, 33, 40);
    view_idle_draw_fill(canvas, 15, 33, 31);
    view_idle_draw_white_icon(canvas, 18, 35, &I_slash);
    view_idle_draw_white_str(canvas, 22, 42, "clear");
    canvas_draw_icon(canvas, 48, 35, &I_left);
    view_idle_draw_outline(canvas, 92, 33, 30);
    view_idle_draw_fill(canvas, 101, 33, 21);
    canvas_draw_icon(canvas, 95, 35, &I_right);
    view_idle_draw_white_str(canvas, 104, 42, "Esc");
```

The legend's, Up's and Down's splits each become their outline and a fill,
at `74, 0, 30`, `15, 14, 53` and `14, 52, 54`.
The flashes: ◀ `46, 34, 8, 9`, ▶ `93, 34, 8, 9`, `/clear` `16, 34, 29, 9`, Esc `102, 34, 19, 9`.

- **`view_idle_draw_split` gives way to `view_idle_draw_fill`,
  a box between the outline's rows, at the `x`, `y` and width the boards give the fill,**
  not a mirrored copy, a side under a `switch`, nor one split of six arguments:
  such a box has no corner to square, so it takes no joint and names no side,
  each call reads as one row of a board's table,
  and, by the third fact, it draws the `rbox` and joint's cells.

The tests:

| Test | Change |
|---|---|
| `test_arrow_left_sends_arrow_left`, `test_arrow_right_sends_arrow_right` | New, in `actions_test.c` |
| `test_sent_actions_vibrate`, `test_arrows_flash_for_100_ms`, `test_the_rest_earn_nothing` | Both arrows in each |
| `test_tap_acts`, `test_hold_acts` | The side taps; Right's hold |
| `test_tap_on_left_or_back_does_nothing` | `test_tap_on_back_does_nothing` |
| `test_press_acts_only_on_right` | `test_press_never_acts`, over every key |
| `test_right_acts_on_press_alone` | Goes: `test_tap_acts`, `test_hold_acts` and `test_press_never_acts` hold its asserts |
| `test_right_pressed_on_usb_idle_returns_nothing`, `test_right_pressed_on_idle_takes_no_usb` | The press answers `ActionNone` |
| `test_right_held_elsewhere_on_usb_returns_nothing` | `test_right_held_elsewhere_on_usb_keeps_usb`; the main screen's hold answers `ActionEsc` |
| `test_right_held_elsewhere_takes_no_usb` | Its loop asserts the link alone |

In `docs/guide/using-the-remote.md` the `Right, press` row goes, and three rows follow `Down, tap`:

```text
| Left, tap | Left arrow | Lowers the `/effort` slider a level, or moves to a dialog's previous tab |
| Right, tap | Right arrow | Raises the `/effort` slider a level, moves to a dialog's next tab, or takes a suggested prompt |
| Right, hold | Esc | Stops the turn |
```

Lines 27-28 become "Tapping Back does nothing." and
"On an empty prompt the left arrow sends the session to the background and opens agent view:
Esc returns, and `leftArrowOpensAgents` in `/config` turns that off.".
`docs/guide/troubleshooting.md:16` names a tap on Back.
`docs/internals/usb.md:26` becomes
"With no link no key is sent, so Right's hold, Esc with the link up, switches the link there,".
In `catalog/description.md`, line 19 opens `**Right, hold:**`, after a new bullet,
"**Left and Right, tap:** the arrows, for the effort slider and a dialog's tabs".

Test seams:

- `keymap.c`: the host suite, through `keymap.h`; red is `test_tap_acts`' assert on Left.
- `actions.c`, `feedback.c`, `deed.c`: their suites; red is the build, at the undeclared names, then at each switch.
- `transport_hid.c`, with no host test: the build types its rows, a `git grep` counts them, and the trial sends both keys.
- `view_idle.c`, which only renders: the frame over RPC, equal before and after the fill, then to each board.

## Risks / Trade-offs

- A press released early sends the right arrow for Esc → the ▶ cell flashes, not `Esc`'s, and the hold is 300 ms.
- A row of `transport_hid_keys` left out sends usage 0 with no warning → task 3.1's `git grep`, and the trial.
- The box draws a cell apart from the `rbox` and its joint → task 1.1's two frames.

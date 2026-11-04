# Proposal

## Why

On the device, a tap on an arrow or on OK showed nothing:
`app-keymap`'s check 3.4 found it, and the owner ordered a revision of screen D.
Wireframe version `1793790945-7273` holds it:
the ring moves right, as the buttons sit on the device's body,
labels are whole (`/effort high`, not `high`),
and seven actions flash instead of four.
The owner put it before `ble-transport`,
so D's disconnected board gets drawn on this layout.

Decisions, already settled:

- **Arrows flash 100 ms, Enter and the commands 400 ms,**
  not 400 ms for all:
  arrows come in series, and below about 6 taps a second 100 ms leaves a pause between flashes, per the board's note `sent`.
- **OK is split by a 45° diagonal, the microphone on the fill at the upper left, an Enter arrow at the lower right,**
  not `Enter` over a microphone, a vertical seam or a tilted microphone:
  the owner's call of 2026-11-04 from four mockups, and fill first keeps the legend's `HOLD | TAP` order.
- **The diagonal at `x + y <= 113` and a 3×7 slash icon,**
  not `x + y <= 110` and the font's slash:
  the owner's call on seeing the first drawing, where the microphone crowded the seam and the font's slash looked foreign and cost 4 px.

Decisions proposed here, for the owner to strike:

- **`feedback` gives the duration, 0 for no flash,**
  not `feedback_flashes` beside a second function, nor the number in `screen`:
  how long an action flashes is a rule, and rules stay in the core.
- **One flash at a time,**
  not a timer per element:
  a new action replaces a running flash, as commands already do, and `Action flash` stays one word.
- **The owner judges the 100 ms by eye,**
  not frames alone:
  the frames prove the display's memory, and the board itself asks whether the LCD's lag eats the pause.

## What Changes

- `src/view.c` draws the new layout and seven flashes.
- `src/feedback.*`: `feedback_flash_ms` replaces `feedback_flashes`.
- `src/screen.*`, `src/app.c`: a flash takes its duration from `feedback`, and `SCREEN_FLASH_MS` goes.
- `test/feedback_test.c` pins the durations.
- `images/` gains `enter.png`, `slash.png` and `microphone_9x13.png`.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-screen`: the idle layout changes, and Enter and the arrows start to flash, with two durations.

## Impact

No tool or dependency enters `mise.toml`.
A trial build takes the package from 8.8 KB to 9.9 KB, with the same 38 imports.
`button-actions` stays as it is: what each button does, and its buzz.

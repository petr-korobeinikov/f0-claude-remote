# Design

## Context

See `proposal.md`, Why.
The change lands in the core alone, in `src/core/keymap.c`,
whose one task stays mapping an event to an action given the mode;
`test/keymap_test.c` checks it through `keymap.h`, which stays as it is.
A firmware bump can break the second list:

- **Documented** (`input.h:22-26`):
  a tap's `Short` comes with its release, a hold's `Long` once the long-press interval passes,
  each after the button's `Press`.
- **Firmware 1.5.1 sources alone:**
  `Long` fires 300 ms after `Press` (`input.c:13-14`), and `Short` goes out just before `Release` (`input.c:146-154`);
  the GUI hands the rest of a press only to the view port its `Press` reached,
  and of that only the `Release` once another view port is on top (`gui.c:310-333`).
- **Seen:** `usb-transport`'s task 6.3, Up on `D_usb_stopped` with the cable in bringing `D_usb_main` at once.

## Goals / Non-Goals

**Goals:** the spec's rule in the core, pinned on the host.

**Non-Goals:** Esc on Right's press and the stopped screen's press, which act on `Press` itself;
a flash, which changes no mode; the review's other findings, each a change of its own.

## Decisions

A flag per button, and what `keymap_handle` does with it:

| Event | The button's flag | Answer |
|---|---|---|
| `Press` | Set | As before |
| `Short`, `Long` | Set | As before |
| `Short`, `Long` | Clear | `ActionNone`; Back's `Long` exits as before |
| `Repeat` | Either | `ActionNone`, as before |
| `Release` | Cleared | As before: OK's ends voice or `ModeVoiceCut` |
| A new mode, or the link switched | All cleared | — |

- **`bool pressed[KeymapKeyMAX]` in `Keymap`,**
  not each press's `Mode` kept and compared,
  which needs a value for no press that `Mode` lacks.
- **`right_pressed_on_no_link` goes, and the switch reads Right's flag,**
  not kept beside the flags: it is the same rule for one button.
- **One function sets the mode and clears the flags when the mode differs, and the switch clears them too,**
  not a clear at each place that sets `keymap->mode` now:
  one forgotten clear reopens the bug.
- **The check stands at the top of `keymap_handle`, for `Short` and `Long` only, and lets `ActionExit` through,**
  not inside each mode's case:
  one place states the rule, and Back's exemption is one comparison.
- **A `Short` or `Long` with no `Press` before it does nothing,**
  as the GUI never sends one (`gui.c:310-333`);
  the tests that sent one send the `Press` first, as the firmware does.

## Risks / Trade-offs

- A link that drops and returns within a tap swallows it → the board showed the drop, and a second tap acts.
- A hold whose screen changed does nothing → it is held again; Back's, which only leaves, is spared.
- Tests now send the presses they skipped → a path with no `Press` loses its pins; the GUI never takes it.

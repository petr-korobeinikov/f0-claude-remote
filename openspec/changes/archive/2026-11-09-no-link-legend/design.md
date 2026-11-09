# Design

## Context

See `proposal.md`, Why.
The change lands in the UI layer alone, in `src/ui/view.c`, whose one task stays drawing the screen for a mode.
The geometry is rows 10 and 11 of the tables on `D_disconnected` and `D_usb_disconnected`.

## Decisions

- **`view_draw_hold_legend` in `view.c`,**
  not `view_idle_draw_legend` with a flag:
  that one draws a frame, `TAP` and a squared joint, and `HOLD` alone needs none of them.
- **`view_draw` calls it on `ModeNoLink`, beside `view_draw_switch_plate`,**
  not `view_draw_no_link_status_row`:
  `D_disc_voice` and `D_stopped` share that row, and they show no legend.
- **No host test:**
  `view` only renders the mode it is handed, and the seam is the screen itself, checked by a look.

## Risks / Trade-offs

None: no state or input changes.

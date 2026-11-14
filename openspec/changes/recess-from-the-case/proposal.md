# Proposal

## Why

The idle screen draws its recess as a ring joined to a lobe (`src/ui/view_idle.c:38-48`).
On the canvas that shape gave way to one traced from a photo of the case,
which the note `backlogMain`, item 6, calls accepted:
at version `1794652954-5503` it stands on `D_Main`, `D_usb_main` and the nine `D_sent_*` boards,
with the ring and OK 12 px to the left (the note `noteD`).
`app-screen` holds the device's frame to those boards by name
(`openspec/specs/app-screen/spec.md:51-53`, `:101-110`, `:219-222`),
while the code draws `D_Main` as it stood at `1794586781-3562`
(`openspec/changes/archive/2026-11-14-side-arrows/tasks.md:21-22`),
2,176 of 8,192 cells from today's, by the board kept beside the repository.
The shift frees 12 px beside Back's plate for its second action (`noteD`; `backlogMain`, item 7),
so the layout comes first, as a change of its own.

Decisions:

- **The image is `asset/image/recess.png`, hence `I_recess`,**
  not the canvas's `I_recess_settings`:
  that name comes from the boards it was drawn on, `D_exp_recess_settings` and `D_exp_recess_settings_esc`,
  whose Back also opens settings, and the app has none.
- **The owner retakes `asset/screenshot/main.png` in qFlipper,**
  as `side-arrows` settled (`openspec/changes/archive/2026-11-14-side-arrows/proposal.md:34-37`).
- **One trial by the owner on a scratch session, its frames logged, and no press over RPC,**
  as there (`:38-39`), with each of the nine actions once:
  every flash moved, each on a board of its own, so none proves another.
- **Nothing is tried on USB:**
  `view_idle_draw` takes no link (`src/ui/view_idle.h:7`),
  and `D_usb_main` is `D_Main` under the mark `view.c` draws (`src/ui/view.c:32`).
- **Relatively simple, so implemented on `develop`:**
  one file of the UI is redrawn from its board's table and one image joins it,
  with no rule and no suite changed.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `feat(view): trace the recess from the case`.**

## What Changes

- `D_Main` shows the buttons in the recess traced from the case:
  the ring and OK 12 px to the left, and Back's plate under `Esc`, at (78, 46),
  with 12 px of the recess left empty on its right, as `noteD` gives them.
- Each of the nine flashes moves with its element.
- One image holds the outline, drawn by one call, as row 9 of `D_Main`'s table gives it,
  in place of the four shapes of `view_idle_draw_recess`.
- The screenshot follows.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-screen`: the ring and Back's plate stand in one outline, the case's recess, with Back's plate under `Esc`.

## Impact

`src/ui/view_idle.c`, `asset/image/recess.png`, new, and `asset/screenshot/main.png`.
`view.c`, the core, platform-specific code and `test/` stay as they are.
So do `README.md` and `docs/`, but for the screenshot they show:
the guide's two lines on the ring (`docs/guide/using-the-remote.md:42`, `:52`) hold as written.
`app-screen`'s other requirements hold as written, on the boards the canvas redrew with `D_Main`;
so do `bluetooth-link`, `usb-link` and `button-actions`, which name `D_Main` and `D_usb_main` and no cell of either.

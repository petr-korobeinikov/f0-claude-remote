# Proposal

## Why

`src/ui/view.c` asks which link is in use in four places:
`link == LinkUsb` picks the status row's mark (`:8`), the mark with no link (`:27`)
and the mark on the switch plate (`:49`),
and an `if` on it picks the message under `No link` between two draw calls (`:35-39`).
Each names USB and leaves every other link to its `else`:
what a link shows is spread over four functions,
and a third link would draw Bluetooth's boards, unasked.
It is the question `link-from-the-keymap` took out of `transport_hid.c`, which link is live,
answered there since by a table of two rows (`src/platform/transport_hid.c:25-28`),
and that plan left it in `view.c` as a non-goal
(`openspec/changes/archive/2026-11-12-link-from-the-keymap/proposal.md:12`, `:34-38`, and its `design.md:22-23`).
It comes now, with no plan queued
and `LinkMAX` in place to size the view's table (`src/core/link.h:6`, `916b426`).

Decisions:

- **One table in `view.c`, a row for each link, holds the four things a board shows by it,**
  not four tables of one column each:
  what a link shows then reads in one place,
  and a new link is one row there, as it is one in `transport_hid.c`.
- **One call draws both messages, centered on 64, as USB's is drawn now,**
  not an `x` kept in each row:
  `Reconnecting...` is 74 px wide, so the call starts it at 27, where `view.c:38` puts it (`design.md`, Context).
- **The mark on the switch plate is a field of the row,**
  not the mark of the link the keymap switches to (`src/core/keymap.c:27-30`), which the view is not handed:
  a board labels its buttons itself, as `src/ui/view_idle.c:51-84` does,
  and the target would cross `screen.c` as a fourth word for one label (`src/platform/screen.c:12-14`).
- **Four frames prove it, read before the rewrite and after:
  `D_Main`, `D_disconnected` and their USB twins,
  shown by handing `view_draw` each mode and link in `screen.c`'s draw callback,**
  not the boards reached by turning the Mac's Bluetooth off and holding Right, seen by eye on USB,
  where the console goes (`docs/internals/usb.md:34-37`):
  the four draw all eight cells of the table, take no hand and send no key.
- **No spec delta: `.openspec.yaml` sets `skip_specs: true`,**
  as no spec-level behavior changes:
  every board stays, pixel for pixel,
  and `app-screen` already says what a link changes on one (`openspec/specs/app-screen/spec.md:213-217`).
- **Relatively simple, so implemented on `develop`:**
  one file, four reads of one table, and no rule, header or test moves.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `refactor(view): keep what a link shows in a table`.**

## What Changes

- `view.c` gains one table with a row for each link:
  its mark, its mark with no link, its message under `No link` and the mark on its switch plate.
- The four functions that compared the link read their row,
  and no line of `src` compares a link with an enumerator any more.
- Every board stays as it is, pixel for pixel.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

None: the change sets `skip_specs: true`.

## Impact

`src/ui/view.c` alone.
`view.h`, `view_idle`, the core, the platform, the tests, `README.md` and `docs/` stay as they are:
`docs/internals/architecture.md:31` gives `view.c`'s task, not how it picks.

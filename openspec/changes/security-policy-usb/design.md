# Design

## Context

See `proposal.md`, Why.
No app layer is touched: the change rewrites one section of `SECURITY.md`.

| Fact | Where |
|---|---|
| GitHub shows `SECURITY.md` from `develop`, its default branch | `gh repo view --json defaultBranchRef`, 2026-11-10 |
| The USB keyboard is `Claude Remote` on `0x046D`, `0xC529`, the firmware's default IDs | `src/platform/transport_usb.c:11-17`; `furi_hal_usb_hid.c:16-17` at `1.5.1` |
| The Mac listed it on 046D:C529 at 12 Mb/s | usb-transport, task 6.1 |
| `046d` is `Logitech, Inc.`, and `c529` under it `Logitech Keyboard + Mice` | the USB ID Repository, `https://usb-ids.gowdy.us/read/UD/046d`, 2026-11-10 |
| A Mac laptop with Apple silicon asked to allow the keyboard the first time | `docs/guide/usb.md:22-28` |

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** IDs of the project's own; `SECURITY.md`'s other sections.

## Decisions

| File | Its one task | What changes |
|---|---|---|
| `SECURITY.md` | Gives a reader weighing the app's risk the project's security policy | Lines 21-22, in `## What the App Can Do` |

The section reads as below, lines 21-22 becoming the seven from `To the Mac` to `as [USB]`:

```markdown
## What the App Can Do

To the Mac the device is a keyboard.
Over Bluetooth it is paired once in the Mac's settings and confirmed on the device.
Over USB nothing is paired:
holding Right on `No link` makes it a keyboard to the computer at the cable's other end,
named `Claude Remote` on Logitech's IDs `046D:C529`, the firmware's default;
a Mac laptop with Apple silicon asks to allow it the first time,
as [USB](https://petr-korobeinikov.github.io/f0-claude-remote/guide/usb/#what-the-mac-asks) says.
A press acts on whatever the Mac has focused.
The app has no lock yet,
so a stray tap on OK can send a draft or approve a permission prompt,
and whoever holds the device holds those keys.
```

- **The Mac's alert links the guide's `What the Mac asks`,**
  not Apple's page, which `docs/internals/usb.md` cites:
  the guide says what this keyboard met, and that a declined alert leaves the Mac ignoring it.

Test seam: `mise run docs-build` builds the page the link targets, with its anchor.

## Risks / Trade-offs

- The anchor is not `what-the-mac-asks` → task 1.1 reads it off the built page.

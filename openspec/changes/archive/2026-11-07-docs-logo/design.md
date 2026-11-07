# Design

## Context

See `proposal.md` for why. No app layer is touched.
Zensical 0.1.0 renders `theme.logo` as an `<img>` 1.2rem high in the header and 1.6rem in the mobile drawer;
the `modern` theme's rem is 20 px, 22 px from a 100em viewport and 24 px from 125em.

## Goals / Non-Goals

**Goals:** the spec's.

**Non-Goals:** the favicon; a new icon.

## Decisions

| File | Its one task |
|---|---|
| `docs/assets/logo.svg` | Draws the app icon's pixels |
| `zensical.toml` | Points `theme.logo` at it |

- **SVG over a scaled PNG:** a path of pixel runs with `shape-rendering="crispEdges"` stays sharp at any density;
  a PNG would need one file per scale.
- **Black on Flipper orange `#FF8200`,** as Claude Design draws it, over transparent:
  black pixels alone would vanish in the dark header.
- **A 1-pixel margin, 12×12:** at the usual 24 px each pixel is 2 CSS px, whole, and the orange frames the strokes that touch the icon's edge.

**Test seam:** a script decodes `f0_claude_remote.png` and the SVG's rectangles and compares the two grids;
the build and the header in `docs-dev`, light and dark, show the rest.

## Risks / Trade-offs

- The icon changes → the script's check, rerun, shows the logo drifting; the SVG is redrawn by hand.
- 26.4 or 28.8 px on a wide screen, 32 px in the drawer, is a fractional pixel → `crispEdges` keeps it sharp, uneven by a device pixel at most; a CSS override pinning the size was passed over as a file for a pixel.

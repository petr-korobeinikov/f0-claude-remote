# Proposal

## Why

The Flipper Apps Catalog's manifest names seven screenshots (`.github/workflows/release.yml:101-107`),
six of them added in `3f65835`,
while the README shows one, `main.png` (`README.md:3`),
and so does the site: Home (`docs/index.md:9`) and `Using the remote` (`docs/guide/using-the-remote.md:50`),
both through the one link `docs/assets/main.png`.
`Listening`, `No link` on either link, `Voice stopped: release OK`, `Reconnect stopped`
and the main screen on USB are described in words alone.
The owner asked on 2026-11-14 to show them in the README and on the site.

Decisions:

- **One folder link, `docs/assets/screenshot` to `../../asset/screenshot`, replaces `docs/assets/main.png`,**
  not a file link per screenshot:
  Zensical copies only what `docs/` holds (`openspec/changes/archive/2026-11-07-docs/proposal.md:34`),
  and one link takes every screenshot, a new one included.
  Whether Zensical copies a linked folder is unchecked, and a strict build does not fail an image outside `docs/` (same line),
  so task 1.1 compares every copied file.
  If the folder is not copied, the fallback is decided here:
  one file link per screenshot in a real `docs/assets/screenshot/`,
  and `app-build`'s `Paths` scenario lists them, fixed in place.
- **On the site each screenshot stands above the paragraph of its screen,** as `main.png` does now,
  not in a gallery, which would part a screen from what the guide says of it.
  `Using the remote` takes six, both `No link` frames above its paragraph;
  `USB` takes `main-usb.png` above the paragraph on the status row's USB mark and `Connected`.
- **The README keeps `main.png` on top and gains `## Screens` after `## Features`,**
  the other six in an HTML table of three rows of two, each `width="256"`, half its 512 px:
  `main-usb | voice-bt`, `no-link-bt | no-link-usb`, `voice-cut-bt | stopped-bt`.
  A Markdown table needs a header row, and these pairs share no field to name it.
- **Each image's alt text names its screen:**
  `The main screen on USB`, `Listening`, `No link on Bluetooth`, `No link on USB`,
  `Voice stopped`, `Reconnect stopped`; `main.png` keeps its own.
- **Relatively simple, so implemented on `develop`:** Markdown, one link and three spec deltas, no code.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `docs: show every screen in the readme and site`.**

## What Changes

- `docs/assets/main.png` gives way to `docs/assets/screenshot/`, a link per screenshot, by the fallback;
  Home and `Using the remote` point at `assets/screenshot/main.png`.
- `Using the remote` shows five more screenshots, `USB` one.
- The README gains `## Screens` with six screenshots.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-build`: the site's links outside `asset/` are one per screenshot.
- `docs-site`: the build copies every screenshot under `.site/assets/screenshot/`.
- `user-guide`: `Using the remote` and `USB` show their screens, and the README shows all seven.

## Impact

`docs/assets/main.png` (removed), `docs/assets/screenshot/` (seven new links), `docs/index.md`,
`docs/guide/using-the-remote.md`, `docs/guide/usb.md` and `README.md`.
`asset/screenshot/`, `.github/workflows/release.yml`, `zensical.toml` and `catalog/` stay as they are.

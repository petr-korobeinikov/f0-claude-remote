# Proposal

## Why

The app builds and does nothing:
`f0_claude_remote.c` is the template's 12 lines, which log twice and return,
and the device lists it as `App f0_claude_remote`.
This is the first of four small app changes,
`app-screen`, `app-keymap`, `ble-transport` and `remote-actions`,
which start from the interface so every step shows a real result on the Flipper.
A dry run in a scratch clone built and linted these sources against SDK 1.5.1,
API 88.2, into a 5.0 KB FAP;
no device was attached, so nothing here has run on hardware yet.

Decisions, settled with the owner on 2026-11-03:

- **Screen variant A, board `Main`,**
  not the list or the large status:
  each label stands where its button is.
- **Mechanics first, no lock,**
  not the wireframe's locked boards:
  whether a lock exists at all stays open, at the bottom of the backlog.
- **A static `Connected`,**
  not a real status:
  Bluetooth arrives with `ble-transport`.
- **Layout checked by eye on the device,**
  not by a test:
  `view` only renders, and TDD starts with `keymap`.

Decisions proposed here, for the owner to strike:

- **The menu name is `Claude Remote`,**
  not `F0 Claude Remote`:
  on the device, the prefix names the device itself.
- **`app.c` replaces `f0_claude_remote.c`, with the entry point `app_main`,**
  not the template's names:
  a file's name prefixes its content, and the project calls this module `app`,
  at the cost of two spec deltas that name the old file.
- **No `app.h`,**
  not the brief's header for every module:
  the loader finds the entry point by name, so the header has no consumer.
- **Sources stay flat at the root,**
  not under `src/`:
  `ide-navigation` already speaks of a `.c` file added at the root.
- **`fap_category` stays `Examples`,**
  not `Bluetooth`:
  the app has no Bluetooth yet, and moving it rewrites a scenario of `app-build`,
  so `ble-transport` decides.
- **The menu icon stays the template's,**
  not a new one:
  the wireframe draws none.
- **The exit rule sits in `app` until `app-keymap`,**
  not in a `keymap` written now:
  that change starts TDD, and its plan moves the rule under a test.

## What Changes

- `application.fam` gets its `name` and `entry_point`.
- New `app.c`, `view.c`, `view.h` and six icons in `images/`.
- `f0_claude_remote.c` and `images/.gitkeep` are removed.

## Capabilities

### New Capabilities

- `app-screen`: shows the remote's one screen and leaves it on request.

### Modified Capabilities

- `device-debug`: its breakpoint scenarios name `app.c`.
- `ide-navigation`: its database and navigation scenarios name `app.c`.

## Impact

No tool or dependency enters `mise.toml`;
the icons are written once with Pillow 10.4.0, which toolchain 39 already carries.
`dist/f0_claude_remote.fap` and its path on the device stay as they are.
The app now stays open, so `mise run launch` meets a running copy for the first time:
`design.md` covers it.

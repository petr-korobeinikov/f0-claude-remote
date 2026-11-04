# Proposal

## Why

`src/` holds 14 files side by side,
while the config names three layers and every design must say which one a file lands in.
The tree shows none of that:
`host-tests` keeps the core free of the SDK through a list of five paths that each new core file has to join by hand,
and `ble-transport` and `effort-actions` are about to add `transport` and `actions`.
The owner's call of 2026-11-04 is to split by layer now, as a rename of its own,
while nothing is in flight and the code diff comes to five include lines.

Decisions, settled by the owner on 2026-11-04:

- **`src/core/`, `src/ui/`, `src/platform/`,**
  not `core` with `flipper`, nor nginx's `os/flipper`:
  the config's own layer words make a design's layer a path, and no second OS is planned.
- **`view` in `src/ui/`,**
  not beside `screen`:
  the config lists it above the core, not among the platform-specific code.
- **No `include/`,**
  not a header tree apart:
  it serves libraries, and a header stays in its layer's folder.
- **An include crosses a layer as `"../core/action.h"`,**
  not `"src/core/action.h"`:
  the firmware's own apps write it so (`hid_app/views/hid_media.c:6`),
  and it rests on no include path, where the app root's place on that path shows only in `fbt_extapps.py:148`, in none of the manifest or build documents.

Decisions proposed here, for the owner to strike:

- **Each file goes where the config's lists put it,**
  not a fresh look at `screen`, whose name reads as UI:
  a rename reopens no layer.
- **`test/` stays flat,**
  not `test/core/`:
  only the core has suites, and test files left untouched are part of the proof.
- **A spec pins the direction of includes,**
  not the folders alone:
  an unchecked folder is a convention, and two `git grep`s cost nothing.
- **The `no SDK` scenario greps `src/core`,**
  not a longer list of files:
  a new core file is covered on arrival.
- **The proof compares objects as well,**
  beyond the config's counts, renames and search,
  and not the package:
  on a trial the link took objects in a new order, so `.text` and `.rodata` shift within the same 9,908 bytes,
  while all 7 objects differ only in `.debug_info`, `.debug_line` and `.debug_str`.
- **One launch and one frame, no press,**
  not a round of every button:
  addresses move and code does not, and nothing here buzzes.

## What Changes

- 14 files move under `src/core/`, `src/ui/` and `src/platform/`.
- Five include lines name the layer they cross into.
- `.mise/tasks/test` finds the suites' sources in `src/core/`.
- `openspec/config.yaml` says where each layer lives and how an include crosses.

`application.fam` stays:
the manifest documentation has `*.c*` take every C source in the app folder, and the build confirms it.
`compile_commands.json` stays as well: it is a tracked symlink, and the build rewrites its target.

## Capabilities

### New Capabilities

- `source-layers`: which folder a source file sits in, and which way an include may point.

### Modified Capabilities

- `host-tests`: `The logic needs no SDK` checks the core's folder rather than a list of paths.
- `ide-navigation`: the compilation database and CLion's check name `src/platform/app.c`.

## Impact

No tool or dependency enters `mise.toml`, and no behavior changes.
`device-debug` says `app.c` by bare name and holds as written.
On this machine, the ignored `.idea/workspace.xml` keeps a breakpoint at `src/app.c:66` that goes stale.
Next comes `ble-transport`, on the new layout.

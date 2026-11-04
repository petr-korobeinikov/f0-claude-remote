# Design

## Context

See `proposal.md` for why.
`AppManifests.md` has `sources=["*.c*", "!test"]` take every C source in the app folder, so `application.fam` stays,
and gives an app no include path of its own: `fap_include_paths` belongs to private libraries under `lib/`.
A quoted include looks beside the including file first, so same-folder lines stay as they are.
`CODING_STYLE.md` says nothing on include spelling, and its directory pattern `^[0-9A-Za-z_]+$` admits the three names.

A trial on a scratch clone, SDK 1.5.1, ran every check in `tasks.md` but the device's and CLion's.

## Goals / Non-Goals

**Goals:** a path that names the layer, an include that shows the direction, the same code in every object.

**Non-Goals:** any behavior; `view.c`'s seam, which `ble-transport`'s third board shows; `transport.h`'s folder, which `ble-transport` decides and the config's header rule points at `src/core/`.

## Decisions

Every file keeps its name, its one task and its check.

| From `src/` | To | Layer | Its one task |
|---|---|---|---|
| `action.h` | `src/core/` | Core | Names the remote's actions |
| `feedback.h`, `.c` | `src/core/` | Core | Says how an action confirms itself |
| `keymap.h`, `.c` | `src/core/` | Core | Maps a button event to an action |
| `view.h`, `.c` | `src/ui/` | UI | Draws the screen |
| `app.h`, `.c` | `src/platform/` | Platform | Runs the application's event loop |
| `keymap_input.h`, `.c` | `src/platform/` | Platform | Translates a firmware event for the keymap |
| `main.c` | `src/platform/` | Platform | Hands the firmware's entry point to `app` |
| `screen.h`, `.c` | `src/platform/` | Platform | Keeps what the display shows |

Nothing new lands on the platform side:
`app` stays there for `FuriEventLoop` and `NotificationApp`, `keymap_input` for `InputEvent`, `main` for the firmware's entry point, `screen` for `ViewPort` and `FuriEventLoopTimer`, as `app-keymap` named.
The suites keep their seam, `test/<name>_test.c` against the core's `<name>.c`.

The include lines that now cross a layer:

```text
src/ui/view.h:5                 "action.h"    ->  "../core/action.h"
src/platform/app.c:6            "feedback.h"  ->  "../core/feedback.h"
src/platform/keymap_input.h:5   "keymap.h"    ->  "../core/keymap.h"
src/platform/screen.c:5         "view.h"      ->  "../ui/view.h"
src/platform/screen.h:5         "action.h"    ->  "../core/action.h"
```

`.mise/tasks/test`, line 13:

```text
-Isrc -Itest -o "$dir/$name" "src/$name.c" "$test"
-Isrc/core -Itest -o "$dir/$name" "src/core/$name.c" "$test"
```

`openspec/config.yaml`, in place of ``Sources live in `src/`, tests in `test/`.``:

```text
Sources live in `src/`, one folder per layer - `src/core/`, `src/ui/` and
`src/platform/` - and tests in `test/`. There is no `include/`: a header
sits in its layer's folder. An include crosses a layer downward only and
relative to its file, as `"../core/action.h"`.
```

- **`-Isrc/core` for the suites,**
  not `"../src/core/keymap.h"` in the tests:
  the test files stay untouched, and with `-Isrc` gone a suite sees the core alone.
- **Sections compared by name, object against object,**
  not by a hash of each `.o`:
  `.debug_*` holds the paths and has to differ, and a by-name read says which sections did.

## Risks / Trade-offs

- A new link order moves addresses and could wake a latent memory bug → the launch and frame in `tasks.md`, then the owner's next presses.
- Seven `.o` files stay orphaned under `.ufbt/build/f0_claude_remote/src/` → the link takes none of them: the trial built over them.
- A small header loses its rename in `git log --follow` → `view.h` holds at 78%, the lowest of the 14, against git's 50%.
- CLion keeps the old paths until it reloads the database → the owner's check in `tasks.md` follows a build.

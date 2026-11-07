---
description: The three layers of the source, each file's one task, and the files a press passes through.
---

# Architecture

## Layers

| Folder | Layer | Includes from |
|---|---|---|
| `src/core/` | The rules, with no Flipper header, tested on the Mac | Its own folder |
| `src/ui/` | Draws the state it is handed | Its own folder and the core |
| `src/platform/` | What exists only in the firmware | Every folder |

An include points down the layers only, relative to its file, as `"../core/action.h"`.
The core names what it needs, as the keys of `transport.h`,
and the platform supplies it, as `transport_bluetooth.c` does.

## Files

| File | Its one task |
|---|---|
| `src/core/action.h` | Names the remote's actions |
| `src/core/mode.h` | Names the remote's modes |
| `src/core/keymap.h`, `src/core/keymap.c` | Maps a button event or a link change to an action, given the mode |
| `src/core/feedback.h`, `src/core/feedback.c` | Says how an action confirms itself |
| `src/core/actions.h`, `src/core/actions.c` | Sends an action's keys |
| `src/core/transport.h` | Names the keys the core presses |
| `src/ui/view.h`, `src/ui/view.c` | Draws a mode's board |
| `src/ui/view_idle.h`, `src/ui/view_idle.c` | Draws the idle board's buttons |
| `src/platform/main.c` | Hands the firmware's entry point to `app` |
| `src/platform/app.h`, `src/platform/app.c` | Runs the application's event loop |
| `src/platform/keymap_input.h`, `src/platform/keymap_input.c` | Translates a firmware event for the keymap |
| `src/platform/screen.h`, `src/platform/screen.c` | Keeps what the display shows |
| `src/platform/transport_bluetooth.h`, `src/platform/transport_bluetooth.c` | Is the Mac's Bluetooth keyboard for the app's life |

## A press's path

1. `screen.c`, on the GUI thread: the view port's input callback puts the `InputEvent` on the input queue.
2. `app.c`, on the event loop's thread: takes the event off the input queue.
3. `keymap_input.c`: translates the firmware's key and type into the keymap's own.
4. `keymap.c`: answers the action, given the mode, and moves the mode.
5. `actions.c`: presses and releases the action's keys through `transport.h`.
6. `transport_bluetooth.c`: puts each key into the HID profile's report.

`app.c` then buzzes as `feedback.c` says, has `screen.c` flash, and hands it the mode.

The link takes the other queue:
`transport_bluetooth.c`'s status callback, on the Bt thread, puts `true` or `false` on the link queue,
and `app.c` hands it to `keymap.c`,
whose answer `actions.c` sends as a press's would be.

The queues exist because every function of an event loop must be called from the thread that made it
(documented: [`event_loop.h`](https://github.com/flipperdevices/flipperzero-firmware/blob/1.5.1/furi/core/event_loop.h#L15)).
The draw callback, on the GUI thread, reads the mode and the flash as two `_Atomic` words.

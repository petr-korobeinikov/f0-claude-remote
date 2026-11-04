# Design

## Context

See `proposal.md` for why.
The boards are `D_voice` and `D_sent_*` of the wireframe,
version `1793780081-d235`;
read off their pixels, each `D_sent_*` differs from `D_Main` in its flash box alone.

Firmware facts come from SDK 1.5.1's headers and the disassembly of its `firmware.elf`.
A tap publishes Press, Short, Release, whatever the comment in `input.h` says.
Long fires 300 ms after Press, then Repeat every 150 ms.
`sequence_single_vibro` is vibro on, 100 ms, off.

## Goals / Non-Goals

**Goals:** every button's action, proved on the host; its buzz, flash and screen on the device.

**Non-Goals:** `transport` and keystrokes; D's disconnected board, which `ble-transport` brings; the lock.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/action.h` | Core | Names the remote's actions | Compiled by both suites | The enum |
| `src/keymap.h`, `.c` | Core | Maps a button event to an action | `test/keymap_test.c` | Its own key and event types |
| `src/feedback.h`, `.c` | Core | Says how an action confirms itself | `test/feedback_test.c` | `Action` in, `bool` out |
| `src/keymap_input.h`, `.c` | Platform | Translates a firmware event for the keymap | Every button, on the device | `InputEvent` |
| `src/screen.h`, `.c` | Platform | Keeps what the display shows | Frames, on the device | `view_draw` |
| `src/view.h`, `.c` | UI | Draws the screen | Frames against the boards | `Canvas*`, all it touches |
| `src/app.h`, `.c` | Platform | Runs the application's event loop | Buttons, on the device | `screen.h`, `keymap_input.h` |
| `src/main.c` | Platform | Hands the firmware's entry point to `app` | `mise run launch` | `app.h` |
| `test/test.h` | Test | Runs one test and reports it | Both suites | `test_run` |
| `.mise/tasks/test` | Tooling | Builds and runs the suites | `mise run test` | `test/<name>_test.c` against `src/<name>.c` |

`keymap_input`, `screen`, `app` and `main` are platform code
because `InputEvent`, `ViewPort`, `FuriEventLoopTimer`, `NotificationApp` and the entry point's contract exist only in the firmware.

```c
Action keymap_handle(Keymap* keymap, KeymapKey key, KeymapEvent event);
bool keymap_is_voice_on(const Keymap* keymap);
bool feedback_vibrates(Action action);
bool feedback_flashes(Action action);
Action keymap_input_handle(Keymap* keymap, const InputEvent* event);
Screen* screen_alloc(FuriEventLoop* event_loop, FuriMessageQueue* input_queue);
void screen_set_voice(Screen* screen, bool voice_on);
void screen_flash(Screen* screen, Action action);
void view_draw(Canvas* canvas, bool voice_on, Action flash);
void app_run(App* app);
```

- **A table of key by event in `keymap`,**
  not a `switch`:
  it reads as the layout, and an empty cell is `ActionNone`, so nothing acts by accident.
- **`keymap` holds voice,**
  not `app`:
  a Release after Long looks like one after Short,
  and that state also decides what the other buttons do.
- **Voice and flash as two words,**
  not one `Action shown`:
  in one word a flash's timer would wipe a voice screen started inside its 400 ms;
  apart, `view_draw` only prefers voice.
- **`_Atomic`,**
  not a `FuriMutex`:
  the GUI thread reads two words with no invariant between them,
  and a trial build imports no `__atomic_*`.
- **`FuriEventLoopTimer`,**
  not `FuriTimer`:
  it fires on the loop's thread, and starting it again restarts it.
- **`screen` and `keymap_input` apart from `app`,**
  not one file:
  a trial `app.c` reached 130 lines, and these two seams leave it 66.
- **`view.c` stays one file, at 115 lines,**
  not one per board:
  its components are its seams, and `ble-transport`'s third board is when a second file pays.
- **The status row loses the legend,**
  which becomes `view_draw_legend`:
  both boards draw the row, so `ble-transport` changes it once.
- **`sources=["*.c*", "!test"]` in `application.fam`,**
  not an explicit list:
  the recursive glob stays, and in a trial build `test/` reaches neither the objects nor the database.

Teardown runs `screen_free` first, so no input arrives once the queue goes;
it frees the timer before `app_free` frees the loop, as `event_loop_timer.h` demands.

The voice board, after a black `box(0, 0, 128, 64)`, all white, baselines as y:

```text
icon   bluetooth            2  1
str    "Connected"         12  9   FontPrimary
line   0,12 to 127,12
icon   microphone_large    22 27
str    "Listening"         48 37   FontPrimary
str    "Release OK to stop" 48 48  FontSecondary
```

The flash, an XOR box drawn last on the idle board:

```text
high    67 15 21 9
max     67 53 22 9
clear    4 34 32 9
Esc     85 34 26 9
```

`microphone_large` is 16×21, a 1-bit PNG like the rest:

```text
......####......
....########....
....########....
...##########...
...##########...
...##########...
...##########...
...##########...
#..##########..#
#..##########..#
#..##########..#
#..##########..#
#...########...#
.#..########..#.
..#...####...#..
...#........#...
....########....
.......##.......
.......##.......
.......##.......
...##########...
```

## Risks / Trade-offs

- A swapped row in `keymap_input` passes the host suites → the device check walks every button.
- A tap past 300 ms turns Enter into voice, the brief's critique 3 → the device check with the owner feeds the next plan.
- A press on Right in a pocket sends Esc → the lock question, at the bottom of the backlog.
- The input queue of 8 drops OK's release when full, and voice stays on → the next OK release stops it.
- The database lists no `test/` file, so CLion indexes none → `mise run test` still reports every error; a host database is a change of its own.

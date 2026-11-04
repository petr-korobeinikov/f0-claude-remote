# Proposal

## Why

The app's buttons still do almost nothing:
`src/app.c` reacts to a held Back alone,
and `view_draw` knows one frame.
This is the second of four app changes and the first with logic,
so TDD starts here:
`keymap` decides what each button does, on the host,
and a vibration, a label flash and the voice screen show the result on the Flipper,
before `transport` exists.
Planning waited for D's remaining states,
which wireframe version `1793780081-d235` now holds:
`D_voice` and four `D_sent_*` boards.

Decisions, already settled:

- **The host tests compile with the system `cc`,**
  not `zig cc`:
  the owner's call, 2026-11-04, at the cost of an unpinned version
  and an exception to the README's "needs mise and nothing else" for `mise run test`.
- **On a still screen a fill means hold, and a flash means sent,**
  not a fill for pressed as in stock remotes:
  the board's answer to the question `app-screen` left open,
  since the legend explains the fill and only one element changes.
- **Mechanics only,**
  not the lock, Bluetooth or keystrokes:
  the lock stays at the bottom of the backlog,
  the status stays a static `Connected`,
  and `ble-transport` and `remote-actions` follow.
- **`main.c` holds the entry point, and `app` gets a header,**
  not `app_main` inside `app.c`:
  the owner's call, 2026-11-04, which gives `app.h` the consumer `app-screen` found none for.

Decisions proposed here, for the owner to strike:

- **The entry point is `main_entry`,**
  not `app_main`:
  a file's name prefixes its content, and `-Wmain` refuses a `main` taking `void*`.
- **Esc fires at press,**
  not when a tap ends:
  past the 300 ms long press a slow Esc would otherwise do nothing,
  just when interrupting Claude matters.
- **While voice is on, every other button does nothing, exit included,**
  not exit on a held Back:
  a slipping thumb must not end a dictation.
- **One buzz on voice start, none on its stop or on exit,**
  not a buzz for all of them:
  the thumb that lets go, or the menu that returns, says so already.
- **The firmware's 300 ms for every hold,**
  not a longer one for `clear`:
  the device check decides whether `clear` fires too easily,
  and a longer hold can count repeats with no clock, in a change of its own.
- **No test framework, `assert` and one macro printing `ok <name>`,**
  not Unity or minunit:
  the SDK ships neither, and vendoring one means an audit for 16 tests of pure functions.
- **A `feedback` module in the core,**
  not the choice inside `app` or `view`:
  that arrows and Enter do not flash is a rule, and rules stay out of the UI.
- **A `Test` run configuration,**
  not the terminal alone:
  `Build`, `Launch` and `Debug` already live on CLion's toolbar.
- **The workflow stays build-only,**
  not a third job:
  `app-build` counts two jobs, so running the tests there is a change of its own.

## What Changes

- New in `src/`: `action.h`, `keymap`, `feedback`, `keymap_input`, `screen`, `main.c` and `app.h`.
- `src/app.c` keeps the event loop,
  and hands the entry point to `main.c` and the view port to `screen`.
- `src/view.c` gains the voice board and the flash, and `images/` the microphone.
- New `test/`, `.mise/tasks/test` and `.run/Test.run.xml`;
  `application.fam` names the new entry point and keeps `test/` out of the package.
- `README.md` and `openspec/config.yaml` state the test setup.

## Capabilities

### New Capabilities

- `button-actions`: what each tap and hold does, and its buzz.
- `host-tests`: proves the logic on the Mac, with no device and no SDK.

### Modified Capabilities

- `app-screen`: the voice screen and the flash arrive,
  the ring becomes the idle state, and the other buttons no longer do nothing.
- `ide-run-configurations`: a `Test` configuration joins the list.

## Impact

No tool or dependency enters `mise.toml`.
A trial build takes the package from 5.0 KB to 8.8 KB;
`dist/f0_claude_remote.fap` and its path on the device stay as they are.

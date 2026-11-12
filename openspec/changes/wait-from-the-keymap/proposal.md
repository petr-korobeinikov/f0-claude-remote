# Proposal

## Why

When the 60 s with no link start, and start afresh, is decided in `src/platform/app.c`, which no suite builds (`.mise/tasks/test:13`):
`App` keeps the wait it last followed (`app.c:20`),
`app_follow_wait` starts the timer only when that turns from 0 (`:62-70`),
and `app_perform` zeroes it on `ActionUseUsb` and `ActionUseBluetooth`, so the next follow counts afresh (`:35-38`).
It is a rule, not a firmware fact, and three scenarios stand on it:
`bluetooth-link`'s `A press does not extend it` and `Counted afresh`, and `usb-link`'s `Counted afresh`.
The keymap's suite pins the wait's length and its end alone (`test/keymap_test.c:360-368`, `:574-580`),
so a timer restarted on every press would pass all 82 tests.
Beside it, `keymap_wait_ms` answers by `==` on two modes (`src/core/keymap.c:156-158`):
a sixth mode would wait for nothing, unasked.
The review of 2026-11-10 found the first,
and `mode-without-tables` left the second to this plan (`openspec/changes/archive/2026-11-12-mode-without-tables/design.md:23-24`).
It comes now, as `mode-without-tables`, whose `keymap.c` it edits, landed in `4bd210b`.

Decisions:

- **The keymap answers each event with what the wait does - start, stop or keep - and `app.c` starts and stops its timer by that answer alone,**
  not a number for each wait, which `app.c` would compare with the one it last followed:
  that comparison is the rule the review found, left where no suite builds it.
- **`keymap_wait_ms` asks every mode in a `switch`,**
  not two by `==`:
  the build then stops at a mode nobody asked, as it does at a link report with no answer (`openspec/specs/app-build/spec.md:116-127`).
- **The requirement goes to `host-tests`,**
  not to a capability of its own, nor `skip_specs`:
  `bluetooth-link` and `usb-link` already say what the 60 s count from,
  and what changes is where that is decided and proved.
- **One device check, by frames, with the Mac's Bluetooth off for a minute and a half,**
  not none:
  no suite builds `app.c`'s calls of the timer.
  Both `Counted afresh` then rest on the host test and on the start's one case, which the check sees at a drop.
- **Complex, so implemented on `feature/wait-from-the-keymap`:**
  the rule crosses the layer boundary - the core gains an answer, `app.c`'s timer is rewired with no host test, and one check takes the device.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `refactor(keymap): hold the wait's restart rule`.**

## What Changes

- `keymap` answers each event with what the wait does, read through `keymap_wait`,
  and its suite gains eleven tests, one for each kind of event.
- `keymap_wait_ms` asks every mode how long it waits, in a `switch`.
- `app.c` starts and stops the wait's timer by the keymap's answer;
  `App.wait_ms` and `app_perform`'s reset go.
- `docs/internals/architecture.md` and `docs/internals/usb.md` say who starts the timer.
- Every board, key, buzz and second stays as it is.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `host-tests`: the core decides when the wait is counted, the keymap's suite pins it,
  the platform keeps no count, and every mode is asked how long it waits.

## Impact

`src/core/keymap.h`, `src/core/keymap.c`, `test/keymap_test.c`, `src/platform/app.c`,
`docs/internals/architecture.md` and `docs/internals/usb.md`.
`ActionUseUsb` and `ActionUseBluetooth` stay: the transport still switches by them (`src/core/actions.c:51-56`).
The other sources, `README.md` and the rest of `docs/` stay as they are.

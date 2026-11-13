# Proposal

## Why

When the display is lit is decided in `src/platform/`, which no suite builds (`.mise/tasks/test:13`):
`app_perform` darkens it on `ActionGiveUp` and lights it on `ActionStartOver` (`src/platform/app.c:46-48`),
and `backlight_alloc` lights it at the opening, unasked (`src/platform/backlight.c:15-16`).
It is a rule, not a firmware fact, and four scenarios stand on it:
`app-screen`'s `Held on`, `Off on stopping` and `Leaving after starting over` (`openspec/specs/app-screen/spec.md:192-211`),
and `usb-link`'s `A minute with no link` (`openspec/specs/usb-link/spec.md:60-62`).
No test names the backlight (`git grep -n -i backlight -- test` prints nothing),
so a display left lit once the offer stops, or dark from the opening, would pass all 93 tests.
`wait-from-the-keymap` took the wait's rule out of `app.c` and left this one as a finding of its own
(`openspec/changes/archive/2026-11-13-wait-from-the-keymap/design.md:21`).
It comes now, as that plan, whose `app.c` it edits, landed in `45611df`.

Decisions:

- **The core says of each mode whether it keeps the display lit, and the platform sets the backlight by that after every event,**
  not an answer for each action, as `feedback` gives:
  the opening is lit with no action,
  and thirteen of the fifteen actions would need a third answer, that the light stays as it is.
- **The rule is a unit of its own, `display`, beside `feedback` and `deed`,**
  not a fifth answer of the keymap, as the wait is:
  the wait's answer depends on the event, the light on the mode alone,
  and `keymap`'s one task is mapping events to actions (`docs/internals/architecture.md:26`).
- **`backlight_alloc` takes the opening's answer, as `screen_alloc` takes the mode and the link (`app.c:121-122`),**
  not lighting the display by itself:
  the opening would stay lit by a rule the core is never asked.
- **The backlight is set where `app_perform` set it, after the action,**
  not beside the board, before it:
  every call into the firmware keeps its order.
- **The requirement goes to `host-tests`, beside the wait's,**
  not to `app-screen`, whose `The backlight follows the offer` stays word for word:
  what changes is where the rule is decided and proved.
- **The probe of `host-tests`' `Every mode is asked how long it waits` goes into `display.c`'s switch too,**
  not left as written:
  `mise run test` stops at the first suite that fails to build (`.mise/tasks/test:3`, `:9-15`),
  and `display` builds before `keymap`, so an enumerator in no switch would be named at `display.c` alone.
- **One device check, by frames and the owner's eye, two clicks and three glances,**
  not none:
  no suite builds `app.c`'s calls of the backlight, and no frame shows the light.
- **Complex, so implemented on `feature/backlight-from-the-mode`:**
  the rule crosses the layer boundary - the core gains a unit,
  `app.c` and `backlight.c` are rewired with no host test, and one check takes the device.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `refactor(display): hold the backlight's rule`.**

## What Changes

- A new core unit, `display`, says of each mode whether it keeps the display lit,
  and a suite of its own pins the answer for all five.
- `app.c` sets the backlight by that answer after every event and hands it to `backlight_alloc` at the opening;
  `app_perform`'s `if` on the two actions goes.
- `backlight_alloc` lights the display only when asked to.
- `docs/internals/architecture.md` names the unit.
- Every board, key, buzz and second stays as it is, and the display is lit and dark when it was.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `host-tests`: the core decides when the display is lit, and a suite pins it;
  `Every mode is asked how long it waits` probes past `display.c`.

## Impact

`src/core/display.h`, `src/core/display.c` and `test/display_test.c`, new;
`src/platform/app.c`, `src/platform/backlight.h`, `src/platform/backlight.c` and `docs/internals/architecture.md`.
`ActionGiveUp` and `ActionStartOver` stay: the transport still withdraws and offers the link by them (`src/core/actions.c:45-50`).
The keymap, the other sources, `README.md` and the rest of `docs/` stay as they are.

# Design

## Context

See `proposal.md`, Why.
The rule moves from platform-specific code into the core, as `src/core/display.c`.
`backlight.c` keeps the backlight's lock alone, which exists only in the firmware's notification service.

| Fact | Where |
|---|---|
| The mode turns to `ModeStopped` only with `ActionGiveUp`, and leaves it only with `ActionStartOver` | `src/core/keymap.c:138-146`, `:181-189` |
| The keymap's suite pins both moves | `test/keymap_test.c:448-460`, `:493-502` |
| `backlight_set` sends nothing when asked for what it holds | `src/platform/backlight.c:31-33` |
| Every event ends in `app_settle` | `src/platform/app.c:69-97` |
| A suite `test/<name>_test.c` is built with `src/core/<name>.c` | `.mise/tasks/test:9-14` |
| The package takes every `.c` outside `test/` | `application.fam:15` |

By the first row, lit in every mode but `ModeStopped` is the display of today,
which opens lit, goes dark on `ActionGiveUp` and lights on `ActionStartOver`.

## Goals / Non-Goals

**Goals:** the spec's, with the display lit and dark when it is today, and every call into the firmware in its order.

**Non-Goals:** `ActionGiveUp` and `ActionStartOver`, by which the transport still withdraws and offers the link;
`backlight_set` and `backlight_free`, which stay line for line.

## Decisions

| File | Layer | Its one task | What changes |
|---|---|---|---|
| `src/core/display.h`, `.c` | Core | Says whether a mode keeps the display lit | New |
| `test/display_test.c` | Test | Pins what `display` answers | New, two tests |
| `src/platform/backlight.h`, `.c` | Platform | Holds the display lit while asked to | `backlight_alloc` takes `lit` |
| `src/platform/app.c` | Platform | Runs the application's event loop | Sets the backlight by `display_lit`; the `if` on two actions goes |

`display.h` includes `<stdbool.h>` and `"mode.h"`, and declares the one function `display.c` holds:

```c
bool display_lit(Mode mode) {
    switch(mode) {
    case ModeIdle:
    case ModeVoice:
    case ModeNoLink:
    case ModeVoiceCut:
        return true;
    case ModeStopped:
        break;
    }
    return false;
}
```

`backlight_alloc` opens unlit, the lock not taken, and is asked as `backlight_set` is:

```c
Backlight* backlight_alloc(bool lit) {
    Backlight* backlight = malloc(sizeof(Backlight));
    backlight->notification = furi_record_open(RECORD_NOTIFICATION);
    backlight->lit = false;
    backlight_set(backlight, lit);
    return backlight;
}
```

In `app.c`, `app_alloc` moves `keymap_alloc` above the backlight's line, which becomes
`app->backlight = backlight_alloc(display_lit(keymap_mode(app->keymap)));`.
`app_perform` loses its `if` on `ActionGiveUp` and `ActionStartOver` (`:46-48`),
and `app_settle` gains one line, between `app_perform`'s call and `app_follow_wait`'s:

```c
    backlight_set(app->backlight, display_lit(keymap_mode(app->keymap)));
```

`app.c` includes `"../core/display.h"`.
`app_free` stays as it is: `backlight_free` reads no keymap.

The tests:

| Test | Modes asked | Answer |
|---|---|---|
| `test_lit_while_the_link_is_held_or_offered` | `ModeIdle`, `ModeVoice`, `ModeNoLink`, `ModeVoiceCut` | Lit, each |
| `test_dark_once_the_offer_stops` | `ModeStopped` | Dark |

`docs/internals/architecture.md` gains a row after `deed`'s (`:28`),
"| `src/core/display.h`, `src/core/display.c` | Says whether a mode keeps the display lit |",
and its `:65` becomes "and `app.c` has `backlight.c` go dark, as `display.c` says of the new mode."

- **`backlight_alloc` asks `backlight_set`,**
  not an `if(lit)` of its own:
  one line of `backlight.c` then takes the lock (`:36`), at the opening as on a start over.
- **Two tests that name every mode,**
  not the rule read through the keymap's events:
  the keymap's suite already pins when the mode turns to `ModeStopped` and back,
  and `display` is handed a mode alone.
- **The lit modes return in their cases and `false` follows the switch, as `feedback_vibrates` is written (`src/core/feedback.c:3-24`),**
  not the reverse:
  the two files read alike.

Test seams:

- `display.c`: its own suite, through `display.h`.
  Red is the missing `display.h`, then, with `ModeStopped` among the lit cases, the assert of `test_dark_once_the_offer_stops`.
- `backlight.c` and `app.c` have no host test:
  `mise run build` stops at a `backlight_alloc` called with no answer,
  the spec's two `git grep` find an action named for the light and an opening that asks nobody,
  and tasks 4.1 and 4.2 see the display lit at the opening, dark at the stop and lit after the press.

## Risks / Trade-offs

- `backlight_set` is called after every event, not after two actions → it returns at once unless the answer changed (`backlight.c:31-33`), so the firmware gets the messages it gets today.
- A mode added later is lit or dark by whoever adds it → the build stops at `display.c`'s switch until the mode is named there.
- The light after the press is told from the press's own only past the device's Backlight Time → the check looks at 40 s, the time being 30 s, its default (`openspec/changes/archive/2026-11-08-reconnect-timeout/tasks.md:7`).
- `Leaving after starting over` is not seen on the device again → `backlight_free` and the lock's balance stay line for line (`backlight.c:20-26`).

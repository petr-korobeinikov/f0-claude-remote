# Proposal

## Why

Five `switch` statements over `Action` end in `default:`,
which turns off the check `-Wall` gives both builds:
`-Wswitch` warns at an enumerator a switch leaves out,
and, in GCC's manual, "The presence of a default label prevents this warning."
So an action added to `src/core/action.h` and the keymap, and to no switch,
builds with no warning and takes every default unasked:
it buzzes (`src/core/feedback.c:13`), the confirmation `button-actions` gives an action,
yet sends no key (`src/core/actions.c:57`);
it gets no flash (`feedback.c:30`) and no box to flash (`src/ui/view_idle.c:112`), and earns no XP (`src/core/deed.c:9`).
`6be8b40` and `3e283c5` each added two actions that send no key,
and each put them among `feedback_vibrates`' silent cases by hand.
The review of 2026-11-10 found three of the five labels; the tree holds the other two.
The two switches over `Mode`, in `keymap_handle` and `view_draw`, already name every mode and carry no `default:`.
It is the pass's next finding, past what ships or misleads.

Decisions:

- **All five labels go, not the review's three:**
  `feedback.c:30` and `view_idle.c:112` hide the same decision, whether a new action flashes and how.
- **A case that does nothing ends in `break`, and one `return` after the switch gives the nothing value, `false`, `0` or `DeedNone`,**
  not a `return` in every case and none after:
  an `Action` holding a value no case names would run off the function's end,
  and using that result is undefined, as GCC's manual says under `-Wreturn-type`.
- **`feedback_vibrates` names the eight actions that buzz and returns `false` after the switch,**
  not its seven silent ones with `true` after:
  a value no case names then buzzes no more than it flashes or earns XP.
- **`default:` goes, not `-Wswitch-enum` added,**
  which warns even with a `default:`:
  it asks for the same cases, and for a flag in both builds.
- **The rule goes to `openspec/config.yaml`, and the stop it buys to `app-build`,**
  as `asset-folder` split its folder rule from `A missing picture stops the build`:
  the rule binds future switches, and the stop is how the build behaves.

## What Changes

- `feedback_vibrates` and `feedback_flash_ms` (`src/core/feedback.c`), `actions_perform` (`src/core/actions.c`),
  `deed_earned` (`src/core/deed.c`) and `view_idle_draw_flash` (`src/ui/view_idle.c`)
  name every action and carry no `default:`.
  Every action's buzz, flash, keys and XP stay as they are.
- `openspec/config.yaml`'s Engineering section gains the rule.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-build`: a `switch` over an enum names every enumerator, with no `default:`, so the build stops at one a switch leaves out.

## Impact

`src/core/feedback.c`, `src/core/actions.c`, `src/core/deed.c`, `src/ui/view_idle.c` and `openspec/config.yaml`.
The headers, the tests, `README.md` and `docs/` stay as they are.

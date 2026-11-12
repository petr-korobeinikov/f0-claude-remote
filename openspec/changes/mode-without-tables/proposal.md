# Proposal

## Why

A link report moves the mode through two tables with no length,
`keymap_link_up_modes[]` and `keymap_link_down_modes[]` (`src/core/keymap.c:28`, `:36`):
each is as long as its last row, and `Mode` (`src/core/mode.h`) has no `…MAX` to size it by.
A sixth mode builds once `keymap_handle` and `view_draw` name it (`keymap.c:101`, `src/ui/view.c:71`),
and nothing asks for its rows:
a report in a mode added last reads past the table's end (`keymap.c:141-142`).
The review of 2026-11-10 found it, and proposed `ModeMAX` with a `_Static_assert` on the length.
Since then `switch-without-default` (`63c8fa3`) has made it the rule
that the build stops at an enumerator a `switch` leaves out.
The tree splits its enums two ways:
its seven switches are over `Action` and `Mode`, which have no `…MAX`,
and its eight sized tables are indexed by five enums of its own and the SDK's two, each closed by a `…MAX`.
`Mode` alone is used both ways.
It is the pass's next finding.

Decisions:

- **A link report is answered in a `switch` over the mode, as a button event is (`keymap.c:101`),**
  not from the tables sized by a `ModeMAX`:
  a sized table still builds with a row left out, which reads as zero, `ModeIdle`, the board that says `Connected`,
  and `ModeMAX` would be a sixth enumerator for both switches over `Mode` to name.
- **The rule goes to `openspec/config.yaml`, and the stop it buys to `app-build`,**
  as `switch-without-default` split its own:
  an enum is switched over or indexes tables, never both, which binds the next enum as it does `Mode`.

## What Changes

- `keymap_set_connected` answers a link report in a `switch` over the mode,
  and `keymap_link_up_modes` and `keymap_link_down_modes` go.
  Every mode answers a report as before.
- `openspec/config.yaml`'s Engineering section gains the rule.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-build`: an enum under a `switch` indexes no table, so the build stops at a mode with no answer to a link report,
  and a table takes its length from its enum's `…MAX`.

## Impact

`src/core/keymap.c` and `openspec/config.yaml`.
The headers, the tests, the UI, the platform, `README.md` and `docs/` stay as they are:
`docs/internals/architecture.md:26` gives `keymap.c`'s task, not its tables.

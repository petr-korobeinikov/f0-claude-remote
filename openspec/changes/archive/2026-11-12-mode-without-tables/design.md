# Design

## Context

See `proposal.md`, Why.
The change lands in the core, in `src/core/keymap.c`, where a link report is already answered:
no rule moves between layers, and no header changes.

| Fact | Where |
|---|---|
| An array with no length is as long as "the highest value specified plus one", and `int a[6] = { [4] = 29, [2] = 15 };` "is equivalent to" `{ 0, 0, 15, 0, 29, 0 }` | GCC 12.3.1 manual, Designated Initializers |
| `keymap_actions` names 9 of its 30 cells, and both builds pass | `src/core/keymap.c:17-24` |
| Zero is `ModeIdle`, whose board reads `Connected` | `src/core/mode.h:4`, `src/ui/view.c:10`, `:72-73` |
| `-Wswitch` warns at a switch that "lacks a `case` for one or more of the named codes", and "is enabled by `-Wall`" | GCC 12.3.1 manual, Warning Options |
| Both builds run `-Wall -Wextra -Werror` | `.mise/tasks/test:11`; `cc_args` in the SDK's `sdk.opts`, `1.5.1` |
| `src/` holds seven switches and eight sized tables | `git grep -n 'switch(' -- src`; `git grep -n 'MAX\] = {' -- src` |
| Eleven tests pin the mode and the answer of all ten pairs of a mode and a report | `test/keymap_test.c:150-200`, `:420-446`, `:518-525` |

## Goals / Non-Goals

**Goals:** the spec's, every mode answering a report as before.

**Non-Goals:** `keymap_wait_ms` (`keymap.c:155`), which answers by two comparisons, not a table:
the wait's restart rule in `app.c` is a finding of its own, and its plan weighs this one;
the eight tables that have a length.

## Decisions

| File | Its one task | What changes |
|---|---|---|
| `src/core/keymap.c` | Maps a button event, a link change or the wait's end to an action, given the mode | Both tables go; `keymap_set_connected` |
| `openspec/config.yaml` | States the project's rules | A paragraph under Engineering |

Both tables go, and their comment (`keymap.c:26-27`) moves above `keymap_set_connected`, word for word:

```c
/* ModeStopped stays: stopping waits for the radio to go idle (furi_hal_bt.c:261-267), and stopping
 * reports a link down itself (bt.c:312), so a report read there is stale. */
Action keymap_set_connected(Keymap* keymap, bool connected) {
    switch(keymap->mode) {
    case ModeIdle:
        if(!connected) {
            keymap_set_mode(keymap, ModeNoLink);
        }
        break;
    case ModeVoice:
        if(!connected) {
            keymap_set_mode(keymap, ModeVoiceCut);
            return ActionVoiceStop;
        }
        break;
    case ModeNoLink:
    case ModeVoiceCut:
        if(connected) {
            keymap_set_mode(keymap, ModeIdle);
        }
        break;
    case ModeStopped:
        break;
    }
    return ActionNone;
}
```

- **One `switch` in `keymap_set_connected`,**
  not two functions from a mode to a mode, called where the tables were indexed:
  six of their ten cases would name a mode to say it stays,
  and the cut would still be read off the mode before the report and the one after (`keymap.c:143`),
  where `ModeVoice`'s case now answers `ActionVoiceStop` itself.

The rule's text, under Engineering, after the paragraph ending "follows the switch.":

```yaml
  A table indexed by an enum takes its length from the enum's `…MAX`,
  never from its rows. An enum a `switch` is over has no `…MAX`, which
  every such switch would have to name, and indexes no table: a row left
  out reads as zero with no warning, where a case left out stops the
  build.
```

Test seams:

- `keymap.c`: the host suite, through `keymap.h`.
  The eleven tests already pin every answer, so none is added:
  the suite stays green through the rewrite, its count of `ok` lines unchanged.
- The rule: `ModeProbe`, an enumerator put in `mode.h` for one build and taken out.
  Before the rewrite the host build names it at `keymap.c:101` alone,
  after it at both switches of `keymap.c`.

## Risks / Trade-offs

- A case answers otherwise than its row did → the eleven tests pin all ten pairs.
- `mise run build` may stop before it compiles `keymap.c` → `mise run test` compiles it alone and names both switches.
- Each new mode takes a case in three switches → the build names the switch it misses.

# Proposal

## Why

`test/deed_test.c` checks 13 of `Action`'s 15 values:
`test_the_rest_earn_nothing` (lines 12-24) leaves out `ActionGiveUp` and `ActionStartOver`.
`6be8b40` added both, and named them in `test/feedback_test.c` and `test/actions_test.c` only;
`3e283c5` named the next two, `ActionUseUsb` and `ActionUseBluetooth`, in all three suites.
`dolphin-deeds` has the application earn the dolphin "no XP of its own for any other action" (`openspec/specs/dolphin-deeds/spec.md:32`),
and `src/platform/app.c` hands `deed_earned` every action but `ActionNone` (lines 42, 75-76),
these two among them: the minute with no link running out (`src/core/keymap.c:126`) and the press that starts over (`:109`).
`deed_earned` answers `DeedNone` for both, through its `default:` (`src/core/deed.c:9-10`),
yet it could give either a deed and every suite would still pass.
The review of 2026-11-10 found it.
`switch-without-default` will name all fifteen in `deed_earned` by hand,
and its design leaves these two to be read against its table, as no suite pins them
(`openspec/changes/switch-without-default/design.md:100`).

Decisions:

- **Two asserts in `test_the_rest_earn_nothing`, not a loop over every action:**
  `Action` has no `ActionMAX` to bound one,
  and an `ActionMAX` would take a case in each of the five switches over `Action` once `switch-without-default` lands;
  `test/feedback_test.c` names every action by hand as well.
- **No spec delta: `.openspec.yaml` sets `skip_specs: true`,**
  not a delta to `dolphin-deeds`:
  its `Nothing else earns XP` already covers both as "any other action", and the app behaves as before.

## What Changes

- `test_the_rest_earn_nothing` asserts `DeedNone` for `ActionGiveUp` and `ActionStartOver` as well,
  so `test/deed_test.c` checks all 15 actions.
- The code the app runs, and `mise run test`'s `ok` lines, stay as they are.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

None: the change sets `skip_specs: true`.

## Impact

`test/deed_test.c` alone.
`src/`, `README.md` and `docs/` stay as they are: no doc names a suite's cases.

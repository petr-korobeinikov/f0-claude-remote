# Design

## Context

See `proposal.md`, Why.
The change lands in the core's host tests, `test/deed_test.c`, and in no code the app runs.

| Fact | Where |
|---|---|
| `test_the_rest_earn_nothing` names the rest in `action.h`'s order, `ActionExit` then `ActionUseUsb` | `test/deed_test.c:21-22`, `src/core/action.h:14-17` |
| `test_run` prints one `ok` line per test, whatever its asserts | `test/test.h:7-12` |
| A failed assertion stops `mise run test` non-zero, naming its file and line | `openspec/specs/host-tests/spec.md:19-22` |

## Goals / Non-Goals

**Goals:** the proposal's, with every answer `deed_earned` gives as before.

**Non-Goals:** `src/core/deed.c`, which `switch-without-default` rewrites: either change can land first.

## Decisions

| File | Its one task | What changes |
|---|---|---|
| `test/deed_test.c` | Pins which deed each action earns | Two asserts in `test_the_rest_earn_nothing` |

The two go where `action.h` puts them:

```c
    assert(deed_earned(ActionExit) == DeedNone);
    assert(deed_earned(ActionGiveUp) == DeedNone);
    assert(deed_earned(ActionStartOver) == DeedNone);
    assert(deed_earned(ActionUseUsb) == DeedNone);
```

- **The red step is a mutation, not skipped:**
  the asserts pass from their first run,
  and canonical TDD asks to watch a test fail for the right reason,
  so `deed_earned` gives `DeedClear` for each of the two in turn, then is undone.

Test seam: `deed_test.c` calls `deed_earned` through `deed.h` on the host, as now.

## Risks / Trade-offs

- The mutation stays in `src/core/deed.c` → task 1.1 ends on `git diff --exit-code -- src`.

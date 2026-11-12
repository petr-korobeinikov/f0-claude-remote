# Design

## Context

See `proposal.md`, Why.
The change lands in the core, in `feedback.c`, `actions.c` and `deed.c` under `src/core/`,
and in the UI, in `src/ui/view_idle.c`;
each switch keeps every answer it gives, so no rule moves between layers, and no header changes.

| Fact | Where |
|---|---|
| `-Wswitch` "is enabled by -Wall"; "The presence of a default label prevents this warning." | GCC 12.3 manual, Warning Options |
| Clang's `-Wall` controls `-Wswitch` | Clang's Diagnostics Reference, `-Wall` |
| The host suites build with `-Wall -Wextra -Werror` | `.mise/tasks/test:11` |
| The SDK builds the app with `-Wall -Wextra -Werror` on GCC 12.3.1 | `cc_args` in the SDK's `sdk.opts`, `1.5.1`; the toolchain's `lib/gcc/arm-none-eabi/12.3.1` |
| Using the result of a non-void function that flows off its end is undefined | GCC 12.3 manual, `-Wreturn-type` |
| `src/` holds seven `switch` statements: five over `Action`, each ending in `default:`, and two over `Mode` with none | `git grep -n 'switch(' -- src` |
| `test/feedback_test.c` pins both functions for all 15 actions, `test/actions_test.c` performs all 15, `test/deed_test.c` checks 13 | `test/` |

## Goals / Non-Goals

**Goals:** the spec's, every action answering as before.

**Non-Goals:** `test/deed_test.c` leaving out `ActionGiveUp` and `ActionStartOver`, a finding of its own;
the switches over `Mode`, which already comply.

## Decisions

| File | Its one task | What changes |
|---|---|---|
| `src/core/feedback.c` | Says how an action confirms itself | `feedback_vibrates`, `feedback_flash_ms` |
| `src/core/actions.c` | Turns an action into calls on the transport | `actions_perform` |
| `src/core/deed.c` | Says which deed an action earns | `deed_earned` |
| `src/ui/view_idle.c` | Draws the idle board's buttons | `view_idle_draw_flash` |
| `openspec/config.yaml` | States the project's rules | A paragraph under Engineering |

Each `default:` gives way to the actions it answered, named in `action.h`'s order.
The cases that do nothing close the switch and end in `break`;
every case already written keeps its place.

| Switch | Cases that act | Cases that do nothing | After the switch |
|---|---|---|---|
| `feedback_vibrates` | The eight that buzz, now named, `return true` | The seven silent ones, as now | `return false;` |
| `feedback_flash_ms` | The seven that flash, `100` or `400`, as now | The other eight | `return 0;` |
| `actions_perform` | Thirteen, as now | `ActionNone`, `ActionExit` | Nothing |
| `deed_earned` | `ActionVoiceStart`, `ActionClear`, as now | The other thirteen | `return DeedNone;` |
| `view_idle_draw_flash` | The seven boxes, as now | The eight `feedback_flash_ms` gives `0` | Nothing |

`feedback_vibrates`, the one whose named group changes, then reads:

```c
bool feedback_vibrates(Action action) {
    switch(action) {
    case ActionEnter:
    case ActionArrowUp:
    case ActionArrowDown:
    case ActionEsc:
    case ActionEffortHigh:
    case ActionEffortMax:
    case ActionClear:
    case ActionVoiceStart:
        return true;
    case ActionNone:
    case ActionVoiceStop:
    case ActionExit:
    case ActionGiveUp:
    case ActionStartOver:
    case ActionUseUsb:
    case ActionUseBluetooth:
        break;
    }
    return false;
}
```

- **Cases grouped by answer, the silent ones last,**
  not every case in `action.h`'s order:
  labels share an answer only when stacked.

The rule's text, under Engineering, after the paragraph ending "holds what an editor can enforce.":

```yaml
  A `switch` over an enum names every enumerator and has no `default:`:
  both builds run `-Wall -Werror`, so `-Wswitch` stops them at an
  enumerator a switch leaves out. What a function returns for a value no
  case names follows the switch.
```

Test seams:

- `feedback.c`, `actions.c` and `deed.c`: the host suites, through their headers, `actions.c` against `test/transport_fake.c`;
  every answer they pin stands.
- `view_idle.c` only draws the flash it is handed and has no host test:
  `mise run build` compiles it, and its cases are read against the table.
- The rule: `ActionProbe`, an enumerator put in `action.h` for one build and taken out,
  meets `A new action` only once the labels are gone.

## Risks / Trade-offs

- A case lands in the wrong group → the suites pin every answer but `deed_earned`'s two silent ones and the boxes, which are read against the table.
- The probe may stop the build at the first switch it fails → `No default` vouches for the rest.
- Each new action takes a case in five switches → the build names the switch it misses.

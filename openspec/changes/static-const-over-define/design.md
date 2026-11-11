# Design

## Context

See `proposal.md`, Why.
The change lands in platform-specific code, `src/platform/transport_bluetooth.c`, where the wait already is:
it gives the firmware's second core its time after each `bt_disconnect` (lines 41-42 and 55-56);
and in `openspec/config.yaml`.

| Fact | Where |
|---|---|
| `furi_delay_ms` takes a `uint32_t` | `furi/core/kernel.h:114`, SDK 1.5.1 |
| `nvm` is the SDK's own word | `furi_hal_bt_nvm_sram_sem_acquire`, `furi_hal_bt.h:162` |
| The SDK compiles the app at `-Os` | `cc_args` in the SDK's `sdk.opts` |
| No object, `const` or not, is an integer constant expression; an array sized by anything else is a variable length array, which no struct member may be | N1570, the C11 draft: 6.6:6, 6.7.6.2, 6.7.2.1:9 |
| `TRANSPORT_FAKE_EVENTS` sizes `events` in `struct Transport` | `test/transport_fake.c:6-9` |
| `src/` and `test/` define three macros: this one, `TRANSPORT_FAKE_EVENTS`, and `test_run`, which takes an argument | `git grep -n '#define' -- src test` |

## Goals / Non-Goals

**Goals:** the proposal's, with the code the device runs as before.

**Non-Goals:** the two macros that stay, `TRANSPORT_FAKE_EVENTS` and `test_run`.

## Decisions

| File | Its one task | What changes |
|---|---|---|
| `src/platform/transport_bluetooth.c` | Is the Mac's Bluetooth keyboard for its own life | Line 8, and the name at lines 42 and 56 |
| `openspec/config.yaml` | States the project's rules | A paragraph under Engineering |

Line 8 becomes:

```c
static const uint32_t transport_bluetooth_nvm_ms = 200;
```

`uint32_t` is the type `furi_delay_ms` takes, as `keymap_link_wait_ms` has it;
the name keeps the macro's words, `nvm` being the SDK's own.

The rule's text, under Engineering, before the paragraph opening "One file, one screen of code":

```yaml
  A named constant is a `static const` of its type, since a `#define` is
  text with no type of its own. A `#define` names a constant only where C
  requires an integer constant expression, which a `const` object is not,
  as an array's length.
```

- **Before "One file, one screen of code",**
  not after the paragraph ending "holds what an editor can enforce.",
  where `switch-without-default` puts its rule:
  either change can land first, and the two rules end up side by side, the switch's first.

Test seams:

- `transport_bluetooth.c` has no host test, being platform code:
  `mise run build` compiles it under `-Werror`, and this lists its code:

  ```sh
  .ufbt/toolchain/arm64-darwin/bin/arm-none-eabi-objdump -d \
      .ufbt/build/f0_claude_remote/src/platform/transport_bluetooth.o
  ```

  Assumed, and checked by task 1.1: `-Os` folds a `static const` the code only reads, so the listing stays the same.
- The rule: `openspec instructions` hands agents `config.yaml`'s context, and its `--json` output shows the paragraph.

## Risks / Trade-offs

- The listing differs → task 1.1 stops and shows both listings to the owner.

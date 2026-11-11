# Proposal

## Why

`src/platform/transport_bluetooth.c:8` names the 200 ms it gives the firmware's second core to write its storage
as `#define TRANSPORT_BLUETOOTH_NVM_MS 200`,
while `src/core/keymap.c:7` names the minute the link is offered
as `static const uint32_t keymap_link_wait_ms`:
two durations, declared two ways.
The review of 2026-11-10 found it.
The owner's rule of 2026-11-11 settles which way:
a `static const` keeps its type, and a `#define` keeps none.
`openspec/config.yaml`, whose rules every plan and implementation carries, does not state it.
It is the next finding the pass plans ahead.

Decisions:

- **The rule goes to `openspec/config.yaml`'s Engineering section,**
  not the macro alone:
  it binds the next constant, as `switch-without-default`'s rule binds the next switch.
- **`TRANSPORT_FAKE_EVENTS` stays a `#define`, the owner's call of 2026-11-11, and the rule names its case:**
  it sizes an array in a struct (`test/transport_fake.c:9`),
  where C takes only an integer constant expression, which a `const` object is not.
- **No spec delta: `.openspec.yaml` sets `skip_specs: true`,**
  not a requirement in `app-build`:
  the app and its build behave as before,
  where `switch-without-default`'s rule makes the build stop at a forgotten enumerator,
  and OpenSpec's proposal instruction has a change with no behavior to specify set it
  rather than invent a requirement for validation.

## What Changes

- `TRANSPORT_BLUETOOTH_NVM_MS` becomes `static const uint32_t transport_bluetooth_nvm_ms`;
  both waits stay 200 ms.
- `openspec/config.yaml`'s Engineering section gains the rule:
  a named constant is a `static const` of its type,
  and a `#define` names one only where C requires an integer constant expression.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

None: the change sets `skip_specs: true`.

## Impact

`src/platform/transport_bluetooth.c` and `openspec/config.yaml`.
The core, the tests, `README.md` and `docs/` stay as they are:
`docs/internals/bluetooth.md:100` and `docs/internals/usb.md:77` give the 200 ms, not the macro.

# Proposal

## Why

Which link the remote is on crosses to the transport encoded twice:
`keymap.c` turns the link it has just moved to into `ActionUseUsb` or `ActionUseBluetooth`
(`src/core/keymap.c:32-35`, `:133-135`),
and `actions.c` turns that action back into the `Link` (`src/core/actions.c:51-56`),
though `keymap_link()` answers it already, and the screen follows that (`src/platform/app.c:69`).
The two enumerators stand in all five switches over `Action`
(`actions.c:14`, `feedback.c:4`, `:28`, `deed.c:4`, `src/ui/view_idle.c:88`),
on 28 lines of `src` and `test`, and a third link would bring a third.
`link-from-the-keymap` kept them, as `app_perform`'s wait reset read them
(`openspec/changes/archive/2026-11-12-link-from-the-keymap/proposal.md:43-46`);
`wait-from-the-keymap` took that reset out and left them to the transport alone
(`openspec/changes/archive/2026-11-13-wait-from-the-keymap/proposal.md:65`, `45611df`),
so it comes now.

Decisions:

- **The platform puts the transport on the link the keymap holds, after every event, as it does the screen,**
  not one action with the link beside it, which `link-from-the-keymap` weighed:
  the keymap answers its link after every event, so any action for the switch says it twice.
- **`transport_use` leaves the core's `transport.h` for `transport_hid.h`, as `transport_hid_set_link`,**
  not kept in the core:
  no core file calls it once the cases go,
  and a header is written for its one consumer (`openspec/config.yaml:106-107`).
- **`transport_hid_set_link` returns at once on the link in use, as `backlight_set` on the light it holds (`src/platform/backlight.c:31-33`),**
  not the keymap answering whether the link moved:
  a state followed after every event cannot be missed, where an answer read once can.
  It is `transport_hid.c`'s one `if`, where `link-from-the-keymap` left none (its `tasks.md:34`):
  it asks whether the link asked for is open, never which link to call.
- **The two tests of the switch go (`test/actions_test.c:130-150`), 95 tests turning 93,**
  not kept against a `transport_use` in the fake:
  they pin the translation this change deletes,
  and the keymap's suite still pins what the hold does to the link.
- **Twelve tests of that suite tell by `keymap_link` whether the link moved (`design.md`, the tests),**
  not by the hold's answer:
  it becomes `ActionNone` whether the hold switches or not,
  and nine of them would no longer tell a switch from none.
- **The requirement goes to `host-tests`, beside the wait's and the backlight's,**
  not `skip_specs`, as `link-from-the-keymap` had it:
  the hold's proof moves to the link the keymap answers,
  and `Nothing counted in the platform` greps `ActionUse` (`openspec/specs/host-tests/spec.md:64`), a name nothing would hold.
- **One device check, by the owner's eye and a key over each link, as `link-from-the-keymap`'s task 6.1,**
  not none:
  no suite builds `app.c` or `transport_hid.c`,
  and on USB the console goes, with its frames (`docs/internals/usb.md:34-37`).
- **Complex, so implemented on `feature/transport-from-the-link`:**
  the core's contract loses a function and `Action` two enumerators,
  `app.c` and `transport_hid.c` are rewired with no host test, and one check takes the device.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `refactor(transport): follow the keymap's link`.**

## What Changes

- Right's hold on `No link` answers no action, the keymap's link being its answer;
  `ActionUseUsb` and `ActionUseBluetooth` go, with their cases in five switches.
- `app.c` hands the transport the keymap's link after every event, after the screen, through `transport_hid_set_link`;
  `transport.h` loses `transport_use`.
- The fake keeps no link, `test/actions_test.c` loses two tests,
  and twelve of `test/keymap_test.c` read the link.
- `docs/internals/architecture.md` says who hands the transport its link;
  `docs/development/host-tests.md` drops the fake's reader of it.
- Every board, key, buzz and second stays as it is.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `host-tests`: the core decides which link is in use, the keymap's suite pins it and the platform follows it;
  `Nothing counted in the platform` greps the wait alone.

## Impact

`src/core/`: `action.h`, `keymap.c`, `actions.c`, `feedback.c`, `deed.c`, `transport.h`;
`src/ui/view_idle.c`;
`src/platform/`: `app.c`, `transport_hid.h`, `transport_hid.c`;
`test/`: every suite but the display's, and the fake;
`docs/internals/architecture.md` and `docs/development/host-tests.md`.
`ActionGiveUp` and `ActionStartOver` stay:
they name what the transport does with the link in use (`src/core/actions.c:45-50`), not which link it is.
The other sources, `README.md` and the rest of `docs/` stay as they are.

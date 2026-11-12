# Proposal

## Why

Which link the remote is on, Bluetooth or USB, is held in three places:
`Keymap.on_usb` (`src/core/keymap.c:13`), a `bool`, though `src/core/link.h` names the links;
`Screen.link` (`src/platform/screen.c:13`), which `app_perform` sets by turning the switch action into a `Link` (`src/platform/app.c:38`);
and `Transport`'s two pointers, one of them `NULL` (`src/platform/transport_hid.c:8-13`).
Each opens on Bluetooth by a line of its own (`keymap.c:64`, `screen.c:37`, `transport_hid.c:29-30`),
and a hold on `No link` moves each on its own, in `keymap_handle`, `app_perform` and `actions_perform`:
six places agree by hand.
`transport_hid.c` then asks which link is live in every call:
`transport->usb` in four functions (`:35`, `:44`, `:52`, `:60`),
`transport->bluetooth` in `transport_withdraw` (`:68`), as USB has no `withdraw`,
while `transport_use_usb` and `transport_use_bluetooth` (`:74-86`) mirror each other.
The review of 2026-11-10 found the three places.
The `if` was `usb-transport`'s choice (`openspec/changes/archive/2026-11-09-usb-transport/design.md:73-75`),
which the owner's bar of 2026-11-12 overturns:
SOLID, a header as the interface, no `if` asking which implementation is live.
It comes now, as `same-screen-gesture`, whose `keymap.c` it edits, landed in `34985ac`.

Decisions:

- **`Keymap` holds a `Link`, and `keymap_link()` answers it beside `keymap_mode()`,**
  not the `bool`:
  the core already names its links, and the platform then reads the link as it reads the mode.
- **`screen_alloc` and `transport_hid_alloc` take `keymap_link()`,**
  not Bluetooth of their own:
  the keymap alone decides the link, at launch as after a hold.
- **`app_settle` hands the screen the keymap's mode and link before the action,**
  not the link alone, in `app_perform` on a switch:
  the screen follows the core with no action to read,
  and the board still changes before a switch holds the loop 700 ms (`app.c:35`).
- **`transport_hid.c` calls the link in use through `TransportLink`,
  six function pointers that `transport_bluetooth.c` and `transport_usb.c` each fill,**
  not an `if` in every call:
  the firmware's BadUSB reaches these two links so (`bad_usb_hid.h:15-31`),
  and a link is then its own files and one table row.
- **`transport.h` takes the link as `transport_use(Transport*, Link)`,**
  not a function per link:
  that is the same ladder in the core's contract,
  and the fake then records a `Link`, not a `bool`.
- **`ActionUseUsb` and `ActionUseBluetooth` stay,**
  not one action with the link beside it:
  an action stays the keymap's whole answer,
  and `app_perform`'s wait reset, a finding of its own, reads these two.
- **One device check, by eye and by a key over each link,**
  not one per board:
  the platform layer has no host test,
  and launch, the hold both ways and a key are where the link reaches the screen and the transport.
- **No spec delta: `.openspec.yaml` sets `skip_specs: true`,**
  as no spec-level behavior changes:
  every board and key stays, and no spec says whether a board shows before or after its action's keys.

## What Changes

- `keymap` holds a `Link`, answered by `keymap_link()`;
  `on_usb` goes.
- `transport.h` declares `transport_use` in place of `transport_use_usb` and `transport_use_bluetooth`;
  `actions.c` and the fake follow.
- New `src/platform/transport_link.h` names what `transport_hid.c` needs of a link:
  `transport_bluetooth` and `transport_usb` become its two instances,
  and `transport_hid.c` calls the one in use, with no `if`.
- `screen_alloc` and `transport_hid_alloc` take the keymap's link;
  `app_settle` hands the screen the keymap's mode and link before the action,
  and `app_perform` no longer turns an action into a `Link`.
- `test/keymap_test.c` pins the link at launch, through the holds and through every mode;
  `test/actions_test.c` reads the link the transport was told to use.
- `docs/internals/architecture.md` names the new header and says when `app.c` hands the screen the board;
  `docs/development/host-tests.md` names the fake's new reader.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

None: the change sets `skip_specs: true`.

## Impact

In the core, `link.h`, `keymap.h`, `keymap.c`, `transport.h` and `actions.c`;
under `test/`, the keymap's and the actions' suites and the fake;
in the platform, `app.c`, `screen.h`, `screen.c`, the new `transport_link.h`,
and the `.h` and `.c` of `transport_hid`, `transport_bluetooth` and `transport_usb`;
in `docs/`, `internals/architecture.md` and `development/host-tests.md`.
`feedback`, `deed`, the views and `README.md` stay as they are.

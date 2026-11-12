# Design

## Context

See `proposal.md`, Why.
The core holds the link;
`app.c`, `screen.c` and the `transport_*` files take its answer.
They stay platform code:
the event loop, the GUI thread, the link queue, the HID codes and both stacks exist only in the firmware.

| Fact | Where |
|---|---|
| The functions of BadUSB's `BadUsbHidApi` take a `void* inst`, and each link fills one `static const` | `bad_usb_hid.h:15-31`, `bad_usb_hid.c:88-104`, `:238-254`, firmware 1.5.1 |
| `-Wall -Wextra -Werror` stop the SDK's build at one enum passed as another and at an unused `static` function, yet warn of no field a designated initializer leaves out | `sdk.opts`; the toolchain's `gcc.info`, Warning Options |

## Goals / Non-Goals

**Goals:** the proposal's, with every board and key as before.

**Non-Goals:** the 60 s counted afresh, which `app_perform` keeps zeroing on a switch:
the restart rule in `app.c` is a finding of its own;
`src/ui/view.c`'s four branches on the link (`:8`, `:27`, `:35`, `:49`),
which pick a mark or a line by it and are proved by frames.

## Decisions

Each file keeps its task, the sources' as `docs/internals/architecture.md:23-40` gives them.
The new `src/platform/transport_link.h` names what `transport_hid.c` needs of a link:

```c
typedef struct {
    void* (*alloc)(FuriMessageQueue* link_queue);
    void (*free)(void* instance);
    void (*press)(void* instance, uint16_t key);
    void (*release)(void* instance, uint16_t key);
    void (*offer)(void* instance);
    void (*withdraw)(void* instance);
} TransportLink;
```

`transport_bluetooth.c` and `transport_usb.c` make their functions `static`, with these types,
and each defines its `TransportLink`, `transport_bluetooth` or `transport_usb`,
every slot named for its function, as `.offer = transport_usb_offer,`.
Its header declares that `extern const` alone.
`transport_hid.c` holds the one in use:

```c
struct Transport {
    FuriMessageQueue* link_queue;
    const TransportLink* link;
    void* instance;
};

static const TransportLink* const transport_hid_links[LinkMAX] = {
    [LinkBluetooth] = &transport_bluetooth,
    [LinkUsb] = &transport_usb,
};

static void transport_hid_open(Transport* transport, Link link) {
    transport->link = transport_hid_links[link];
    transport->instance = transport->link->alloc(transport->link_queue);
}

static void transport_hid_close(Transport* transport) {
    transport->link->free(transport->instance);
}

/* A Connected that one link left queued would read as the other's. */
void transport_use(Transport* transport, Link link) {
    transport_hid_close(transport);
    furi_message_queue_reset(transport->link_queue);
    transport_hid_open(transport, link);
}
```

`transport_hid_alloc` opens the link it is handed, `transport_hid_free` closes it,
and `transport.h`'s other four functions are each one call through `transport->link`,
so every slot has one caller.
`actions.c` calls `transport_use` with `LinkUsb` and `LinkBluetooth`,
and the fake keeps the `Link` it is told, opening on Bluetooth, behind `transport_fake_link()`.
In `keymap.c`, `keymap_set_screen` takes a `Link`,
and Right's hold moves from before the `switch` into `ModeNoLink`'s case,
so each mode's answers sit in its own case:

```c
static const Link keymap_other_links[LinkMAX] = {
    [LinkBluetooth] = LinkUsb,
    [LinkUsb] = LinkBluetooth,
};

static const Action keymap_use_actions[LinkMAX] = {
    [LinkBluetooth] = ActionUseBluetooth,
    [LinkUsb] = ActionUseUsb,
};
```

```c
    case ModeNoLink:
        if(key == KeymapKeyRight && event == KeymapEventLong) {
            Link link = keymap_other_links[keymap->link];
            keymap_set_screen(keymap, ModeNoLink, link);
            return keymap_use_actions[link];
        }
        break;
```

```c
/* The board goes first: a switch then holds this loop at least 700 ms. */
static void app_settle(App* app, Action action) {
    screen_set_mode(app->screen, keymap_mode(app->keymap));
    screen_set_link(app->screen, keymap_link(app->keymap));
    if(action != ActionNone) {
        app_perform(app, action);
    }
    app_follow_wait(app);
}
```

`app_perform`'s `if` keeps `app->wait_ms = 0` alone,
its comment cut to "A wait of zero makes app_follow_wait start the 60 s afresh.",
and `app_alloc` hands `keymap_link(app->keymap)` to both allocs, as their last argument.
In `docs/internals/architecture.md`, `transport_link.h`'s row follows `transport_hid`'s,
and the sentence after a press's path becomes:
"Before step 5, `app.c` hands `screen.c` the mode and the link `keymap.c` holds.
After step 7, it hands the firmware's dolphin the deed `deed.c` names,
buzzes as `feedback.c` says, and has `screen.c` flash."
`docs/development/host-tests.md:32` names `transport_fake_link`.

- **`void* instance`,**
  not a base struct each link embeds first, as the SDK's `FuriHalBleProfileBase` (`profile_interface.h:13-18`):
  a function converts it with no cast, as every callback here its `context`,
  and `transport_hid_open` alone pairs it with its table.
- **USB fills `withdraw` with `transport_usb_withdraw`, which does nothing,**
  not a `NULL` the router checks:
  a keyboard on a cable does not advertise (`docs/internals/usb.md:120-121`),
  and a slot to check is the inverted `if` again.
- **`Link` gains `LinkMAX`, which sizes the three tables,**
  not their last index:
  `KeymapKeyMAX` and `TransportKeyMAX` size theirs so (`keymap.c:17`, `transport_hid.c:15`).
- **One new test walks every mode on USB,**
  not a link assert in every test that holds Right:
  the hold alone changes the link.
  `test_fresh_keymap_is_on_bluetooth` pins the launch;
  `test_right_held_again_switches_back_and_forth` asserts `LinkUsb`, `LinkBluetooth`, `LinkUsb` after its holds;
  `test_link_stays_through_every_mode` takes `keymap_test_alloc_on_usb` through the other four modes
  and back to `ModeNoLink`, asserting `LinkUsb` at each.

Test seams:

- `keymap.c`: the host suite, through `keymap.h`.
  Red is the undeclared `keymap_link`, green a getter over `on_usb`,
  and the refactor brings `Link link`, the tables and the hold's move with the suite green.
- `actions.c`: the host suite, against the fake.
  Red is the undeclared `transport_fake_link`.
- The platform files have no host test:
  `mise run build` types every call and stops at a function left out of its table,
  task 3.1's `git grep` counts six slots a link, each holding its namesake,
  and task 6.1 sends a key over each link, switches both ways and leaves.

## Risks / Trade-offs

- The board shows before its action runs → it is the keymap's state, settled before the answer; the keys follow in the same pass of the loop.

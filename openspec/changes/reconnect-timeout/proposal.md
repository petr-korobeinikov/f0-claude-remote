# Proposal

## Why

With no link the remote never gives up:
`D_disconnected` reads `Reconnecting...` for as long as the app is open,
under a backlight `a764474` forces on with no spec behind it,
while the firmware advertises every 1–2.5 s once its first 60 s pass.
On 2026-11-06 the owner asked that a long loss turn the backlight off,
stop the reconnecting,
and leave a screen offering a press to start over.
CD drew that screen, `D_stopped`, at the owner's order.

Decisions, already settled by the owner:

- **A long loss darkens the screen and stops offering the link, and a press starts over,**
  not reconnecting for as long as the app is open.
  Past the window the Mac can no longer reconnect by itself.
- **60 s, not the owner's first 30 s,** on these facts.
  Upper bound: after each start and each drop the firmware advertises every 80–100 ms for 60 s,
  then every 1–2.5 s (`gap.c:15`, `:430-435`, `:474`);
  in that slow phase a connect took 28 s and once timed out at 31 s (`ble-transport`'s task 4.5).
  Lower bound: stopping during the `Verify code` dialog cuts the link pairing has just made (`gap.c:481-483`);
  Bluetooth gives pairing 30 s (Core, Vol 3, Part H, 3.4), the Mac gave up at 30 s with error 4807,
  and our own dialog stood 20 s,
  so 30 s leave next to nothing for reaching the Mac's settings, and 60 s leave 30.
  A paired Mac in range reconnects in 0.87 s under either.
- **Any button but Back starts over,**
  not OK alone:
  the press that lights the dark screen reaches the app too, as `app-keymap`'s `Backlight off` check saw.
- **On `D_stopped`, Back's press exits at once,**
  not a hold as on every other screen.
- **The change applies after `v0.1`, as an improvement,**
  not before `release`:
  so it edits the site the docs changes build, not `README.md`.

Decisions proposed here, for the owner to strike:

- **The 60 s also run from the opening,**
  not only after a link existed:
  an app opened away from the Mac would otherwise stay lit until Back.
- **A press during `Reconnecting...` does not extend them,**
  not a restart on each press:
  an extension would outlast the firmware's fast phase.
- **Neither stopping nor starting over buzzes or flashes,**
  not a buzz:
  a buzz means sent.
- **Once stopped, a link report changes nothing until a press,**
  not a return to `D_Main`:
  stopping waits until the radio is idle (`furi_hal_bt.c:261-267`),
  so a report read afterwards is stale.
- **The backlight gets a requirement,
  and `A press acts with the backlight off` a scenario that can happen,**
  not the stale one:
  since `a764474` the backlight never times out with the link up.

## What Changes

- `keymap` gains `ModeStopped`, the 60-s wait, and a press that starts over or, for Back, exits;
  `actions` withdraws and offers the link through `transport`.
- `transport_bluetooth` stops and restarts advertising.
- New `src/platform/backlight.h`, `.c`; `app` runs the wait's timer.
- `view` draws `D_stopped`.
- `test/` pins the rules; the fake transport records the offer.
- The site: `guide/pairing.md` trades the slow-advertising caveat for a press on `D_stopped`,
  `guide/using-the-remote.md` and `guide/troubleshooting.md` gain the screen and its fix,
  `internals/bluetooth.md` the stop, and `internals/architecture.md` the new files.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `bluetooth-link`: the link is offered 60 s at a time, not for as long as the app is open.
- `app-screen`: `D_stopped`, Back's press there, and the backlight's rule.
- `button-actions`: the press that starts over, and a backlight-off scenario that can happen.
- `user-guide`: `Using the remote` lists `D_stopped`.
- `internals-guide`: `Bluetooth` covers the stop.

## Impact

No dependency.
The change needs `docs`, `docs-development` and `docs-internals` archived,
since two of its deltas modify their specs; validate prints INFO until then.
Holding the link down for a check takes forgetting the device on the Mac,
so the device checks pair twice more.

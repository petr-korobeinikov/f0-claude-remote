# Design

## Context

See `proposal.md` for why.
A firmware bump can break the second list:

- **Documented:** `dolphin_deed` is thread safe and asynchronous (`dolphin.h:32-36`);
  20 XP a day for each application, levels at 300 and 1800 XP
  ([Pet dolphin](https://docs.flipper.net/zero/basics/dolphin)).
- **Firmware 1.5.1 sources alone:**
  `PluginStart` weighs 2 XP, `PluginGameStart` 1, `PluginGameWin` 10, `GpioUartBridge` 1, `U2fAuthorized` 3,
  all one application, capped at 20 a day (`dolphin_deed.c:38-44`, `:54`);
  the Apps menu earns `PluginStart` for each app it starts (`loader_applications.c:130`);
  the day turns at 05:00 by the device's clock (`dolphin.c:30`, `:189-213`);
  `butthurt` drops 1 at the day's 1st, 16th, 31st and 46th XP across applications (`dolphin_state.c:155-163`)
  and rises 1 for every 48 h with no deed (`dolphin.c:19`, `:229`);
  Passport reads `Happy` up to 4, `Angry` from 10 (`passport.c:43-52`);
  XP at 300 or 1800 takes no more until the desktop shows the level-up
  (`dolphin_state.c:140-145`, `animation_manager.c:195-200`);
  the state is saved 30 s after the last deed (`dolphin.c:16`, `:230`) to `/int/.dolphin.state`:
  8 bytes of header (`saved_struct.c:8-14`), then the day's Plugin XP at byte 14
  and the total XP at bytes 20-23, little-endian (`dolphin_state.h:13-21`).

## Goals / Non-Goals

**Goals:** the specs; the rule in the core.

**Non-Goals:** a deed at launch; a count or cap of the app's own; XP or mood on the app's screen;
a setting; deeds for Enter, the arrows, Esc or effort.

## Decisions

| File | Layer | Its one task | Check | Seam |
|---|---|---|---|---|
| `src/core/deed.h`, `.c` | Core | Says which deed an action earns | `test/deed_test.c` | `Action` |
| `src/platform/app.c` | Platform | Runs the application's event loop | `.dolphin.state` over USB | `deed.h`, `dolphin/dolphin.h` |

`DolphinDeed` and `dolphin_deed` exist only in the firmware.

```c
typedef enum {
    DeedNone,
    DeedDictation,
    DeedClear,
    DeedMAX,
} Deed;

Deed deed_earned(Action action);
```

Tests: `ActionVoiceStart` earns `DeedDictation`, `ActionClear` earns `DeedClear`,
and every other `Action`, `ActionNone` through `ActionExit`, earns `DeedNone`.

- **`deed` in the core names what an action earns, as `feedback` names how it confirms itself,**
  not a switch in `app.c`:
  the owner's pick is a rule, pinned on the host.
- **A dictation earns on `ActionVoiceStart`,**
  not on `ActionVoiceStop`:
  the start comes once per hold, from a press with the link up,
  where the stop comes both from the release and from a lost link (`keymap_set_connected`).
- **`PluginGameStart` for a dictation, `PluginGameWin` for a `/clear`,**
  not `GpioUartBridge`, the Plugin application's other deed of 1 XP, nor another application's deed:
  `GpioUartBridge` names a built-in app's act, another application's would draw on that app's cap,
  and `PluginGameWin` is the only deed of 10.
- **`app.c` maps `Deed` to `DolphinDeed` in a table of two and calls `dolphin_deed` after the keys,**
  not a platform file of its own:
  two entries, and `app_perform` already turns `feedback`'s answers into firmware calls.
- **Each deed goes to the firmware as it comes,**
  not counted or capped by the app:
  the firmware caps the day across every catalog app, so a count of the app's own would drift from it.

## Risks / Trade-offs

- Another app has spent the day's 20 XP → the remote earns nothing until 05:00, by the firmware's design.
- XP stands at 300 or 1800 → no deed counts until the desktop's level-up; task 4.1 reads it first.
- A firmware bump reweighs a deed → the guide names firmware 1.5.1, bumped with the SDK's pin.

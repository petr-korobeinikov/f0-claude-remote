# Proposal

## Why

A day of work with the remote earns the device's dolphin nothing beyond its launch:
`git grep dolphin src` finds no call,
and only the Apps menu pays, 2 XP for each app it starts (`loader_applications.c:130`).
The owner launches the app once or twice a day,
and that launch carries the whole day's dictations and sessions.
Firmware 1.5.1 gives the Flipper Apps Catalog's apps 20 XP a day together (`dolphin_deed.c:54`)
and takes a point off the dolphin's anger, `butthurt`, at the day's 1st and 16th XP (`dolphin_state.c:155-163`),
so the remote leaves at least 16 XP and one point a day unearned.
On 2026-11-07 the owner asked for XP earned by the work, not by the launch alone.

Decisions, already settled by the owner:

- **A dictation earns 1 XP and a `/clear` 10,**
  not dictations alone, sessions alone, nor 2 XP for every 30 minutes of use:
  the owner's pick of the four offered;
  a launch and two `/clear`s fill the day, and a day with no `/clear` still counts.
- **The change lands before `reconnect-timeout`,**
  planned ahead on `develop`:
  it rests on nothing unlanded.

Decisions proposed here, for the owner to strike:

- **No deed of the app's own at launch,**
  not `PluginStart` at start, as the stock Bluetooth remote sends (`hid.c:224`):
  the Apps menu already pays the launch its 2 XP, so the app's own would pay it twice;
  a launch outside the Apps menu, as by `mise run launch`, stays unpaid.
- **A deed is silent,**
  not a `+10 XP` on the screen:
  the action's buzz already confirms it, and Passport shows the mood and the level.
- **`README.md`'s and Home's Features and the Flipper Apps Catalog's description gain a line each,**
  not the guide's section alone:
  a Flipper owner looks for what an app does for the dolphin.

## What Changes

- New `src/core/deed.h`, `.c`: which deed an action earns;
  new `test/deed_test.c` pins it.
- `src/platform/app.c` hands the deed to the firmware's dolphin.
- `docs/guide/using-the-remote.md` gains `The dolphin`;
  `docs/internals/architecture.md` names the new files;
  `README.md`'s and `docs/index.md`'s Features and `catalog/description.md` gain a line each.

## Capabilities

### New Capabilities

- `dolphin-deeds`: the XP the remote's work earns the device's dolphin.

### Modified Capabilities

- `user-guide`: `Using the remote` gives the dolphin's XP.

## Impact

No dependency, and `application.fam` does not change:
`dolphin_deed` is public in SDK 1.5.1 (`api_symbols.csv:1000`).
The cap is shared:
two `/clear`s spend the day's 20 XP for every catalog app, games included,
and a game played first leaves the remote less.
`reconnect-timeout`'s task 2.1 counts 44 tests before; this change adds 3.

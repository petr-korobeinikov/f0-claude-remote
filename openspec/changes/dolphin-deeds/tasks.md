# Tasks

Group 2 is test-first and host-only;
`Every other action`, `A dictation ends` and `No link` rest on its tests and on `keymap`'s.
Every check on the device needs it unlocked on USB, on firmware 1.5.1, paired.
`.dolphin.state` is read with the SDK's `scripts/storage.py read /int/.dolphin.state`, with no press,
at least 30 s after the last deed, its bytes as the design lays them out.
A hold buzzes the device and acts on the focused session,
so the owner holds, with a scratch Claude Code session focused on the Mac.

## 1. The state file

- [ ] 1.1 Read `.dolphin.state`. Check: 40 bytes; byte 14 at most 20; bytes 24-27, `butthurt`, at most 14. Else stop and raise it: the design's layout is wrong.

## 2. The rule

- [ ] 2.1 Add `test/deed_test.c` with the design's tests. Check: with `src/core/deed.h` and `.c` empty, `mise run test` fails on the undeclared names; declare the design's names with a stub returning `DeedNone`: it fails on an assertion; implement: exit 0, 47 tests, 44 before.

## 3. The deed on the device

- [ ] 3.1 In `app.c`, hand `deed_earned`'s answer to `dolphin_deed` through the design's table. Check: test, build and lint exit 0; `git grep -E '#include <(furi|gui|input|notification|bt|dolphin)' -- src/core` prints nothing; `git grep -n dolphin_deed -- src` finds `app.c` alone.
- [ ] 3.2 Write the docs the proposal names: `using-the-remote.md`'s `## The dolphin`; `architecture.md`'s row for `deed.h` and `deed.c`, and `deed.c` in a press's path; a Features line in `README.md` and `docs/index.md`; a sentence in `catalog/description.md`. Check: `mise run docs-build` exits 0; `The dolphin`, `Home`, `Files`, `No refused construct`.

## 4. With the owner

- [ ] 4.1 Read the state: byte 14 must leave room for 11 and the XP must not be 300 or 1800, else wait for 05:00 or the desktop's level-up. Run `mise run launch`; read after 30 s. Observe: unchanged, `Launch`.
- [ ] 4.2 The owner holds OK for a second; read after 30 s; the owner holds Left; read after 30 s. Observe: byte 14 and the XP up 1, then 10: `Dictation`, `Clear`.

## 5. Close

- [ ] 5.1 Check: `mise run test`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate dolphin-deeds --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `wc -w` holds the budgets.

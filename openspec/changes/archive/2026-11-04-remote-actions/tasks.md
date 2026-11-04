# Tasks

Every check on the device needs it unlocked on USB, on firmware 1.5.1,
paired, launched from its desktop.
A press buzzes the device,
so a check that presses waits for the owner's word, and the owner presses.
Every press, on either keyboard,
lands in a second Claude Code session, focused in another pane:
`/clear`, Enter or Esc in the implementing session would wipe or interrupt it.
Groups 2 and 3 are test-first; the rest holds no rule.
`Claude working`, `A menu` and the Mac's side of a drop stay unproven:
the documentation holds the first two,
and no real use holds a link down for the third.

## 1. The Mac's side

- [x] 1.1 With text in the prompt and the design's lines not yet bound, the owner presses Ctrl+Q, then `ctrl+x ctrl+l`, on the Mac's keyboard. Observe: both `Keys not bound`.
- [x] 1.2 Put the design's block in `keybindings.json`, replacing `alt+k`. Under a non-Latin layout, the owner holds Ctrl+Q for 10 seconds, speaking, then releases. Observe: `Held`, `Released`, `Non-Latin layout`. Note `claude --version`. If it fails, stop and raise it: the voice key falls.
  Claude Code 2.1.311.
- [x] 1.3 Under a non-Latin layout, the owner presses `ctrl+x ctrl+x`, then, with a conversation behind it and text in the prompt, `ctrl+x ctrl+l`. Observe: effort-control's `Effort max`; `Clear`, `Non-Latin layout`, `A draft`. If one fails, stop and raise it.
  `ctrl+x ctrl+q` started a recording, so the owner moved effort max to `ctrl+x ctrl+x`, which set `max` and started none.
  The owner's `ctrl+x ctrl+l` ran `/clear` 4 times by the session logs, in a session with no conversation, so the screen showed no change.
  `Clear` and `A draft` read from frames of a throwaway session in tmux, the same bytes behind one exchange.
- [x] 1.4 Rename `README.md`'s `## Effort` to `## Keys on the Mac`: the design's block, what each button sends, `/voice hold` and key repeat, `/resume` after a stray clear, waiting out `Voice: processing…`. Check: the block equals the owner's lines; both `The block`.

## 2. The keys

- [x] 2.1 In `test/actions_test.c`, write the design's tests for its keys table, narrowing `test_every_other_action_sends_nothing` to none and exit. Check: `mise run test` fails on undeclared `TransportKeyEnter`. Add the names to `transport.h`: it fails on an assertion. Extend `actions_perform`. Check: exit 0, count noted, 33 before.
  40 tests.
- [x] 2.2 Map the new names in `transport_bluetooth.c`, per the design. Check: `mise run test`, build and lint exit 0; `git grep -E '#include <(furi|gui|input|notification|bt)' -- src/core` prints nothing.

## 3. The drop

- [x] 3.1 In `test/keymap_test.c`, pin the design's answers. Check: `mise run test` fails on a void value. Have `keymap_set_connected` answer. Check: exit 0, count noted.
  44 tests.
- [x] 3.2 Have `app_link_callback` perform the answer, as `app_input_callback` does. Check: test, build and lint exit 0.

## 4. With the owner

Under a non-Latin layout, after `mise run launch`.

- [x] 4.1 The owner holds OK for 10 seconds, speaking, then releases. Observe: `Held`, `Released`, `Non-Latin layout`, so a hold over Bluetooth keeps dictation running. If it fails, stop and raise it.
  By the owner's own trial in place of these steps, reported as working as wanted; the log holds a dictated message, but not the hold's length or the layout.
- [x] 4.2 The owner taps OK on that transcript; once the turn ends, taps Up, then Down; sends a long prompt and presses Right during its turn. Observe: `Enter`, `The arrows`, `Esc`.
  By the same trial. The log shows the message sent; the arrows and Esc leave no record, so they rest on the owner's word.
- [x] 4.3 The owner holds Down, then, with a conversation behind it, Left. Observe: effort-control's `Effort max`, `Clear`.
  By the same trial. The log shows `/effort max` and `/clear`, and cannot tell the device from the keyboard.

## 5. Close

- [x] 5.1 Check: `mise run test`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate remote-actions --strict` exit 0; after a build and lint, `git status --short` lists only this change's files; `git grep -nE 'Option\+K|alt\+k|ctrl\+x ctrl\+q' -- README.md src` prints nothing; `wc -w` holds the budgets.

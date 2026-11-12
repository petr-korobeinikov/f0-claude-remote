# Tasks

No task takes the device or the owner's hands:
every mode answers a report as before, and the host suite pins all ten answers.

## 1. The link report

- [ ] 1.1 Note `mise run test`'s count of `ok` lines. Put `ModeProbe` after `ModeStopped` in `src/core/mode.h`, run `mise run test`, then take it out. Check: it exits non-zero naming `ModeProbe` at `keymap.c:101` alone; `git diff --exit-code -- src/core/mode.h` passes.
- [ ] 1.2 Delete both tables and rewrite `keymap_set_connected` per the design. Check: `mise run test` exits 0 with 1.1's count; `mise run build` and `mise exec -- ufbt lint` exit 0; `No table over a mode or an action` and `No table sized by its rows` hold; `git diff --stat -- src test` lists `src/core/keymap.c` alone.
- [ ] 1.3 Put `ModeProbe` back, run `mise run test` and `mise run build`, then take it out. Check: both exit non-zero, the test naming it at both switches of `keymap.c`, the build at the one that answers a link report; with it out, both exit 0 and `git diff --exit-code -- src/core/mode.h` passes.

## 2. The rule

- [ ] 2.1 Add the design's paragraph to `openspec/config.yaml`. Check: `git diff -- openspec/config.yaml` adds that paragraph alone, after the one ending "follows the switch."; `mise exec -- openspec validate mode-without-tables --strict` exits 0.

## 3. The docs against the tree

- [ ] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 4. Close

- [ ] 4.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate mode-without-tables --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

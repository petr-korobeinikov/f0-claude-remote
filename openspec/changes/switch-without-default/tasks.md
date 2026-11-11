# Tasks

No task takes the device or the owner's hands:
no answer changes, the host suites pin the core's,
and `view_idle_draw_flash` keeps its seven boxes as they are.

## 1. The switches

- [ ] 1.1 Note `mise run test`'s count of `ok` lines. Put `ActionProbe` after `ActionUseBluetooth` in `src/core/action.h`, run `mise run build` and `mise run test`, then take it out. Check: both exit 0 with the probe in, so `app-build`'s `A new action` fails; `git diff --exit-code -- src/core/action.h` passes.
- [ ] 1.2 Rewrite the five switches per the design's table. Check: `mise run test` exits 0 with 1.1's count; `mise run build` and `mise exec -- ufbt lint` exit 0; `git grep -n 'default:' -- src test` prints nothing; read against the table, `git diff -- src` keeps every action's answer.
- [ ] 1.3 Put `ActionProbe` back, run `mise run build` and `mise run test`, then take it out. Check: both exit non-zero with the probe in, naming it; with it out, both exit 0 and `git diff --exit-code -- src/core/action.h` passes.

## 2. The rule

- [ ] 2.1 Add the design's paragraph to `openspec/config.yaml`. Check: `git diff -- openspec/config.yaml` adds that paragraph alone, after the one ending "holds what an editor can enforce."; `mise exec -- openspec validate switch-without-default --strict` exits 0.

## 3. The docs against the tree

- [ ] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 4. Close

- [ ] 4.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate switch-without-default --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

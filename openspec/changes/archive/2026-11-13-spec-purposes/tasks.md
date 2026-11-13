# Tasks

No task takes the device or the owner's hands: no code changes.

## 1. The Purposes

- [x] 1.1 Replace line 5 of `openspec/specs/app-screen/spec.md` and of `openspec/specs/release/spec.md` with the proposal's Purposes, word for word. Check: `mise exec -- openspec validate --specs --strict` passes 27 of 27; `git diff --stat` shows those two files, one line each.

## 2. The docs against the tree

- [x] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

  No findings: `git grep -i -E "purpose|openspec validate|specs/"` over `README.md` and `docs/` hits no Purpose and no validation count;
  `docs/contributing/workflow.md` names only `openspec view`. `mise run docs-build` exits 0.

## 3. Close

- [x] 3.1 Check: `mise exec -- prek run --all-files` and `mise exec -- openspec validate spec-purposes --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

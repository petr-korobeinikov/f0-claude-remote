# Tasks

No task changes business logic: the build, a pixel comparison and the owner's look are the tests, per the design.
Nothing touches the device.

## 1. Logo

- [x] 1.1 Draw `docs/assets/logo.svg` and set `theme.logo` in `zensical.toml`, per the design.
  Check: `Pixels` holds through a scratchpad script; `mise run docs-build` exits 0.
  28 black pixels, the same set in both; every page's header points at `assets/logo.svg`, none draws `book-open`.
- [x] 1.2 Check: `Header` holds in `mise run docs-dev`, seen by the owner.
  Accepted by the owner on 2026-11-07.

## 2. Close

- [x] 2.1 Check: `mise exec -- prek run --all-files`, `mise run docs-build`
  and `mise exec -- openspec validate docs-logo --strict` exit 0;
  `git status --short` lists only the design's files and this change; `wc -w` holds the budgets.
  Words: proposal 147, design 261, tasks 113 as planned, spec 113.

# Tasks

No task changes business logic: the build and a line search are the tests, per the design.
Nothing touches the device: the debugging steps move unchanged.

## 1. Base

- [ ] 1.1 Check that `openspec/specs/` holds `docs-site`, `user-guide` and `device-debug`
  with the requirements this change modifies or removes, differing from its deltas only where they edit;
  else rebase the deltas, or stop and raise it. Check: `diff` of each requirement block.

## 2. Development

- [ ] 2.1 Write Toolchain, Build and run, Host tests and CLion per `developer-guide`, from the design's sources.
  Check: their scenarios hold; the line search finds `CONTRIBUTING.md`'s opening and `## Toolchain` in them;
  `mise run docs-build` exits 0.
- [ ] 2.2 Write Debug probe and Debugging from the README's `## Debugging`, then take it out of the README per `user-guide`.
  Check: `Moved` holds through the line search; `README` holds; `The debugging steps are on the site` holds by reading.

## 3. Contributing

- [ ] 3.1 Write Workflow, Hooks and CI and Release from `CONTRIBUTING.md`, then reduce it to the pointer.
  Check: `contributor-guide`'s `Moved` and `Pointer` hold; `mise run docs-build` exits 0.
- [ ] 3.2 Add the pages to the sidebar and nav per `docs-site`, and `docs/` to the config's Housekeeping.
  Check: `Sidebar` holds on the build; `rg -n 'CONTRIBUTING\.md#|#debugging\)'` finds nothing outside `openspec/`.

## 4. Close

- [ ] 4.1 Check: `mise exec -- prek run --all-files`, `mise run test`, `mise run docs-build`
  and `mise exec -- openspec validate docs-development --strict` exit 0;
  `git status --short` lists only the design's files and this change; `wc -w` holds the budgets.
  The pages publish with the owner's push of `develop`.

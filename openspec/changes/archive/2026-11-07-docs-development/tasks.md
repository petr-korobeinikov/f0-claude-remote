# Tasks

No task changes business logic: the build and a line search are the tests, per the design.
Nothing touches the device: the debugging steps move unchanged.

## 1. Base

- [x] 1.1 Check that `openspec/specs/` holds `docs-site`, `user-guide` and `device-debug`
  with the requirements this change modifies or removes, differing from its deltas only where they edit;
  else rebase the deltas, or stop and raise it. Check: `diff` of each requirement block.

## 2. Development

- [x] 2.1 Write Toolchain, Build and run, Host tests and CLion per `developer-guide`, from the design's sources.
  Check: their scenarios hold; the line search finds `CONTRIBUTING.md`'s opening and `## Toolchain` in them;
  `mise run docs-build` exits 0.
  Toolchain keeps line 9's `clion .` with line 8, whose sentence it ends, and the `check-workflow`, `docs-*`
  and `release` items, which the design placed nowhere; `debug`'s item went to Debugging. Build, lint, 44 `ok` lines.
- [x] 2.2 Write Debug probe and Debugging from the README's `## Debugging`, then take it out of the README per `user-guide`.
  Check: `Moved` holds through the line search; `README` holds; `The debugging steps are on the site` holds by reading.
  `SECURITY.md`'s `README.md#before-a-session` link, broken by the cut, now points at Debug probe's Risks.

## 3. Contributing

- [x] 3.1 Write Workflow, Hooks and CI and Release from `CONTRIBUTING.md`, then reduce it to the pointer.
  Check: `contributor-guide`'s `Moved` and `Pointer` hold; `mise run docs-build` exits 0.
- [x] 3.2 Add the pages to `zensical.toml`'s `nav` and `llmstxt` sections per `docs-site`, and `docs/` to the config's Housekeeping.
  Check: `Sidebar` and `Index` hold on the build; `rg -n 'CONTRIBUTING\.md#|#debugging\)'` finds nothing outside `openspec/`.

## 4. Close

- [x] 4.1 Check: `mise exec -- prek run --all-files`, `mise run test`, `mise run docs-build`
  and `mise exec -- openspec validate docs-development --strict` exit 0;
  `git status --short` lists only the design's files and this change; `wc -w` holds the budgets.
  The pages publish with the owner's push of `develop`.
  All exit 0; `git status` adds `SECURITY.md`, per 2.2.

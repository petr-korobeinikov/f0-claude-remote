# Tasks

No task changes business logic: the build, a file check and link checks are the tests, per the design.
Nothing touches the device.

## 1. Base

- [ ] 1.1 Check that `openspec/specs/docs-site/spec.md` holds the sidebar requirement as `docs-development` leaves it,
  differing from this change's delta only by Internals; else rebase the delta, or stop and raise it.
  Check: `diff` of the two blocks.

## 2. Pages

- [ ] 2.1 Write Architecture per `internals-guide`, from the design's sources.
  Check: `Files` through the script over `git ls-files src`; `Path` against `src/platform/app.c` and `screen.c`;
  `mise run docs-build` exits 0.
- [ ] 2.2 Write Bluetooth from `bluetooth-link` and the archived `ble-transport`.
  Check: `Marks` holds by reading; each firmware link pins `1.5.1` and answers 200 to `curl -fsSI`.
- [ ] 2.3 Write Keys in Claude Code from the three control specs and the archived `effort-actions` and `remote-actions`.
  Check: `Marks` holds by reading; each documentation link answers 200 to `curl -fsSI`.
- [ ] 2.4 Add Internals to `zensical.toml`'s `nav`, between Development and Contributing, and as a `llmstxt` section, per `docs-site`.
  Check: `Sidebar` and `Index` hold on the build.

## 3. Close

- [ ] 3.1 Check: `mise exec -- prek run --all-files`, `mise run test`, `mise run docs-build`
  and `mise exec -- openspec validate docs-internals --strict` exit 0;
  `git status --short` lists only the design's files and this change; `wc -w` holds the budgets.
  The pages publish with the owner's push of `develop`.

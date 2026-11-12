# Tasks

No task takes the device or the owner's hands.
The scenarios rest on renders, 1.1's for the lines and 2.1's for the file around them;
`Rerun on a release commit` is first checked on `v0.3`.

## 1. The rule

- [x] 1.1 Render `mise exec -- git-cliff --config catalog/cliff.toml --tag v0.3`: `v0.1:` ends with `- Look up the tag's commit by its full ref`, its 17th line. Add the parser per the design. Check: the render keeps `v0.3:`'s four lines and `v0.2:`'s ten and drops that one; adding the range `d55327c..76ab21a` prints `v0.3:`, a blank line and `- Internal changes only`.
  Renumbered `0.2` to `0.3` across the plan inline, as `v0.2` was cut on 2026-11-11 (roadmap); `catalog/changelog.md:19` is now `:29`.
  Render 33 lines before, 32 after; the range's render also ends in a blank line.

## 2. The prepend

- [x] 2.1 Change `.mise/tasks/release:62` per the design. Check: that command, with `v0.3` for the tag and `--prepend` aimed at a copy of `catalog/changelog.md` in a `mktemp -d` directory, leaves the copy equal to its `--unreleased` render, `v0.3:`'s section and a blank line, followed by `catalog/changelog.md`; `mise exec -- prek run --files .mise/tasks/release` exits 0.
  `--prepend` writes nothing to stdout; `cmp` of the copy and render-then-file: equal.
- [x] 2.2 Rewrite `openspec/config.yaml:40-41`, `docs/contributing/release.md:15` and its `## Changelog` (lines 81-86) per the design. Check: `git grep -n catalog/changelog.md` lists each statement in this task's notes, none saying the release regenerates the file or `cliff.toml` writes it; `mise run docs-build` exits 0.
  Outside `openspec/`: `release.yml:99` (the manifest's reference), `.mise/tasks/release:41,62,67`, `docs/contributing/release.md:16,89`, `openspec/config.yaml:41`.
  `openspec/specs/` still says "written whole" until `/opsx:sync`.

## 3. The docs against the tree

- [x] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.
  None false beyond 2.2's: `README.md` has no changelog statement, `docs/` only `release.md`.
  `release.md`'s `## Changelog` no longer calls the catalog file `cliff.toml`'s, closing the review's finding on it (roadmap 7).

## 4. Close

- [x] 4.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate flipper-apps-catalog-changelog --strict`; `git diff --exit-code develop -- catalog/changelog.md` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.
  All exit 0; words: proposal 349, design 445, tasks 382, specs 232 and 157.

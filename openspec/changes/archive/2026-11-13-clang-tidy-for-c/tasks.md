# Tasks

`Clean` and `ci`'s `Commands` close in task 1.1, `Named one by one` in 1.2,
`A finding` and `A finding in a header` in 1.3, `The caller's configuration` in 1.4,
`developer-guide`'s `Content` in 1.5, `In CLion` in 2.1, and `developer-guide`'s `As written` in 3.1.
`ci`'s `Shell checked`, `Pinned SDK` and `Unbuilt checkout` hold as they are.
Task 1.1 takes a Docker daemon, as `mise run check-workflow` does:
a `build` job failing there at its new step is a stop, not a fix.

## 1. The checks and their command

- [x] 1.1 Add `.clang-tidy`, `.clangd`, `.mise/tasks/clang-tidy` and the `build` job's step, per the design. Check: `mise run clang-tidy` exits 0 and leaves `git status --short` as it was; `mise run check-workflow` ends with `test`, `build`, `hygiene` and `docs` each `Job succeeded`, the `build` job's log holding `mise run clang-tidy`.
  The first `check-workflow` lost `test` to a timeout cloning `mise-action`, before any step; the rerun passed all four.
  `mise run clang-tidy` took 5.3 s with the build on the Mac; the `build` job's step 2.3 s.
- [x] 1.2 Check the names. `~/Applications/CLion.app/Contents/bin/clang/mac/aarch64/bin/clang-tidy --verify-config --config-file=.clang-tidy` prints `No config errors detected.`; `grep -c '^    - ' .clang-tidy` prints 44; `git grep --untracked -c -E '^ +- .*\*' -- .clang-tidy` prints `.clang-tidy:1`.
- [x] 1.3 Plant a closing `return;` in `keymap_free` and `const bool connected` in `keymap.h`'s `keymap_set_connected`. Check: `mise run clang-tidy` exits non-zero, printing `src/core/keymap.c` over `readability-redundant-control-flow` and `src/core/keymap.h` over `readability-avoid-const-params-in-decls`; with the `const` out, the header's two lines are gone.
- [x] 1.4 With the `return;` still in, write `Diagnostics:`, `  ClangTidy:`, `    Remove: readability-redundant-control-flow` to `$dir/Library/Preferences/clangd/config.yaml`, `$dir` from `mktemp -d`. Check: `mise exec -- env HOME="$dir" sh .mise/tasks/clang-tidy` exits non-zero; with the script's `HOME="$home" XDG_CONFIG_HOME="$home"` out, 0; with both back and the `return;` out, 0, and `git diff --stat -- src` prints nothing.
  The script without the two variables ran from a copy in a scratch folder, the tracked file untouched.
- [x] 1.5 Edit the four pages per the design. Check: `mise run docs-build` exits 0; `git grep -c 'mise run clang-tidy' -- docs` prints `:1` for `contributing/hooks-and-ci.md`, `development/build-and-run.md` and `development/clion.md`; `git grep -c 'and `clang-tidy`' -- docs/development/toolchain.md` prints `docs/development/toolchain.md:1`.

## 2. With the owner

- [x] 2.1 The owner opens `src/ui/view.c`, `src/platform/transport_hid.c` and `src/platform/app.c` in CLion, then types a closing `return;` into `keymap_free` and undoes it. Observe by eye: no warning on `view_links` or `transport_hid_links`, no error in `app.c`, the `return;` marked `readability-redundant-control-flow`.

## 3. Close

- [x] 3.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise run clang-tidy`; `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- actionlint -pyflakes=`; `mise exec -- openspec validate clang-tidy-for-c --strict` exit 0; `git diff --name-only develop` lists the proposal's Impact and this change's files alone; `wc -w` holds the budgets.
  The design counts 878 words against 800, within a tenth.

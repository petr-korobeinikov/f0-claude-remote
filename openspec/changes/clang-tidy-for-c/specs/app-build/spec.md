## ADDED Requirements

### Requirement: The sources pass the checks picked for C
The repository SHALL name its clang-tidy checks one by one in `.clang-tidy`,
and every `.c` and `.h` under `src/` SHALL pass them in the toolchain's `clangd`.
`mise run clang-tidy` SHALL build, read each of those files and fail at a finding, printing its file and its check,
whatever the caller's own clangd configuration says,
and SHALL leave the worktree as it found it.

#### Scenario: Clean
- **WHEN** `mise run clang-tidy` runs in a set-up clone
- **THEN** it exits 0, and `git status --short` prints what it printed before

#### Scenario: A finding
- **WHEN** `keymap_free` in `src/core/keymap.c` gains a closing `return;` and `mise run clang-tidy` runs
- **THEN** it exits non-zero and prints `src/core/keymap.c` and `readability-redundant-control-flow`

#### Scenario: A finding in a header
- **WHEN** `keymap_set_connected` in `src/core/keymap.h` declares `const bool connected` and `mise run clang-tidy` runs
- **THEN** it exits non-zero and prints `src/core/keymap.h` and `readability-avoid-const-params-in-decls`

#### Scenario: The caller's configuration
- **WHEN** the first finding stands and the task's script runs under a `HOME` whose clangd `config.yaml` removes `readability-redundant-control-flow`
- **THEN** it still exits non-zero

#### Scenario: Named one by one
- **WHEN** `git grep -c -E '^ +- .*\*' -- .clang-tidy` runs
- **THEN** it prints `.clang-tidy:1`, the opening `-*`

#### Scenario: In CLion
- **WHEN** `src/ui/view.c` and `src/platform/transport_hid.c` are open in CLion
- **THEN** `view_links` and `transport_hid_links` carry no warning,
  and a closing `return;` typed into `keymap_free` is marked `readability-redundant-control-flow`

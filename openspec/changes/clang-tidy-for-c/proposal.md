# Proposal

## Why

The repository names no clang-tidy checks, so CLion runs its own default set, picked for C++.
Its `cppcoreguidelines-interfaces-global-init` marks `view_links` and `transport_hid_links`
(`src/ui/view.c:14`, `src/platform/transport_hid.c:25`) for taking an object's address,
an address constant in C (C11 §6.6p9), which GCC passes under `-Wall -Werror`.
On `src` these are the only two findings of the set JetBrains documents (article `CPP-A-90276519`), both false,
and outside CLion nothing reads a check at all:
an agent writes the code and sees no editor.
The owner asked for a `.clang-tidy` on 2026-11-13.
The numbers below were measured that day, with the owner's yes, on a copy of `src/` outside the checkout
(`design.md`, the probe).

Decisions:

- **A command holds the checks, `mise run clang-tidy`, and CI's `build` job runs it,**
  not CLion alone:
  a finding would first show at the owner's review.
- **The toolchain's own `clangd` 18.1.8 runs them, in its check mode,**
  not a `clang-tidy` pinned in `mise.toml`:
  the toolchain ships none (`.ufbt/toolchain/arm64-darwin/bin`), CLion's is its own fork of LLVM 24,
  and only clangd asks `arm-none-eabi-gcc` for its headers and target, so nothing is installed.
  The price: check mode is clangd's troubleshooting aid, not a linter, and fails on errors alone
  (18.1.8's `Check.cpp:113-122`), so `.clang-tidy` makes every finding an error,
  and the spec's two planted findings tell when a new toolchain changes that.
- **Each check is named,**
  not taken by family:
  `bugprone-` holds 80 names in LLVM 18.1.8 and 109 in CLion's,
  so a family would read wider in CLion than under the command.
- **A check is in when it acts on C, reads what this code can hold,
  fires alike in both clangds on a planted sample and marks a line that goes wrong, a defect or an accident:**
  43 of the 515 names in 18.1.8's list,
  not CLion's default trimmed of C++, which no rule picked.
  A style the project has not chosen, a policy it has not written, a guess
  and what the build's flags stop already stay out (`design.md`, Out).
- **The 43 find nothing in the 34 files of `src`, in either clangd, so no source changes;**
  31 of them are in CLion's default by name.
- **`test/` stays outside,**
  not checked as well:
  the compile database names no test, and the suites build with the system `cc`, unpinned (`.mise/tasks/test:11`).
- **Complex, so implemented on `feature/clang-tidy-for-c`:**
  the Linux half was read, not run
  (`flipperzero-toolchain`'s `linux/scripts/build-linux-flipper-tools.sh:37-52` builds `clangd` from the same 18.1.8),
  so task 1.1 runs it under `act` before anything rests on it,
  and one check takes the owner's eye in CLion.
- **It builds on `develop` alone:**
  `transport-from-the-link`, queued, shares no file and no requirement with it;
  applied after it, task 1.1 reads the tree that change leaves.
- **It lands as `ci: check the sources with clang-tidy`,**
  as `ci: lint shell scripts with shellcheck` did (`44281fd`).

## What Changes

- `.clang-tidy` names 43 checks and makes a finding an error.
- `.clangd` fits clangd to the build's commands.
- `mise run clang-tidy` builds, then runs the checks over every `.c` and `.h` under `src/`;
  CI's `build` job runs it after lint.
- Four pages of `docs/` gain the task, and where CLion takes its checks.
- No source, test, board, key or second changes.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `app-build`: the sources pass the checks `.clang-tidy` names, under a command.
- `ci`: the `build` job runs that command.
- `developer-guide`: Build and run gives the command, and CLion names the file its checks come from.

## Impact

`.clang-tidy`, `.clangd`, `.mise/tasks/clang-tidy`, `.github/workflows/ci.yml`;
`docs/development/build-and-run.md`, `clion.md` and `toolchain.md`, `docs/contributing/hooks-and-ci.md`.
No dependency: `mise.toml` and `mise.lock` stay as they are.
`src/`, `test/`, `README.md` and the rest of `docs/` stay as they are.

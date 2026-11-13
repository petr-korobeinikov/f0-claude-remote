# Design

## Context

See `proposal.md`, Why.
No layer of the app changes: it lands in the root's configuration, a task, CI and `docs/`.

The probe, 2026-11-13: a copy of `src/` in a scratch folder, the three files below beside it,
each `.c` and `.h` read in check mode by the toolchain's `clangd` 18.1.8 and by the one CLion 2026.3.0.1 bundles.

- Under the 43 checks the 34 files raise no finding in either, the toolchain's done in 3 s,
  and each check fires in both on a sample of its one construct.
- Under JetBrains' default checks CLion's raises 2.
- A sample of the SDK's macros and calls raises one finding in both,
  `bugprone-too-small-loop-variable` on `for(uint8_t i = 0; i < COUNT_OF(…); i++)`:
  the set wants a counter as wide as its bound.

## Goals / Non-Goals

**Goals:** the spec's.

**Non-Goals:** `clang-analyzer-`, which neither clangd runs.

## Decisions

`.clang-tidy` names the checks, `.clangd` fits clangd to the build's commands,
`.mise/tasks/clang-tidy`, mode 755, runs the checks over the sources.

`.clang-tidy`, the 43 names each a line `    - <name>`:

```yaml
# Each check acts on C, fires alike in the toolchain's clangd and in CLion's, and marks a defect
# or an accident. A style, a policy and a guess stay out.
Checks:
    - -*
```

```text
bugprone-branch-clone
bugprone-chained-comparison
bugprone-implicit-widening-of-multiplication-result
bugprone-incorrect-roundings
bugprone-infinite-loop
bugprone-integer-division
bugprone-macro-repeated-side-effects
bugprone-misplaced-operator-in-strlen-in-alloc
bugprone-misplaced-pointer-arithmetic-in-alloc
bugprone-misplaced-widening-cast
bugprone-multiple-statement-macro
bugprone-narrowing-conversions
bugprone-not-null-terminated-result
bugprone-redundant-branch-condition
bugprone-reserved-identifier
bugprone-sizeof-expression
bugprone-string-literal-with-embedded-nul
bugprone-suspicious-include
bugprone-suspicious-memory-comparison
bugprone-suspicious-memset-usage
bugprone-suspicious-missing-comma
bugprone-suspicious-realloc-usage
bugprone-suspicious-semicolon
bugprone-suspicious-string-compare
bugprone-swapped-arguments
bugprone-terminating-continue
bugprone-too-small-loop-variable
bugprone-unused-return-value
cert-err34-c
cert-flp30-c
misc-confusable-identifiers
misc-misleading-bidirectional
misc-misleading-identifier
misc-misplaced-const
misc-redundant-expression
readability-avoid-const-params-in-decls
readability-duplicate-include
readability-inconsistent-declaration-parameter-name
readability-misplaced-array-index
readability-non-const-parameter
readability-redundant-casting
readability-redundant-control-flow
readability-redundant-function-ptr-dereference
```

```yaml
# clangd's check mode fails on errors alone.
WarningsAsErrors: "*"
```

`.clangd`:

```yaml
# clang knows no -mword-relocations, which the build hands arm-none-eabi-gcc.
CompileFlags:
    Remove: [-mword-relocations]

# clangd leaves out a check it has not timed, bugprone-chained-comparison among them.
Diagnostics:
    ClangTidy:
        FastCheckFilter: None
```

`.mise/tasks/clang-tidy`:

```sh
#!/bin/sh
#MISE description="Check the sources against .clang-tidy"
#MISE depends=["build"]
set -eu

# The toolchain's bin stays off PATH, in a folder named for the platform.
set -- "$UFBT_HOME"/toolchain/*/bin
bin=$1

# clangd puts a user-wide config above the project's.
home=$(mktemp -d)
trap 'rm -rf "$home"' EXIT

# A header's finding shows only when the header is read itself, and clangd's editor features,
# left on, would count failures of their own.
status=0
for file in src/*/*.c src/*/*.h; do
    if ! output=$(HOME="$home" XDG_CONFIG_HOME="$home" "$bin/clangd" --check="$file" \
        --check-locations=false --query-driver="$bin/arm-none-eabi-gcc" --log=error 2>&1); then
        printf '%s\n%s\n' "$file" "$output"
        status=1
    fi
done
exit "$status"
```

- **`.clangd` holds the two settings the probe needed,**
  not the SDK's template whole (`.ufbt/current/scripts/ufbt/project_template/.clangd`):
  its other lines answer nothing this tree raises.
- **The task depends on `build`,**
  not on a build run before it:
  it reads the compile database and the generated icons header, both the build's.

`ci.yml`'s `build` job ends with `- run: mise run clang-tidy`, after lint.

The pages:

- `docs/development/build-and-run.md` gains `mise run clang-tidy` in its first block, and the item
  "`clang-tidy` builds it, then checks every `.c` and `.h` under `src/` against `.clang-tidy`."
- `docs/development/clion.md` gains `## Checks`:
  "CLion takes its clang-tidy checks from `.clang-tidy`, in place of its own default set,
  and `mise run clang-tidy` runs the same list."
- `docs/development/toolchain.md:45` reads "`build`, `launch` and `clang-tidy` [Build and run](build-and-run.md),".
- `docs/contributing/hooks-and-ci.md:46` gains `mise run clang-tidy`, last in `build`'s row.

Out, by the proposal's rule.
Every family but seven is written for a library, a platform, another code base or C++ (the manual's table of prefixes).
Of those seven, 18.1.8's sources limit to C++ all but 135 names:
43 in, 13 aliases, 79 out, 30 of them reading only what C lacks. The rest:

- No such API on the firmware (`api_symbols.csv`):
  `bugprone-bad-signal-to-kill-thread`, `bugprone-posix-return`, `bugprone-signal-handler`,
  `bugprone-spuriously-wake-up-functions`, `bugprone-unsafe-functions`, `cert-env33-c`,
  `concurrency-thread-canceltype-asynchronous`, `misc-non-copyable-objects`.
- Silent on its sample, as the build's `-DNDEBUG` empties `assert`:
  `bugprone-assert-side-effect`, `misc-static-assert`.
- Silent, as the toolchain's clangd replays no `#define` or `#if` of a file's preamble and no nested include:
  `bugprone-macro-parentheses`, `misc-header-include-cycle`,
  `readability-avoid-unconditional-preprocessor-if`, `readability-redundant-preprocessor`.
- Silent in C17, or left out by clangd:
  `bugprone-bool-pointer-implicit-conversion`, `bugprone-suspicious-enum-usage`,
  `readability-simplify-boolean-expr`, `misc-include-cleaner`.
- The clangds differ:
  `bugprone-casting-through-void` (18 alone fires),
  `readability-identifier-naming` (24 alone marks `_a` in the SDK's `MAX`).
- The build's flags stop it:
  `misc-unused-parameters`, `performance-type-promotion-in-math-fn`, `readability-const-return-type`,
  `readability-misleading-indentation`, `readability-redundant-declaration`.
- A style not chosen:
  `readability-braces-around-statements` (against `.clang-format:52`), `readability-else-after-return`,
  `readability-isolate-declaration`, `readability-uppercase-literal-suffix`,
  `readability-avoid-nested-conditional-operator`, `readability-magic-numbers` (167 findings),
  `readability-identifier-length`, `readability-function-size`, `readability-function-cognitive-complexity`,
  `bugprone-argument-comment`, `bugprone-assignment-in-if-condition`, `bugprone-inc-dec-in-conditions`.
- A policy not written:
  `bugprone-switch-missing-default-case` (asks the `default:` `app-build` forbids),
  `cert-err33-c`, `cert-msc30-c`, `cert-msc32-c`, `misc-no-recursion`,
  `portability-restrict-system-includes`, `concurrency-mt-unsafe`.
- A guess, or false on the SDK's idioms:
  `bugprone-easily-swappable-parameters` (6 findings, 3 on callbacks the SDK shapes),
  `readability-suspicious-call-argument`,
  `performance-no-int-to-ptr` (5, each a `furi_check`),
  `bugprone-multi-level-implicit-pointer-conversion` (a queue of pointers),
  `bugprone-signed-char-misuse` (`int8_t` read as a character).

Test seams: the task's exit status and output on a finding planted in a source and in a header;
the `build` job under `act`; CLion by the owner's eye.

## Risks / Trade-offs

- The Linux toolchain's `clangd` differs from the Mac's → task 1.1 runs the `build` job under `act` first.
- A misspelled check is off without a word → task 1.2 has CLion's `clang-tidy --verify-config` name it.
- clangd reads the build's `-Wall -Wextra -Werror` as clang, so a warning GCC lacks fails the task → today's tree raises none in either.
- CLion's LLVM 24 marks what 18.1.8 passes → the two agree on `src` and on the 43 samples.

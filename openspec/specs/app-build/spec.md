# app-build Specification

## Purpose

Builds the Flipper application from a pinned SDK,
on a machine that holds nothing but mise and a clone of the repository.

## Requirements

### Requirement: The SDK version is pinned
Setup SHALL deploy the SDK version pinned in `mise.toml`,
whatever the update server's release channel offers that day.

#### Scenario: Pinned version deployed
- **WHEN** `mise run setup` finishes
- **THEN** `mise exec -- ufbt status version` prints the value of `FLIPPER_SDK_VERSION`

#### Scenario: Wrong version refused
- **WHEN** an SDK other than the pinned one is already deployed
- **THEN** `mise run setup` replaces it or exits non-zero

### Requirement: The toolchain needs no system prerequisite
The toolchain SHALL run natively on Apple Silicon,
with no Rosetta and nothing installed outside the project.

#### Scenario: Build without Rosetta
- **WHEN** a build runs on arm64 macOS with `/Library/Apple/usr/libexec/oah/libRosettaRuntime` absent
- **THEN** it exits 0 using `.ufbt/toolchain/arm64-darwin`

### Requirement: A build yields the application package
Building from the repository root SHALL produce a package for hardware target 7
against the pinned SDK's API.

#### Scenario: Successful build
- **WHEN** `mise exec -- ufbt` runs at the root
- **THEN** it exits 0, `dist/f0_claude_remote.fap` exists and the log reads `Target: 7, API: 88.2`

### Requirement: The worktree stays clean
Setup, build and lint SHALL change no tracked file and leave no untracked one.

#### Scenario: After setup, build and lint
- **WHEN** `mise run setup`, `mise exec -- ufbt` and `mise exec -- ufbt lint` have run on a fresh clone
- **THEN** `git status --short` prints nothing

### Requirement: The sources follow Flipper's format
The repository SHALL carry Flipper's `.clang-format` from the pinned SDK,
and the sources SHALL pass the SDK's linter against it.

#### Scenario: Lint
- **WHEN** `mise exec -- ufbt lint` runs
- **THEN** it exits 0

### Requirement: Tasks build the application and start it on the device
`mise run build` SHALL build the application,
and `mise run launch` SHALL build it, upload it to the device and start it.

#### Scenario: Build
- **WHEN** `mise run build` runs
- **THEN** it exits 0 and `dist/f0_claude_remote.fap` exists

#### Scenario: Launch
- **WHEN** `mise run launch` runs with the device unlocked on USB
- **THEN** it exits 0 and its log reads `Launching app: /ext/apps/Bluetooth/f0_claude_remote.fap`

#### Scenario: Device locked by PIN
- **WHEN** `mise run launch` runs while a PIN locks the device, which drops its USB console
- **THEN** it exits non-zero

### Requirement: The app's pictures live under `asset/`
The repository SHALL keep every picture of the app under `asset/`, in one folder per purpose:
the Apps menu's icon in `asset/icon/`, what the screens draw in `asset/image/`, the screenshots in `asset/screenshot/`.
Only the site's link to a screenshot SHALL stand outside it.

#### Scenario: Paths
- **WHEN** `git ls-files '*.png' | grep -vE '^asset/(icon|image|screenshot)/[^/]+$'` runs
- **THEN** it prints `docs/assets/main.png` alone

#### Scenario: The icon
- **WHEN** `git ls-files asset/icon` runs
- **THEN** it prints `asset/icon/f0_claude_remote.png` alone

### Requirement: The package takes its pictures from `asset/`
The package SHALL carry the icon from `asset/icon/` and every image from `asset/image/`,
and nothing from `asset/screenshot/`.

#### Scenario: Images
- **WHEN** `mise run build` has run
- **THEN** `.ufbt/build/f0_claude_remote/f0_claude_remote_icons.h` declares one `I_<name>` for each `asset/image/<name>.png`,
  and no `I_main` or `I_f0_claude_remote`

### Requirement: A missing picture stops the build
`mise run build` SHALL fail when the icon or an image the screens draw is missing,
so no package ships without it.

#### Scenario: A missing image
- **WHEN** `asset/image/up.png` is missing and `mise run build` runs
- **THEN** it exits non-zero and names `I_up`

#### Scenario: A missing icon
- **WHEN** `asset/icon/f0_claude_remote.png` is missing and `mise run build` runs
- **THEN** it exits non-zero and names the file

### Requirement: An undecided enumerator stops the build
A `switch` over an enum SHALL name each of its enumerators and carry no `default:`,
so `mise run build` fails at an enumerator a switch leaves out,
and no package ships one that no switch has decided.

#### Scenario: No default
- **WHEN** `git grep -n 'default:' -- src test` runs
- **THEN** it prints nothing

#### Scenario: A new action
- **WHEN** an enumerator is added to the enum in `src/core/action.h`, and to no switch, and `mise run build` runs
- **THEN** it exits non-zero and names that enumerator

### Requirement: No table decides for a mode or an action
An enum a `switch` is over SHALL index no table,
where a row left out reads as zero with no warning:
`mise run build` SHALL fail at a mode that a link report has no answer for.

#### Scenario: No table over a mode or an action
- **WHEN** `git grep -n -E '\[(Mode|Action)[A-Z]' -- src` runs
- **THEN** it prints nothing

#### Scenario: A new mode
- **WHEN** an enumerator is added to the enum in `src/core/mode.h`, and to no switch, and `mise run build` runs
- **THEN** it exits non-zero and names that enumerator at the switch that answers a link report

### Requirement: A table is as long as its enum
A table indexed by an enum SHALL take its length from the enum's `…MAX`, never from its rows,
so no value of the enum reads past the table's end.

#### Scenario: No table sized by its rows
- **WHEN** `git grep -n '\[\] = {' -- src` runs
- **THEN** it prints nothing

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

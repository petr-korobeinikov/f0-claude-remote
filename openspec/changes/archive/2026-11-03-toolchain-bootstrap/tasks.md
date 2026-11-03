# Tasks

## 1. Pins and guards

- [x] 1.1 Add the design's `[tools]` and `[env]` to `mise.toml`, without `[tool_config]`; run `mise trust`. Check: `mise config ls` lists `pypi:ufbt`, `actionlint`, `act`.
- [x] 1.2 Run `mise install uv python@3.14.10`, then `mise lock`. Check: `grep -A4 'tools."pypi:ufbt"' mise.lock` shows a `uv =` line.
  Deviation: the committed `mise.lock` was format 0, so `mise lock` exited 0 with no graph; `mise lock --upgrade` moved it to format 3 and wrote `.mise/locks/pypi-ufbt/0.2.6~2f4519ff`.
- [x] 1.3 Add `[tool_config] locked = true`; run `mise install`. Check: `mise.lock` unchanged by it; `mise ls` shows `pypi:ufbt 0.2.6`.
- [x] 1.4 Check the private interpreter: `pyvenv.cfg` names `installs/python/3.14.10/bin`; no `pip` in the venv's `bin`; no ufbt in the runtime's `site-packages`; `mise which python3` exits non-zero.
- [x] 1.5 Uninstall `pypi:ufbt@0.2.6`; copy `.mise/locks` aside, then delete the graph, then restore and edit `uv.lock`. Check: `mise install` and `mise exec -- ufbt status` refuse each time. Restore; `mise install` passes.
  Deviation: a failed backup lost the first graph; `mise lock` regenerated it under `locked = true`. Same 3 packages and 6 hash prefixes; digest moved `59db5c20` → `5a0d159b`, attributed to `exclude-newer` (lock time minus the global 1h `minimum_release_age`), not byte-proven: the original is gone.
- [x] 1.6 With shims on `PATH`, run `ufbt --version` from a `mktemp -d` directory. Check: non-zero exit.
- [x] 1.7 Check: `PYTHONPATH=/x mise exec -- printenv PYTHONPATH` exits non-zero; `mise exec -- sh -c 'command -v python3; command -v pip'` prints nothing under the runtime, venv, or `.ufbt`.

## 2. SDK and template

- [x] 2.1 Run `mise exec -- sh -c 'ufbt update --branch "$FLIPPER_SDK_VERSION"'`. Check: `mise exec -- ufbt status version` prints `1.5.1`.
- [x] 2.2 Run `mise exec -- ufbt create APPID=f0_claude_remote`. Check: `application.fam`, the `.c`, `.png`, `images/.gitkeep`, `.github/workflows/build.yml` exist; `.ufbt/toolchain/arm64-darwin` exists; `~/.ufbt` absent.
- [x] 2.3 Write `.gitignore` and `.env` per the design. Check: `git check-ignore .ufbt/ dist/ .vscode/ .act/` lists all four; `env -u UFBT_HOME "$(mise where pypi:ufbt)/bin/ufbt" status state_dir` prints `<root>/.ufbt`.
- [x] 2.4 Run `mise exec -- ufbt`, then `mise exec -- ufbt lint`. Check: `dist/f0_claude_remote.fap` exists; log reads `Target: 7, API: 88.2`; lint exits 0; `.clang-format` appears; `libRosettaRuntime` still absent.
  Lint exits 0 but warns on 4 hyphenated folders under `openspec/`; it renames nothing.

## 3. Proof

- [x] 3.1 Write `.mise/tasks/setup` per the design. Check: `mise run setup` exits 0; `ufbt status version` equals the pin, and returns to it after editing `version` in `.ufbt/current/ufbt_state.json` and rerunning.
  Deviation, owner's decision: the SDK's `.clang-format` ends in a blank line, which `end-of-file-fixer` refuses and `ufbt lint` restores; `setup` trims the template's copy after `ufbt update`, and no file is exempt from the hook.
- [x] 3.2 Touch a timestamp file; run `mise exec -- ufbt -c`, then `mise exec -- ufbt`. Check: `find` under `mise where pypi:ufbt` and `mise where python@3.14.10` prints nothing newer.
  0 newer under both; 457 `.pyc` under `.ufbt/pycache`.
- [x] 3.3 Put a stub `ccache` logging `CCACHE_DISABLE` on `PATH`; clean, build, remove it. Check: the log reads `CCACHE_DISABLE=1`.

## 4. Workflow

- [x] 4.1 Apply the design's pins, `permissions`, `ufbt-version` and upload condition to `build.yml`. Check: `mise exec -- actionlint .github/workflows/build.yml` exits 0; every `uses:` holds 40 hex.
- [x] 4.2 Write `.mise/tasks/check-workflow` per the design; start colima; run it. Check: both jobs end `Job succeeded`; `docker inspect` shows no socket mount, network `bridge`. Stop colima; restore the docker context.
  17 minutes for both jobs. Left in the VM: volume `act-toolcache`.

## 5. IDE

- [x] 5.1 Create the symlink `compile_commands.json`. Check: it lists `<root>/f0_claude_remote.c` with a compiler under `<root>/.ufbt/toolchain/arm64-darwin/bin`.
- [x] 5.2 Add a throwaway `probe.c`; run `mise exec -- ufbt`. Check: the database lists it. Delete it, rebuild. Note whether CLion reloads alone.
  CLion's Sync reran at 13:34, about a minute after the 13:32:43 build, with no reload click reported, and `probe.c` resolved with no banner; the delay's trigger was not isolated.
- [x] 5.3 Quit CLion; delete `.idea/`; run `clion .`. Observe: the repository opens; `furi.h` and `gui/gui.h` resolve; the `.c` shows no error; no setting touched; compiler recognized. Note `clion .` versus `clion "$PWD"`.
  `clion .` opens the root; `clion "$PWD"` not needed. Database imported 16 s after start, no click reported. Owner confirmed navigation; `gui/gui.h` was not reported separately.
- [x] 5.4 Quit CLion; start it from Toolbox. Observe: the same navigation holds. Note whether the mise environment is visible, for `debug-and-ide-tasks`.
  Navigation holds. The mise environment is absent from a Toolbox start (13 variables, no `UFBT_HOME`) and present from `clion .`. SDK function bodies are unreachable: 403 headers, no sources; owner accepted.

## 6. Docs

- [x] 6.1 Add a Toolchain section to `README.md`: fresh-clone commands, the pin bump, tasks in `.mise/tasks/`, `ufbt vscode_dist` forbidden. Check: 7.1 runs its commands as written.

## 7. Clean-machine acceptance

- [x] 7.1 Commit to the branch (collapsed at finish); list `/usr/local`; clone into a `mktemp -d` directory; run `mise trust`, `mise install`, `mise run setup`, `mise exec -- ufbt lint`. Check: all exit 0; `git status --short` empty; icons header generated; `~/.ufbt` absent; `/usr/local` listing unchanged.

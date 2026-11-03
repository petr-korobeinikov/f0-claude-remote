# Tasks

## 1. Pins and guards

- [ ] 1.1 Add the design's `[tools]` and `[env]` to `mise.toml`, without `[tool_config]`; run `mise trust`. Check: `mise config ls` lists `pypi:ufbt`, `actionlint`, `act`.
- [ ] 1.2 Run `mise install uv python@3.14.10`, then `mise lock`. Check: `grep -A4 'tools."pypi:ufbt"' mise.lock` shows a `uv =` line.
- [ ] 1.3 Add `[tool_config] locked = true`; run `mise install`. Check: `mise.lock` unchanged by it; `mise ls` shows `pypi:ufbt 0.2.6`.
- [ ] 1.4 Check the private interpreter: `pyvenv.cfg` names `installs/python/3.14.10/bin`; no `pip` in the venv's `bin`; no ufbt in the runtime's `site-packages`; `mise which python3` exits non-zero.
- [ ] 1.5 Uninstall `pypi:ufbt@0.2.6`; copy `.mise/locks` aside, then delete the graph, then restore and edit `uv.lock`. Check: `mise install` and `mise exec -- ufbt status` refuse each time. Restore; `mise install` passes.
- [ ] 1.6 With shims on `PATH`, run `ufbt --version` from a `mktemp -d` directory. Check: non-zero exit.
- [ ] 1.7 Check: `PYTHONPATH=/x mise exec -- printenv PYTHONPATH` exits non-zero; `mise exec -- sh -c 'command -v python3; command -v pip'` prints nothing under the runtime, venv, or `.ufbt`.

## 2. SDK and template

- [ ] 2.1 Run `mise exec -- sh -c 'ufbt update --branch "$FLIPPER_SDK_VERSION"'`. Check: `mise exec -- ufbt status version` prints `1.5.1`.
- [ ] 2.2 Run `mise exec -- ufbt create APPID=f0_claude_remote`. Check: `application.fam`, the `.c`, `.png`, `images/.gitkeep`, `.github/workflows/build.yml` exist; `.ufbt/toolchain/arm64-darwin` exists; `~/.ufbt` absent.
- [ ] 2.3 Write `.gitignore` and `.env` per the design. Check: `git check-ignore .ufbt/ dist/ .vscode/ .act/` lists all four; `env -u UFBT_HOME "$(mise where pypi:ufbt)/bin/ufbt" status state_dir` prints `<root>/.ufbt`.
- [ ] 2.4 Run `mise exec -- ufbt`, then `mise exec -- ufbt lint`. Check: `dist/f0_claude_remote.fap` exists; log reads `Target: 7, API: 88.2`; lint exits 0; `.clang-format` appears; `libRosettaRuntime` still absent.

## 3. Proof

- [ ] 3.1 Write `.mise/tasks/setup` per the design. Check: `mise run setup` exits 0; `ufbt status version` equals the pin, and returns to it after editing `version` in `.ufbt/current/ufbt_state.json` and rerunning.
- [ ] 3.2 Touch a timestamp file; run `mise exec -- ufbt -c`, then `mise exec -- ufbt`. Check: `find` under `mise where pypi:ufbt` and `mise where python@3.14.10` prints nothing newer.
- [ ] 3.3 Put a stub `ccache` logging `CCACHE_DISABLE` on `PATH`; clean, build, remove it. Check: the log reads `CCACHE_DISABLE=1`.

## 4. Workflow

- [ ] 4.1 Apply the design's pins, `permissions`, `ufbt-version` and upload condition to `build.yml`. Check: `mise exec -- actionlint .github/workflows/build.yml` exits 0; every `uses:` holds 40 hex.
- [ ] 4.2 Write `.mise/tasks/check-workflow` per the design; start colima; run it. Check: both jobs end `Job succeeded`; `docker inspect` shows no socket mount, network `bridge`. Stop colima; restore the docker context.

## 5. IDE

- [ ] 5.1 Create the symlink `compile_commands.json`. Check: it lists `<root>/f0_claude_remote.c` with a compiler under `<root>/.ufbt/toolchain/arm64-darwin/bin`.
- [ ] 5.2 Add a throwaway `probe.c`; run `mise exec -- ufbt`. Check: the database lists it. Delete it, rebuild. Note whether CLion reloads alone.
- [ ] 5.3 Quit CLion; delete `.idea/`; run `clion .`. Observe: the repository opens; `furi.h` and `gui/gui.h` resolve; the `.c` shows no error; no setting touched; compiler recognized. Note `clion .` versus `clion "$PWD"`.
- [ ] 5.4 Quit CLion; start it from Toolbox. Observe: the same navigation holds. Note whether the mise environment is visible, for `debug-and-ide-tasks`.

## 6. Docs

- [ ] 6.1 Add a Toolchain section to `README.md`: fresh-clone commands, the pin bump, tasks in `.mise/tasks/`, `ufbt vscode_dist` forbidden. Check: 7.1 runs its commands as written.

## 7. Clean-machine acceptance

- [ ] 7.1 Commit to the branch (collapsed at finish); list `/usr/local`; clone into a `mktemp -d` directory; run `mise trust`, `mise install`, `mise run setup`, `mise exec -- ufbt lint`. Check: all exit 0; `git status --short` empty; icons header generated; `~/.ufbt` absent; `/usr/local` listing unchanged.

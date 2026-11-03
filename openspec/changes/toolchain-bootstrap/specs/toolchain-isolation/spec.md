# Spec Delta

## Purpose

Confines the toolchain's Python interpreters and packages to the project,
away from other interpreters and system paths.

## ADDED Requirements

### Requirement: Tools install only from the committed graph
A project tool SHALL install from the committed graph, wheels only,
refusing a missing or edited graph.

#### Scenario: Fresh clone with unlocked global tools
- **WHEN** `mise install` runs on a fresh clone with global tools absent from `mise.lock`
- **THEN** it exits 0, leaving `mise.lock` unchanged

#### Scenario: Graph missing
- **WHEN** ufbt is uninstalled and `.mise/locks/pypi-ufbt` is gone
- **THEN** `mise install` and `mise exec -- ufbt status` exit non-zero; no install directory appears

#### Scenario: Graph edited
- **WHEN** ufbt is uninstalled and its `uv.lock` mismatches the recorded digest
- **THEN** `mise install` exits non-zero with `digest mismatch`

### Requirement: ufbt runs on a private interpreter
ufbt SHALL run from its own venv on the pinned runtime, without `pip`,
adding nothing to that runtime.

#### Scenario: Venv configuration
- **WHEN** `pyvenv.cfg` is read
- **THEN** `home` names `installs/python/3.14.10/bin` and `include-system-site-packages` is `false`

#### Scenario: No package manager in the venv
- **WHEN** the venv's `bin` is listed
- **THEN** it holds no `pip`

#### Scenario: Runtime stays clean
- **WHEN** the runtime's `site-packages` is listed after a build
- **THEN** it holds no ufbt, oslex or mslex

### Requirement: No project interpreter reaches PATH
The project environment SHALL expose ufbt's commands,
never the runtime, the venv's `python` or the toolchain's `bin`.

#### Scenario: Inside the project environment
- **WHEN** `mise exec -- sh -c 'command -v python3; command -v pip'` runs
- **THEN** no path printed lies under runtime, venv or `.ufbt`

#### Scenario: mise does not activate Python
- **WHEN** `mise which python3` runs in the project
- **THEN** it exits non-zero

### Requirement: ufbt does not run outside the project
ufbt SHALL fail outside the project, even with mise's shims on `PATH`.

#### Scenario: Foreign directory
- **WHEN** `ufbt --version` runs outside the project
- **THEN** it exits non-zero

### Requirement: Toolchain state stays inside the project
SDK, toolchain, build output and Python bytecode SHALL land inside the project,
however ufbt starts from its root.

#### Scenario: Started through mise
- **WHEN** `mise exec -- ufbt status state_dir` runs in any project directory
- **THEN** it prints `<root>/.ufbt`

#### Scenario: Started by full path
- **WHEN** ufbt runs by full path from the root, `UFBT_HOME` unset
- **THEN** `status state_dir` prints `<root>/.ufbt`

#### Scenario: Setup under denial
- **WHEN** `mise run setup` runs on a fresh clone
- **THEN** it exits 0 with writes denied outside the project and temp, a throwaway `HOME` left empty, and `~/.ufbt` absent

#### Scenario: Shared runtime untouched
- **WHEN** a clean build follows a timestamp file
- **THEN** no file under the runtime or the venv is newer

### Requirement: The caller's environment cannot redirect the tools
The project environment SHALL unset `PYTHONPATH` and `PYTHONHOME`,
refuse `pip` outside a venv, forbid uv's Python downloads and disable ccache.

#### Scenario: Inherited Python variables
- **WHEN** a shell exporting `PYTHONPATH` runs `mise exec -- printenv PYTHONPATH`
- **THEN** it exits non-zero

#### Scenario: ccache on PATH
- **WHEN** a stub `ccache` is on `PATH` during a build
- **THEN** the stub records `CCACHE_DISABLE=1`

# Design

## Context

No core or UI lands here:
every file is build tooling, the template's C source an `app` placeholder.

Every build runs two interpreters:
CPython 3.14.10, mise's runtime with ufbt's own venv, never on `PATH`, runs the `ufbt` wrapper;
CPython 3.11 in `.ufbt/toolchain/arm64-darwin/bin` runs SCons and the SDK scripts,
on `PATH` only in the child `ufbt` starts through `fbtenv.sh`.

`mise.toml` gains:

```toml
[tool_config]
locked = true

[tools]
uv = "0.13.7"
actionlint = "1.7.12"
act = "0.2.95"
"pypi:ufbt" = { version = "0.2.6", python = "3.14.10" }

[env]
PYTHONPATH = false
PYTHONHOME = false
PIP_REQUIRE_VIRTUALENV = "true"
UV_PYTHON_DOWNLOADS = "never"
UFBT_HOME = "{{config_root}}/.ufbt"
PYTHONPYCACHEPREFIX = "{{config_root}}/.ufbt/pycache"
CCACHE_DISABLE = "1"
FLIPPER_SDK_VERSION = "1.5.1"
```

Allowed outside the project:
`~/.local/share/mise`, `~/.local/state/mise`, `~/.cache/mise`, `~/.cache/uv`,
`~/.cache/sigstore-rust` at install;
`/private/tmp`, `/private/var/folders` at run.

## Goals / Non-Goals

**Goals:** close each leak below; re-prove closure on every `setup`.

**Non-Goals:** flash, launch, debug, `.run`, build tasks; CI beyond the template's workflow; host tests.

## Decisions

Measured in dry runs unless a source is cited:

| Leak | Closure | Alternative beaten |
|---|---|---|
| Plain install resolves afresh, may build sdists | `[tool_config]` refuses a missing or edited graph | `mise install --locked` by hand: `mise exec` stays loose |
| `UFBT_HOME` unset means `~/.ufbt` | `mise.toml` plus tracked `.env`, read first (`ufbt/__init__.py:59`) | `mise.toml` alone: a full-path start from the root escapes |
| 60 `.pyc` into the shared runtime and venv | `PYTHONPYCACHEPREFIX`: 0 outside, 447 inside | `PYTHONDONTWRITEBYTECODE`: recompiles SDK scripts each start |
| `ccache` on `PATH` used silently, `HOME` forwarded (`fbt/util.py:24`) | `CCACHE_DISABLE` reaches a stub | `CCACHE_DIR`: SCons does not forward it |
| Toolchain `bin/` holds `python3`, `pip`, `scons` | No `_.path` entry | Adding it for CLion: the database names the compiler absolutely |
| `ufbt vscode_dist` overwrites `.editorconfig`, `.gitignore` (`SConstruct:413`) | Never run; `ufbt lint` deploys `.clang-format` alone | None |
| `act` hands `gh auth token` to the job (`cmd/root.go:437`) | `--secret GITHUB_TOKEN=` | None |
| `act` mounts Docker's socket on network `host`, so a job reaches the VM's other stacks | `--container-daemon-socket -`, `--network bridge` | `./.actrc`: `cmd/actrc.go` refuses both flags |

Rosetta needs no closure:
`fbtenv.sh:151` selects `uname -m`,
and the `arm64-darwin` archive holds 0 x86_64-only Mach-O files.

- **Tracked symlink `compile_commands.json` → `.vscode/compile_commands.json`,**
  not CLion's Change Project Root:
  that stays in `.idea/`, and `SConstruct:279` hardcodes the path.
- **`ufbt` for the database, not `ufbt cdb`:**
  `cdb` leaves `f0_claude_remote_icons.h` ungenerated.
- **`ufbt update --branch`, not `--url`:**
  `--url` records version `unknown`.
- **Proof by `sandbox-exec` plus a throwaway `HOME`,**
  not `fs_usage` (root) or `find` over `$HOME` (noise):
  macOS 27 logs no denials,
  so the throwaway `HOME` catches what a swallowed denial hides.
- **Workflow references are full SHAs of `v7`,**
  not the template's `v4` tags:
  tags move, and `v4` declares the removed Node 20.
  It gains `permissions: contents: read` and `ufbt-version: '==0.2.6'`.
- **The upload step carries `if: ${{ !env.ACT }}`,**
  not `--artifact-server-path`:
  act's server rejects `upload-artifact@v7`'s `mime_type`.
- **Jobs run `linux/arm64`,**
  not the `act` skill's `linux/amd64`:
  colima has `rosetta: false`, and both jobs pass natively.

`setup` runs a probe write to the real `$HOME` that must fail;
then `ufbt update --branch "$FLIPPER_SDK_VERSION"` and `ufbt`, sandboxed with a throwaway `HOME`;
then asserts that `HOME` is empty and `ufbt status version` equals the pin.

```scheme
(version 1)(allow default)(deny file-write*)
(allow file-write* (subpath "<physical root>") (subpath "/private/tmp")
  (subpath "/private/var/folders") (subpath "/dev"))
```

`check-workflow` runs with a throwaway `HOME` and `DOCKER_HOST` defaulting to colima's socket:

```sh
act -W .github/workflows/build.yml --rm \
  -P ubuntu-latest=catthehacker/ubuntu:act-24.04@sha256:c58e2b36… \
  --container-architecture linux/arm64 --container-daemon-socket - \
  --network bridge --secret GITHUB_TOKEN= \
  --action-cache-path .act/actions --cache-server-path .act/cache
```

`clion` is Toolbox's `open -na` script:
a probe started that way inherited the environment but ran in `/`.
Navigation needs neither: the database holds absolute paths.
The current `.idea/` records a plain project, blocking the database import.

- `mise.toml` declares the pinned toolchain with its environment.
- `.env` points a bare `ufbt` at `.ufbt`.
- `.gitignore` ignores `.ufbt/`, `dist/`, `.vscode/`, `.act/`.
- `.clang-format` holds Flipper's format.
- `.mise/tasks/setup` deploys the pinned SDK under the sandbox; Darwin-only, as `sandbox-exec` is.
- `.mise/tasks/check-workflow` runs the workflow's jobs contained.
- `.github/workflows/build.yml` builds the app on GitHub for two SDK channels.
- `compile_commands.json` exposes ufbt's database at the root.
- `application.fam`, `f0_claude_remote.c`, `.png`, `images/.gitkeep` are `ufbt create`'s placeholders.

Testing: no business logic;
the seam is the process boundary,
and each spec scenario is a command or an observation.

## Risks / Trade-offs

- A first `ufbt` before `setup` deploys the latest release (`ufbt/__init__.py:85`) → `setup` replaces it and asserts the pin.
- A full-path `ufbt` from a subdirectory, or a full-path `ufbt-bootstrap`, reaches `~/.ufbt` → nothing documented does this.
- The first lock and every pin bump fail under the policy → drop `[tool_config]` for `mise install uv python@<version>` and `mise lock`; `README.md` says so.
- `sandbox-exec` is deprecated → it only proves; the probe write fails loudly if it stops denying.
- The default `sources` glob compiles every `*.c*` under the root into the app → the first change with logic sets `sources`.
- `clion .` resolves `.` from `/` → fall back to `clion "$PWD"`; task 5.3 records which works.
- The Flipper action fails on GitHub's Node 24 → the first push shows it; fork or inline it.

# Design

## Context

See `proposal.md` for why.
No app layer is touched.

Documented in mise-action's `action.yml`, mise's docs and the runner image's README:

- the action adds `--locked` when `mise.lock` exists, exports the project's env to later steps, and keeps the token to its own install;
- a pinned mise comes from its GitHub release alone, checked against signed checksums;
- mise does not check a restored tool cache against `mise.lock`;
- `mise exec` installs every missing tool the config selects, `mise run` a task's, and `MISE_AUTO_INSTALL=false` stops both;
- `pypi:ufbt`'s `python` option installs that runtime as the tool's own;
- GitHub's `ubuntu-24.04` carries shellcheck 0.9.0.

From sources alone, so a bump can break them:

- an empty `github_token` leaves mise unauthenticated, with a warning (`src/github-token.ts:19`);
- an empty `--branch` deploys the release channel (ufbt `bootstrap.py:464`);
- `ufbt lint` installs the SDK's `.clang-format` over the tracked one (`SConstruct:353`, `:416`);
- prek's `check-symlinks` fails a link whose target is missing (`check_symlinks.rs:30`);
- act ignores branch filters, and copies a symlink as a link (`planner.go:216`, `file_collector.go:202`);
- act's image builds on `buildpack-deps:24.04`, which holds gcc and libc6-dev (`build-ubuntu.yml:125`);
- GitHub's runner sets `vm.mmap_rnd_bits=28` for ASan (`configure-environment.sh:55`).

Assumed, and checked by task 1.1 before anything is written:
gcc 13 builds `test/` under the task's warnings, and ASan runs in act's arm64 container under colima.
`src/core/` already builds under the SDK's gcc 12 with the same warnings.

## Goals / Non-Goals

**Goals:** the spec's, alike on GitHub and under act.

**Non-Goals:** `workflow_call`, the release build, dependabot, the commit-message hooks,
and the Linux lock entries of tools no job installs.

## Decisions

| File | Its one task |
|---|---|
| `.github/workflows/ci.yml`, new | Runs the local checks on GitHub |
| `.github/workflows/build.yml` | Deleted |
| `.mise/tasks/check-workflow` | Runs `ci.yml`'s jobs through act, contained |
| `prek.toml` | `check-symlinks` gains `exclude = '^compile_commands\.json$'` and the proposal's reason as a comment |
| `CONTRIBUTING.md` | The `check-workflow` bullet names `ci.yml`; a `## CI` section follows `## Hooks` |

`ci.yml`:

```yaml
name: ci

on:
  push:
    branches: [develop, main]
  pull_request:

permissions:
  contents: read

env:
  # mise exec and mise run would install every tool a job leaves out, without the token.
  MISE_AUTO_INSTALL: "false"

jobs:
  test:
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@3d3c42e5aac5ba805825da76410c181273ba90b1 # v7.0.1
        with:
          persist-credentials: false
      - uses: jdx/mise-action@f5be966e4a917d54244728e6a5fdcd1d93a35e6c # v5.2.6
        with:
          version: 2026.11.2
          # mise does not check a restored tool cache against mise.lock.
          cache: false
          # The suites need only the runner's cc.
          install: false
      - run: mise run test

  build:
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@3d3c42e5aac5ba805825da76410c181273ba90b1 # v7.0.1
        with:
          persist-credentials: false
      - uses: jdx/mise-action@f5be966e4a917d54244728e6a5fdcd1d93a35e6c # v5.2.6
        with:
          version: 2026.11.2
          cache: false
          install_args: uv pypi:ufbt
      # setup's deploy without its macOS-only sandbox, the pin read inside mise: an empty --branch
      # deploys the release channel.
      - run: mise exec -- sh -c 'ufbt update --branch "$FLIPPER_SDK_VERSION"'
      - run: mise run build
      - run: mise exec -- ufbt lint

  hygiene:
    runs-on: ubuntu-24.04
    steps:
      - uses: actions/checkout@3d3c42e5aac5ba805825da76410c181273ba90b1 # v7.0.1
        with:
          persist-credentials: false
      - uses: jdx/mise-action@f5be966e4a917d54244728e6a5fdcd1d93a35e6c # v5.2.6
        with:
          version: 2026.11.2
          cache: false
          install_args: actionlint aqua:j178/prek
      - run: mise exec -- prek run --all-files
      # The runner's shellcheck is unpinned, and act's image has none.
      - run: mise exec -- actionlint -shellcheck= -pyflakes=
```

`check-workflow` keeps its comments and environment;
its description reads `Run ci.yml's jobs through act, contained`, and its command:

```sh
act -W .github/workflows/ci.yml --rm \
    -P ubuntu-24.04=catthehacker/ubuntu:act-24.04@sha256:c58e2b36… \
    --container-architecture linux/arm64 --container-daemon-socket - \
    --network bridge --secret GITHUB_TOKEN= \
    --action-cache-path .act/actions --no-cache-server
```

- **`MISE_AUTO_INSTALL: "false"`,**
  not `install_args` alone:
  the first `mise exec` would install every tool the job left out.
- **The SDK step reads the pin inside `mise exec`,**
  not from the env the action exports:
  an export gone missing would leave `--branch` empty.
- **No `sha256` input:**
  it pins one platform's binary, while act runs arm64 and GitHub x64;
  the signed packslip checks both.
- **`--no-cache-server`,**
  not `--cache-server-path .act/cache`:
  nothing restores a cache now.

`## CI` in `CONTRIBUTING.md` tables each job's steps as `ci.yml` runs them, with the triggers,
and says that `mise run setup` stands in for the SDK step on the Mac and that the runner image stays unpinned.

**Test seam.**
The process boundary: each scenario is a read of a file, a command, or a line of an act or GitHub log.

## Risks / Trade-offs

- gcc rejects `test/`, or ASan dies in act's container → task 1.1 runs first; stop and raise it, `if: ${{ !env.ACT }}` on `test` being the fallback for ASan.
- colima's VM times out to Cloudflare hosts, `update.flipperzero.one` among them → rerun `check-workflow`.
- act installs with no token, under GitHub's 60 requests an hour → rerun after the hour.
- The runner image moves weekly, gcc with it → the one unpinned input, as the config accepts for `cc`.
- The local mise moves past `2026.11.2` → bump `version` by hand; dependabot moves action SHAs only.
- Each `build` run downloads toolchain 39, about 297 MB → the cost of no cache, accepted.
- `ufbt lint` rewrites `.clang-format` in `build`'s tree → nothing reads the tree after it.
- The Mac's `shellcheck` is a mise shim with no version, failing plain `actionlint` on each `run:` step → use the job's flags.

# Proposal

## Why

No script in the repository is linted.
The 6 tasks under `.mise/tasks/`, 94 lines of `#!/bin/sh`, meet only prek's builtin hooks,
and `hygiene` runs `actionlint -shellcheck= -pyflakes=`, so `ci.yml`'s 6 `run` steps skip shellcheck as well.
The `ci` change turned it off on 2026-11-06:
GitHub's runner carries an unpinned 0.9.0, act's image none, and the Mac a mise shim with no version.
`release` brings a 52-line task that tags and pushes, where a published tag is final,
and `release.yml`'s 6 `run` steps.

Settled by the owner on 2026-11-07:

- **Everything the project scripts passes shellcheck:**
  shell scripts through a prek hook at `pre-commit`, workflow `run` steps through actionlint in `hygiene`.
- **shellcheck 0.11.0, pinned in `mise.toml` as `aqua:koalaman/shellcheck` with no comment,**
  not a fresh audit nor itte's comment block:
  itte's audit of 2026-10-21 holds, as the lock's digests equal itte's.
- **A local prek hook over `types = ["shell"]`,**
  not itte's `files = '^\.mise/tasks/'`:
  a script anywhere is caught, and prek reads an extensionless task's shebang.
- **`hygiene` installs shellcheck and drops `-shellcheck=`,**
  so one pinned version lints on the Mac, under act and on GitHub;
  `-pyflakes=` stays, as no step runs Python.
- **SC2016 on the SDK deploy step takes a directive,**
  not a reworded command:
  the single quotes are meant, as the pin is read inside mise.
- **A change of its own, landing before `release`,**
  cut from that plan for its budgets:
  `release`'s task and `release.yml` meet its checks as they are written.

Proposed here, for the owner to strike:

- **Change id `shellcheck`; capability `ci` modified,**
  not a new one:
  `hygiene` runs both paths, and `ci`'s jobs requirement already names the hooks and the workflow linter.
- **A finding that is meant takes a directive at its line, never a `.shellcheckrc`:**
  actionlint runs shellcheck with `--norc`, so an rc file would reach the hook alone.

## What Changes

- `mise.toml` pins `aqua:koalaman/shellcheck` 0.11.0; `mise.lock` gains its entries, written by mise.
- `prek.toml` gains a local `shellcheck` hook, and its header names local hooks beside builtin ones.
- `ci.yml`: `hygiene` installs shellcheck and runs `actionlint -pyflakes=`;
  the SDK deploy step carries `# shellcheck disable=SC2016`.
- `CONTRIBUTING.md`: `## Hooks` names the hook; `## CI`'s `hygiene` row drops `-shellcheck=`.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `ci`: `hygiene`'s hooks and workflow linter run shellcheck, and a script with a finding fails them.

## Impact

Audit: itte's of 2026-10-21, reused,
rechecked on 2026-11-07 against GitHub, OSV and GHSA.
The owner dropped Socket, and Snyk was not run, as in itte.

| Dependency | Version | License | Source | Known vulnerabilities | Transitive artifacts |
|---|---|---|---|---|---|
| shellcheck | 0.11.0, 2025-08-04, still the latest | GPL-3.0 | aqua, GitHub release built by its own CI; sha256 equal to GitHub's digests and itte's lock | 0 in ShellCheck; 4 in its Hackage dependencies, below | None: a static binary, shipped with its license and readme |

Findings:

- **aeson (HSEC-2023-0001, HSEC-2026-0007) and base (HSEC-2023-0007, HSEC-2024-0006)**
  carry denial-of-service advisories on untrusted JSON and numbers,
  which a lint of our own scripts never reads.
  No network package is linked in, and offline, with no host mount, it lints as online (itte's stage 4).
- **`mise.lock` records no provenance:**
  its sha256 comes from `mise-versions`,
  and each equals the digest GitHub lists for the `.tar.xz` or `.zip` its CI uploaded on 2025-08-04.
  The `.tar.gz` copies koalaman added on 2026-01-05 are not used.

Beyond the code:

- `docs`'s plan still reads `-shellcheck=` in its `ci` delta and tasks 3.1 and 5.1,
  and its delta replaces the jobs requirement whole, which would undo this change's:
  `/opsx:update docs` in a clean session brings it in line before `docs` applies.
- `release`'s task 2.1 already runs the hook.
- Every script added later meets the hook, `docs`'s tasks included.

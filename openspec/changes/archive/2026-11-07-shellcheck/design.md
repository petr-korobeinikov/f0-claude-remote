# Design

## Context

See `proposal.md` for why.
No app layer is touched.

| Fact | Where |
|---|---|
| prek reads an executable's shebang: the 6 tasks, and `release`'s, tag as `executable, file, sh, shell, text` | `prek util identify` |
| actionlint runs `shellcheck` from `PATH` when it finds one, else skips it silently; `-shellcheck=` turns it off | actionlint's `docs/checks.md` |
| actionlint passes `--norc` and excludes SC1091, SC2194, SC2050, SC2153, SC2154, SC2157 and SC2043 | `rule_shellcheck.go:193`, v1.7.12 |
| Under `mise exec`, `shellcheck` resolves to the pinned install ahead of the global shim | seen on the Mac |
| shellcheck's default severity is `style`, so any finding exits 1 | its manual |
| A directive applies to the command below it | shellcheck's wiki, Directive |

Assumed from the note `release`'s implementation left on 2026-11-07, and checked by task 2.3:
the 6 tasks pass shellcheck as written,
and actionlint flags only SC2016, on the SDK deploy step of `ci.yml` and `release.yml`.
A finding in a task is raised, not fixed in passing.

## Goals / Non-Goals

**Goals:** the spec's, alike on the Mac, under act and on GitHub.

**Non-Goals:** an actionlint hook, held off by the owner on 2026-11-06 with hooks for the other CI checks;
`release.yml`'s directive, which `release` writes;
`docs`'s plan, which `/opsx:update docs` revises.

## Decisions

| File | Change |
|---|---|
| `mise.toml` | `"aqua:koalaman/shellcheck" = "0.11.0"`, after prek as `[tools]`' order puts it |
| `mise.lock` | Its 7 platform entries, as mise wrote them |
| `prek.toml` | A `repo = "local"` block between the builtin hygiene hooks and the message gates; the header names local hooks |
| `.github/workflows/ci.yml` | `hygiene`'s `install_args` gains `aqua:koalaman/shellcheck`, and actionlint loses `-shellcheck=`; the SDK deploy step becomes a block scalar holding the directive |
| `CONTRIBUTING.md` | `## Hooks` names the hook and actionlint's part; `## CI`'s `hygiene` row reads `actionlint -pyflakes=` |

The hook, under a comment that names its reach:

```toml
{ id = "shellcheck", name = "shellcheck", entry = "mise exec -- shellcheck", language = "system", types = ["shell"] },
```

- **`entry = "mise exec -- shellcheck"`,**
  not a bare `shellcheck`:
  a commit from CLion or a shell without mise activated would find no binary, or the global shim, which names no version.
- **The deploy step's directive sits in a block scalar,**
  not in a reworded command:
  a directive needs a line above its command,
  and the command stays as `CONTRIBUTING.md`'s `## CI` prints it.

**Splitting the tree.**
The code already sits in the working tree, beside `release`'s uncommitted work.
This change's hunks:

| Path | Hunks |
|---|---|
| `mise.toml`, `mise.lock`, `prek.toml` | All |
| `.github/workflows/ci.yml` | All but `workflow_call:` |
| `CONTRIBUTING.md` | `## Hooks`' two and `## CI`'s `hygiene` row |

- **`release`'s work waits in a stash,**
  not beside this change's hunks staged:
  the tree then shows the owner the commit alone in CLion,
  and no branch move meets a dirty file.
  On `feature/shellcheck`, `git stash push --include-untracked` takes the tree's changes,
  leaving the ignored `.ufbt/` and `dist/` in place,
  and this change's hunks come back from `stash@{0}`.
  After `git flow feature finish`,
  `feature/release` fast-forwards to `develop`
  and `git stash pop` returns `release`'s work.

**Test seam.**
The process boundary: each scenario is a command over a probe file, a read of a file or a line of an act log.

## Risks / Trade-offs

- A `run` step's finding fails only `hygiene`, as actionlint has no hook → `## CI` lists it among the commands to run before pushing.
- A zsh script is tagged `shell`, and shellcheck refuses zsh (SC1071) → `exclude_types = ["zsh"]` on the hook once one lands.
- The pop conflicts on a file → the stash stays: `git checkout 'stash@{0}' -- <file>`, `git reset`, then `git stash drop`.
- A job under act times out reaching a host from colima's VM → rerun `check-workflow`.

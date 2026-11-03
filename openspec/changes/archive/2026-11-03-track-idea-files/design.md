# Design

## Context

Tooling only: no core, no UI, no platform code.
The seam is `git` and the files CLion leaves behind, so each spec scenario is a command or an observation.

`.gitignore` has `.idea/` today.
Git cannot re-include a file under an excluded directory, so that line has to change.

Observed in the working checkout, after CLion sessions:

| File | Against `ide/` | Reading |
|---|---|---|
| `.idea/debug-profiles.xml` | identical, final newline kept | CLion did not rewrite it |
| `.idea/misc.xml` | same content, final newline gone | CLion rewrote it at least once, with no change in content |

## Decisions

- **Allow-list in the root `.gitignore`** (`.idea/*`, then `!.idea/misc.xml`, `!.idea/debug-profiles.xml`),
  not `.idea/.gitignore`:
  CLion generates that file, and tracking it adds a third path with no first-open need.
- **`git mv -f ide/<file> .idea/<file>`,**
  not delete and add:
  history follows the files, and `-f` replaces the local copies, whose only difference is the newline.
- **Two files, by evidence:**
  `debug-and-ide-tasks` tasks 4.1-4.3 passed from exactly these two.
  CLion creates the rest of `.idea/` itself on open.
- **Measure the newline on a clean clone,**
  not reason about it from CLion's save path:
  the proposal's unmade decision turns on what CLion does, and the previous plan's facts came from observing it.

## Risks / Trade-offs

- CLion strips `misc.xml`'s newline on open → task 2.2 stops; the owner picks from the table below.
- CLion adds a component to `misc.xml` on an update → a real diff in `git status`, reviewed like any other; the old copy hid it.
- A clone opened before `mise run setup` links a database that does not exist yet → `README.md` keeps the order; `setup` still comes before `clion .`.

Closures if task 2.2 finds the newline stripped, never a hook exclusion:

| Closure | Cost | Limit |
|---|---|---|
| `git checkout -- .idea/misc.xml` after the first open | one manual step per clone, in `README.md` | holds only if CLion does not strip again |
| Clean filter restoring the newline: `.gitattributes`, plus `git config` in `setup` | one tracked file and one line in `setup` | a clone without `setup` shows the diff |
| Leave `misc.xml` out, track `debug-profiles.xml` alone | none | breaks the import: `.idea/` then exists with no database link |

## Files

- `.gitignore` names what stays out of the repository.
- `.mise/tasks/setup` loses `#MISE depends=["setup-ide"]`.
- `.mise/tasks/setup-ide` is deleted.
- `README.md` loses the `setup-ide` bullet and gains one sentence on the tracked files.

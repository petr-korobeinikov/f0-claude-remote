# Proposal

## Why

The two IDE files a fresh clone needs reach `.idea/` by way of a copy:
`ide/misc.xml` and `ide/debug-profiles.xml`, `.mise/tasks/setup-ide`, and a dependency in `setup`.
That is four paths and a task for what `git clone` delivers on its own.
The owner chose on 2026-11-03 to track those two files in place and drop the copy,
reversing the `debug-and-ide-tasks` decision to keep `.idea/` entirely ignored.

Decisions settled with the owner on 2026-11-03:

- **Only what a fresh clone needs is tracked,**
  not all of `.idea/`:
  everything else there stays ignored.
- **`ide/`, `setup-ide` and `setup`'s dependency go,**
  not kept beside the tracked files:
  a second copy would only drift.

Decisions proposed here, for the owner to strike:

- **The tracked set is `misc.xml` and `debug-profiles.xml`,**
  not also `vcs.xml`, `editor.xml` or `codeStyles/`:
  the `debug-and-ide-tasks` clean-clone acceptance passed with these two alone.
- **`.idea/misc.xml` is tracked with exactly one final newline,**
  not as CLion writes it:
  the repository's rule has no exemptions.

Unmade, and measured before the change can finish:
whether CLion strips that newline on a fresh clone.
The local copy differs from `ide/misc.xml` only by the missing newline, so CLion rewrote it at least once.
How often it does so is unknown, and a repeat breaks the clean worktree this change promises.
If it repeats, apply stops at the measuring task and the owner picks a closure; the design lists the candidates.

## What Changes

- `.gitignore` ignores `.idea/*` except the two tracked files.
- `ide/misc.xml` and `ide/debug-profiles.xml` move to `.idea/`.
- `.mise/tasks/setup-ide` and `setup`'s `depends` line are removed.
- `README.md` drops the `setup-ide` bullet and says which IDE files are tracked.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `ide-run-configurations`: IDE files arrive by clone instead of by `setup`, and only they are tracked under `.idea/`.

## Impact

`.gitignore`, `.idea/`, `ide/`, `.mise/tasks/setup`, `.mise/tasks/setup-ide`, `README.md`.
`ide-navigation` is unchanged: its scenarios hold for the same two files, however they arrive.
A checkout with an edited `.idea/misc.xml` loses the edit when the tracked file lands; this one differs only by the newline.

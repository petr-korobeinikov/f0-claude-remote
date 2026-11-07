# Proposal

## Why

After `docs`, the site guides users, while a developer's knowledge stays in two files read on GitHub:
`CONTRIBUTING.md`, 157 lines of toolchain, workflow, release, hooks and CI,
and the README's `## Debugging`, 87 lines of the probe and a session on the device.
`llms.txt` lists none of it, and the README keeps a section only a developer reads.

Settled by the owner on 2026-11-06:

- **The tree's Development and Contributing, as the second of three changes;**
  the README's Debugging and all of `CONTRIBUTING.md` move there,
  and `CONTRIBUTING.md` becomes a pointer, as in `claude-shim`.

Proposed here, for the owner to strike:

- **Pages in `docs/development/` and `docs/contributing/`:**
  `ufbt lint` warns on a hyphen in any folder it walks.
- **Lines move whole:** a page orders and heads them, but keeps each line,
  so a line search proves the move; a line changes only where a link or a heading must.
  Additions come from specs and archived changes alone.
- **CLion holds the setup, Debugging the session:**
  the README's six CLion steps stay beside the terminal session.
- **The README keeps its head, Why, Features, Installation, Contributing and License;**
  its head and Contributing link the site.
- **`openspec/config.yaml`'s Housekeeping names `docs/` beside `README.md`:**
  the statements a housekeeping pass checks now live there too.
- **After `docs`:** this change needs its site and edits its `docs-site` and `user-guide`.
  The Release page moves `## Release` as `release` leaves it.

## What Changes

- New: `toolchain.md`, `build-and-run.md`, `host-tests.md`, `clion.md`, `debug-probe.md` and `debugging.md` in `docs/development/`;
  `workflow.md`, `hooks-and-ci.md` and `release.md` in `docs/contributing/`.
- `CONTRIBUTING.md` becomes a pointer, the README loses Debugging,
  and `zensical.toml` and `openspec/config.yaml` follow.

## Capabilities

### New Capabilities

- `developer-guide`: the Development pages, from a clone to a breakpoint on the device.
- `contributor-guide`: the Contributing pages, and the pointer `CONTRIBUTING.md` becomes.

### Modified Capabilities

- `docs-site`: the sidebar adds Development and Contributing.
- `user-guide`: the README drops Debugging too.
- `device-debug`: the debugging steps live on the site.

## Impact

No dependency: `docs`'s build and deploy carry the pages.

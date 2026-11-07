# Design

## Context

See `proposal.md` for why.
No app layer is touched, nor the site's build, which `docs` set up.

Where each page's lines come from:

| Page | Moved from | Added from |
|---|---|---|
| Toolchain | `CONTRIBUTING.md`'s opening; `## Toolchain`'s mise, setup, bump and `vscode_dist` lines | `toolchain-isolation`, `app-build` |
| Build and run | `## Toolchain`'s build, launch, lint and `ufbt flash` lines | `app-build`, `app-screen` |
| Host tests | `## Toolchain`'s test and `cc` lines | `host-tests`, `source-layers` |
| CLion | `## Toolchain`'s `clion .` and `.idea` lines | `ide-navigation`, `ide-run-configurations` |
| Debug probe | README's `### The probe`, and the board's risks from `### Before a session` | `device-debug` |
| Debugging | README's `### Before a session`, `### In CLion`, `### In a terminal` | `device-debug` |
| Workflow | `## Workflow` | the commit rules of `openspec/config.yaml` |
| Hooks and CI | `## Hooks`, `## CI` | `ci` |
| Release | `## Release` | `release`, `changelog` |

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** Internals; any change to the tools, tasks or workflows the pages describe.

## Decisions

| File | Its one task |
|---|---|
| `docs/development/*.md`, `docs/contributing/*.md` | Each meets its spec's requirement |
| `zensical.toml` | Lists the new pages in `nav` and in `llmstxt` sections |
| `CONTRIBUTING.md` | Points at the site |
| `README.md` | Gives up Debugging |
| `openspec/config.yaml` | Names `docs/` in Housekeeping |

**`nav`:** Home, Quick start, Guide, Development, Contributing; `llmstxt` gains sections `Development` and `Contributing`.
**The README's head:** Documentation, Quick start, Debugging, each a link into the site.
**`CONTRIBUTING.md`:** one paragraph and a link to Workflow, as `claude-shim`'s;
GitHub keeps linking the file from issues and pull requests.

- **A page orders the lines it takes, never rewords them:**
  the line search then proves the move, and a reworded line would hide a lost fact.
  A line that reads oddly out of its section gets a lead-in line of its own.
- **Debugging keeps CLion's six steps:** they are a session, as the terminal's are;
  the CLion page stops where a session starts.
- **Commit rules stay in `openspec/config.yaml`:** Workflow names the format the hooks enforce and links the file,
  which agents read, rather than copying it.

**Test seam:** the build fails a dead link, the moved pages' links included;
a script takes each line of the moved sections from `git show HEAD:<file>` and searches the pages with `grep -F`.

## Risks / Trade-offs

- `docs` lands changed → task 1.1 rebases this change's deltas on the archived specs.
- A link points at `CONTRIBUTING.md#…` or the README's `#debugging` → task 3.2 finds and repoints it.
- `release` rewrites `## Release` in review → the page takes it as found when this applies.

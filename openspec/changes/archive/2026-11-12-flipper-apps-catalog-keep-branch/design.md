# Design

## Context

See `proposal.md`, Why.
No app layer is touched: the change lands in the `flipper-apps-catalog` job's step and the page describing it.

| Fact | Where |
|---|---|
| `GET /repos/{owner}/{repo}/pulls` filters by `head`, `user:ref-name`, and by `state`, `open` by default | GitHub REST docs, "List pull requests" |
| It takes installation access tokens, and no permission or authentication "if only public resources are requested"; upstream is public | Same section; `gh api repos/flipperdevices/flipper-application-catalog` |
| `POST` to the same path creates a pull request | Same docs, "Create a pull request" |
| `gh api` switches to `POST` once a `-f` or `-F` field is added | `gh api --help`, 2.103.3 |
| `GITHUB_TOKEN` is an installation access token, limited to its repository; the job's holds `Contents: read` and `Metadata: read` | GitHub docs, `GITHUB_TOKEN`; the `v0.2` run's log |
| The runner runs the step as `/usr/bin/bash -e {0}` | The `v0.2` run's log |
| `petr-korobeinikov/f0_claude_remote_0.1` (`76a552a`) and `petr-korobeinikov/f0_claude_remote_0.2` (`e649b9f`) are one commit each on `a1f974c`, both adding the manifest | The fork's commits |
| `docs/contributing/release.md:42` is the one statement on the fork's old branches | `rg -i -e fork -e deleted` over `README.md`, `SECURITY.md`, `CONTRIBUTING.md` and `docs/` |
| macOS's `base64` reads a file through `-i` alone: `base64 -w 0 FILE` exits 64 | `base64 --help` and a run on this Mac |
| `mise run setup` runs under `sandbox-exec` once it has proved the sandbox denies | `.mise/tasks/setup:15-25` |
| The owner's `gh` sits under `~/.local/share/mise/installs/`, off `/usr/bin` and `/bin` | `command -v gh` |

Assumed, and checked by task 1.1: `head` matches a branch name holding a slash; the docs' examples hold none.
Assumed, and checked by task 1.3: `sandbox-exec` with `(deny network*)` refuses every connection.

## Goals / Non-Goals

**Goals:** the spec's.

**Non-Goals:** a new version carried on a kept branch;
a branch cut beside an open pull request rebased once that one merges (Risks).

## Decisions

| File | Its one task | What changes |
|---|---|---|
| `.github/workflows/release.yml` | Publishes the version its tag names | The `flipper-apps-catalog` step, lines 66-84 |
| `docs/contributing/release.md` | Tells a contributor how a release is cut | Line 42 |

Of the step's six steps (archived `flipper-apps-catalog-submission`, `design.md:45-50`), the second changes:

1. `upstream=flipperdevices/flipper-application-catalog` opens the variables, above `fork`.
2. The loop reads, for each `ref`,
   `open=$(GH_TOKEN=$REPOSITORY_TOKEN gh api "repos/$upstream/pulls?state=open&head=$GITHUB_REPOSITORY_OWNER:${ref#refs/heads/}" --jq length)`,
   then DELETEs it under `if [ "$open" -eq 0 ]`.
3. `# The fork's token stays with the fork.` moves from line 83 to that read, now the step's first with `REPOSITORY_TOKEN`;
   a comment above the `if` says deleting the head of an open pull request closes it.

- **The read's parameters stay in its URL,** never in `-f` or `-F`:
  a field turns `gh api` into a `POST`, and a `POST` to `pulls` opens a pull request.
- **`github.token` reads upstream,** not the fork's token:
  the fork's token stays with the fork, as for `commit_sha` (archived `design.md:53-54`).
- **The count assigned before the test,** as the refs are:
  `set -e` ignores a read that fails inside `[ ]`, and the job would go on as if the branch were open.

`docs/contributing/release.md:42`, `the previous version's branch deleted,`, becomes
`each earlier version's branch deleted unless a pull request is open from it,`.

**Test seam.**
actionlint and its shellcheck read the step.
Two checks run it, and neither holds a GitHub credential:

1. **The live read** (task 1.1): `/usr/bin/curl -q -fsS`, with no token, method or body, GETs the public answers.
2. **The dry run** (tasks 1.3-1.5): the step's script, run in a `mktemp -d` folder
   under `sandbox-exec -p '(version 1)(allow default)(deny network*)'`, `env -i` and `/bin/bash -e`,
   its `PATH` that folder and `/usr/bin:/bin`.
   The folder's `gh` is a fake: it logs each call with its `GH_TOKEN`,
   answers the refs and counts with task 1.1's, the other reads with fixed values, and does nothing else.
   The folder's `base64` turns `-w 0 FILE` into `-i FILE`;
   `GH_TOKEN` and `REPOSITORY_TOKEN` hold the labels `fork-token` and `repository-token`.

- **Anonymous `curl` for the live read,** not the owner's `gh`, which holds write access and would `POST` on a stray `-f`.
- **A fake `gh` on a bare `PATH`,** not the real one behind a filter that drops writes:
  a slip in the filter would send the DELETE that closes #1387.
- **The sandbox proved before the dry run,** as `mise run setup` proves its own:
  a guard that stopped denying would let the dry run pass unseen.

| Scenario | Closed by |
|---|---|
| `First submission` | `v0.1`'s run |
| `Manifest` | The `manifest.yml` the dry run writes (task 1.4) |
| `Open pull request` | Tasks 1.1 and 1.4 |
| `Next submission` | The dry run with the manifest on upstream `main` (task 1.5) |

## Risks / Trade-offs

- A release while a pull request is open cuts its branch from a `main` lacking that one's manifest, as with `petr-korobeinikov/f0_claude_remote_0.2`
  → the branch conflicts once that one merges; the first release after the merge cuts an `Update`.

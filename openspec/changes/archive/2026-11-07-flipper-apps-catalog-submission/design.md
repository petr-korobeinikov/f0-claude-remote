# Design

## Context

See `proposal.md` for why.
No app layer is touched.

| Fact | Where |
|---|---|
| Environment secrets reach only jobs naming the environment, once its tag policy passes | GitHub docs |
| A Release made with `GITHUB_TOKEN` starts no workflow | GitHub docs |
| Fork sync, refs and contents take Contents write, a new ref also Workflows; `gh repo sync` fast-forwards | GitHub docs, gh manual |
| `matching-refs` lists each `ref` whole, `refs/heads/feature-a`; `DELETE git/refs/{ref}` takes `heads/feature-a` | GitHub's OpenAPI description, `git-ref-items`, `git-ref-only` |
| `commits/{ref}` takes a commit, `heads/<branch>` or `tags/<tag>`; a `contents` update needs the file's blob `sha` | GitHub docs |
| On an error `gh api` exits 1 and prints the body to stdout, skipping `--jq` | gh 2.103.3, `pkg/cmd/api/api.go:509`, `:546` |
| `@path` reads the source at `commit_sha`; `application.fam` supplies the other fields | catalog's `Manifest.md` |

Assumed, and checked by task 2.2 against the fork: `matching-refs` answers an empty match with `[]`.

The six steps below restate the step the owner reviewed on 2026-11-07, never committed;
steps 2 and 6 fix two of its lines:
its delete named `git/${ref#refs/}`, no endpoint,
and its manifest read took the 404 body gh prints for the blob `sha`.

## Goals / Non-Goals

**Goals:** the spec's.

**Non-Goals:** opening the pull request and its AI disclosure, both the owner's;
a catalog link in the README or on the site, until the listing exists.

## Decisions

| File | Its one task |
|---|---|
| `.github/workflows/release.yml` | Gains the `flipper-apps-catalog` job, which prepares the fork's branch |
| `CONTRIBUTING.md` | `## Release` documents the job, its setup included |

**The `flipper-apps-catalog` job:** `needs: release`, `runs-on: ubuntu-24.04`, `environment: flipper-apps-catalog`,
no `permissions` of its own and no checkout;
one step under `set -eu`, `GH_TOKEN` from `secrets.FLIPPER_APPS_CATALOG_TOKEN`, `github.token` in a second variable.
With `fork=$GITHUB_REPOSITORY_OWNER/flipper-application-catalog`,
`prefix=$GITHUB_REPOSITORY_OWNER/f0_claude_remote_` and `name=${GITHUB_REF_NAME#v}`, it:

1. Runs `gh repo sync "$fork" --branch main` and reads that `main`'s commit.
2. Lists `git/matching-refs/heads/$prefix` into a variable, then DELETEs `repos/$fork/git/$ref` for each `ref`, a rerun's own included.
3. Creates `refs/heads/$prefix$name` at that commit.
4. Reads `commit_sha` from `repos/$GITHUB_REPOSITORY/commits/tags/$GITHUB_REF_NAME` with `github.token`.
5. Writes `manifest.yml` through a heredoc, `Manifest`'s fields in that order, `origin` as `$GITHUB_SERVER_URL/$GITHUB_REPOSITORY.git`.
6. Reads the manifest's blob `sha` on `main`'s commit; PUTs the file to the branch as `Update Claude Remote to $name` with it when the read succeeds, else as `Add Claude Remote $name`.

- **A job of `release.yml`,** not a workflow on `release: published`, which a Release made with `GITHUB_TOKEN` never starts.
- **`commit_sha` from the commits API,** not `GITHUB_SHA`, which the docs never say is peeled from an annotated tag;
  read with `github.token`, so the fork's token reaches the fork alone.
- **The refs listed into a variable first,** not `for … in $(…)`:
  `set -e` ignores a failed substitution in a `for` list, which then walks the error's body.
- **Step 6 judged by the read's exit status,** not by empty output: a 404 prints its body.

**`CONTRIBUTING.md`:** in `## Release`, the sentence on what the tag runs ends on the `flipper-apps-catalog` job:
the fork's `main` synced, the old branch deleted, `<owner>/f0_claude_remote_0.2` cut from `main` with the manifest,
its pull request opened by hand.
The paragraph on a failed run gains: a failed `flipper-apps-catalog` job is rerun alone, the Release kept.
After the notes block, ``The `flipper-apps-catalog` job needs the owner's setup, once:`` and the three steps below,
with the token's yearly revocation.

**The owner's setup,** once, before this change applies:

1. Fork `flipperdevices/flipper-application-catalog` into the owner's account, keeping its name.
2. Create a fine-grained token with access to the fork alone, Contents and Workflows read and write, and no expiry.
3. In this repository's Settings → Environments, create `flipper-apps-catalog`,
   limit its deployment branches and tags to one tag rule, `v*`,
   and give it the secret `FLIPPER_APPS_CATALOG_TOKEN` holding the token.

**Test seam.**
The step first runs at `v0.1`: it needs GitHub's environment, and no task writes to the fork.
Before that, actionlint's shellcheck reads it,
and its two reads, the refs and the manifest's, run against the fork by the owner's `gh` as the step makes them (task 2.2).
`First submission` and `Manifest` are read at `v0.1`, `Next submission` at `v0.2`.

## Risks / Trade-offs

- The step fails after the Release → rerun the job, or commit the file in the fork by hand.
- `v0.2` lands while `0.1`'s pull request waits → deleting `_0.1` closes it; `_0.2` adds the manifest afresh, so one pull request carries `0.2`.
- The fork's `main` diverges from upstream → `gh repo sync` fails on the conflict; `--force` resets it, then rerun.
- A reviewer rejects the screenshot as not qFlipper's → it bundles to the same 128×64 picture; recoloring it costs a version.
- A token unused for a year is revoked → a new one before the next release.

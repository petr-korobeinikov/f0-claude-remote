# Design

## Context

See `proposal.md` for why.
No app layer is touched.

| Fact | Where |
|---|---|
| GitHub recommends drafting a Release, attaching its assets, then publishing it; publishing locks the tag and the assets, while the title and notes stay editable | GitHub docs, immutable releases |
| Immutability takes effect on publish; a draft and its assets stay editable | gh 2.103.3 manual, `gh release create` |
| `gh release edit <tag> --draft=false` publishes a draft | gh manual |
| A job whose need failed or was skipped is skipped, down the whole chain; rerunning failed jobs reruns the jobs that need them | GitHub docs, `needs`, re-running jobs |
| Under `--draft`, `gh release create` neither refuses a tag's published Release nor deletes a draft whose upload failed | gh 2.103.3, `create.go:501-541` |
| `gh release view`, `edit` and `delete` find a draft by its tag through one GraphQL lookup, and stop there when no Release is found | gh 2.103.3, `fetch.go:183-262`, `delete.go:78-80` |

The last two rows are sources alone: a bump of the runner's `gh` can break them.

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** an approval before `publish`; any change to the `flipper-apps-catalog` job's step.

## Decisions

| File | Change |
|---|---|
| `.github/workflows/release.yml` | The jobs below |
| `docs/contributing/release.md` | What the tag runs, and how a failed run is undone |

`release.yml`'s jobs, each needing the one above it:

| Job | Change |
|---|---|
| `ci` | None |
| `draft`, the `release` job renamed | Its last step opens with the guard: when `gh release view "$GITHUB_REF_NAME"` exits 0, an `::error::` naming the tag, and exit 1. `gh release create` follows as before, with `--draft` |
| `flipper-apps-catalog` | `needs: draft` |
| `publish`, new | `runs-on: ubuntu-24.04`, `permissions: contents: write`, no checkout; one step, `gh release edit "$GITHUB_REF_NAME" --repo "$GITHUB_REPOSITORY" --draft=false`, `GH_TOKEN` from `github.token` |

A comment above the guard says `--draft` turns off `gh`'s own refusal and cleanup;
one above `publish` says it runs last because publishing locks the tag.

- **`flipper-apps-catalog` stays after `draft`,** not beside it, though it no longer waits on a published Release:
  the fork is written only once the build and the draft have passed.
- **`publish` needs `flipper-apps-catalog` alone,** not all three jobs: a skip carries down the chain.
- **`publish` by tag,** not by the draft's id handed over from `draft`: the guard leaves one Release per tag.
- **The undo is three commands, the draft, the remote tag, then the local one,**
  not `gh release delete --cleanup-tag`, which deletes no tag when the run made no draft.

**`docs/contributing/release.md`.**
In the sentence on what the tag runs, the Release is drafted, not published,
and after the `flipper-apps-catalog` job comes "and last the `publish` job, which publishes the draft".
The paragraph on a failed run becomes:
until `publish`, the Release is a draft and the tag is not locked;
a failed run is undone, its cause fixed, and the same name cut again, the undo being

```shell
gh release delete v0.2 --yes
git push --delete origin v0.2
git tag --delete v0.2
```

whose first line finds nothing when the run made no draft.
A job that failed on GitHub's side alone is rerun with those after it,
a draft left by a failed `draft` deleted first with that first line.
The sentence on a published Release stays.

**Test seam.**
actionlint reads the jobs, and its shellcheck the guard.
The guard's `if` runs by hand through the owner's `gh`, read-only (task 1.2):
here at `v0.1`, which has no Release, and with `GH_REPO=cli/cli` at `v2.103.3`, which has one.
A draft and `publish` first run at `v0.1`.

## Risks / Trade-offs

- `publish` fails → the Release stays a draft: rerun the job, or run its command by hand.
- The runner's `gh` stops finding drafts by tag → `publish` fails as above; publish the draft from the Releases page.

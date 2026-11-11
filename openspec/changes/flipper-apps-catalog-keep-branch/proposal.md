# Proposal

## Why

`release.yml`'s `flipper-apps-catalog` job deletes every fork branch `<owner>/f0_claude_remote_*`
before it cuts the release's own (`.github/workflows/release.yml:75-79`),
as `flipper-apps-catalog-submission` planned:
its archived design deletes them all (`design.md:46`)
and takes closing `0.1`'s waiting pull request as a risk (`:83`).
On 2026-11-11 the `v0.2` run's job, 08:42:05 to 08:42:13 UTC,
deleted `petr-korobeinikov/f0_claude_remote_0.1`, the head of the Flipper Apps Catalog's pull request #1387, `Add Claude Remote 0.1`,
and GitHub logged `head_ref_deleted` and `closed` on #1387 at 08:42:10.
At 15:12 the owner restored the branch at `76a552a` and reopened #1387, keeping its place in the review queue.
The next tag would close it again; the owner put this change ahead of the housekeeping pass.

Settled by the owner on 2026-11-11:

- **The change closes with its own commit:**
  no scenario waits for a release or for the Flipper Apps Catalog's state to change.
- **Nothing in the change writes to GitHub on its own:**
  its checks hold no credential, and a push waits for the owner's word.

Decisions:

- **A branch with an open pull request upstream stays, and every other one still goes,**
  not a job deleting only the run's own branch,
  which would leave every merged or superseded branch in the fork for the owner to delete by hand.
  `petr-korobeinikov/f0_claude_remote_0.2` goes at `v0.3` unless a pull request is open from it by then.

## What Changes

- `.github/workflows/release.yml`'s `flipper-apps-catalog` job deletes a fork branch
  only when no pull request is open from it upstream.
- `docs/contributing/release.md:42` says so.
- `flipper-apps-catalog-submission` gains `Open pull request`,
  and `Next submission` names the state it needs instead of `v0.2`, whose run met `0.1` unmerged.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `flipper-apps-catalog-submission`: a release keeps every fork branch with an open pull request upstream.

## Impact

`.github/workflows/release.yml` and `docs/contributing/release.md`.
No new dependency, token, permission or repository setting:
the job reads upstream's pull requests with `github.token`, as it reads this repository's commit.

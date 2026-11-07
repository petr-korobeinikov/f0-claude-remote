# Proposal

## Why

`release.yml` publishes the Release in its `release` job,
and the `flipper-apps-catalog` job runs only after it;
a published Release is immutable, its tag locked and its name spent for good, even once deleted.
Neither `gh release create` nor that job has run on GitHub:
act took `release.yml` up to `gh release create` on 2026-11-07,
and the two steps past it first run at `v0.1`, the first release.
A slip in the `flipper-apps-catalog` job would leave `v0.1` published and locked,
its Flipper Apps Catalog branch made by hand, since a rerun repeats the tag's own workflow.

Settled by the owner on 2026-11-07:

- **The Release is drafted, then published by the run's last job, once every other job has passed,**
  the `flipper-apps-catalog` job included,
  not published with immutable releases turned off for `v0.1`:
  GitHub applies immutability only to releases published while it is on, so `v0.1` would stay mutable.

Proposed here, for the owner to strike:

- **Jobs `draft` and `publish`:** kept, `release` would name the job that leaves a draft.
- **`publish` is a job of its own,**
  not the last step of `flipper-apps-catalog`, which holds the fork's token and no write on this repository.
- **`draft` refuses a tag that already has a Release, draft or published:**
  `--draft` turns off `gh`'s own refusal of a published one and its deletion of a draft whose upload failed,
  so a rerun of `draft` would add a second draft, and `publish` takes one by its tag alone.
- **`contributor-guide`'s `Contributing holds the moved lines` goes:**
  it pins three pages to the `CONTRIBUTING.md` lines `docs-development` moved there,
  and this change is the first to rewrite one of those lines.
- **No act run:** every step this change touches calls GitHub, which act would reach for real;
  the steps before them passed under act on 2026-11-07.
- **Before `v0.1`,** in either order with `dependabot`.

## What Changes

- `release.yml`: the `release` job becomes `draft`, guarded and passing `--draft`;
  `flipper-apps-catalog` follows it, and a new `publish` job publishes the draft last.
- `docs/contributing/release.md`: what the tag runs, and how a failed run is undone.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `release`: the Release is drafted once per tag and published by the run's last job, which also gets write access.
- `flipper-apps-catalog-submission`: the branch is prepared once the Release is drafted, not published.
- `contributor-guide`: the requirement pinning the moved lines goes.

## Impact

No new dependency: `publish` runs the runner's `gh`, unpinned, as `draft` does.
The `flipper-apps-catalog` environment and its token stay as they are.

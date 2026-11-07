# Spec Delta

## Purpose

Cuts a release and publishes its GitHub Release.

## ADDED Requirements

### Requirement: The task cuts the release
`mise run release <name>` on `develop` SHALL start `release/<name>`,
write `fap_version="<name>"` into `application.fam`, regenerate `CHANGELOG.md` and `catalog/changelog.md`, build the app,
commit those three files alone as `chore: release <name>`,
finish the branch, which tags `v<name>`, return to `develop`,
and push `main`, `develop` and the tag to `origin` atomically.

#### Scenario: First release
- **WHEN** `mise run release 0.1` runs on `develop`, no tag existing
- **THEN** its output reads `Target: 7, API: 88.2` before the commit's hook lines, ending on `develop`;
  `release/0.1` is gone; `main` and `develop` hold one `chore: release 0.1` commit, changing those three files alone;
  `application.fam` holds one `fap_version="0.1",` line, right after `fap_category`;
  and `git ls-remote origin` shows `main`, `develop` and `v0.1` where `git rev-parse` does

#### Scenario: Next release
- **WHEN** `mise run release 0.2` runs after `v0.1`
- **THEN** `application.fam` holds one `fap_version` line, reading `0.2`

### Requirement: A wrong or failed release changes nothing
The task SHALL exit non-zero, branches, tags and worktree untouched,
on a name not `MAJOR.MINOR` or not the latest tag's `MINOR` plus one (`0.1` with no tag),
a branch not `develop` or an unclean worktree, each refused in a `release:` line,
and when a step before the release commit fails.
After that commit it SHALL undo nothing: git-flow-next resumes the finish, and the push is rerun.

#### Scenario: Nothing changed
- **WHEN** it runs as `0.1.1`, as `0.3` after `v0.1`, on `main`, with a file edited, or over a commit that breaks the build
- **THEN** each run exits non-zero, and HEAD, `git show-ref --heads --tags` and `git status --short` print as before

#### Scenario: Push fails
- **WHEN** the push of `mise run release 0.2` fails
- **THEN** the run exits non-zero on `develop`, `v0.2` kept, and the push rerun by hand lands all three refs

### Requirement: A version tag publishes a Release
A pushed tag `v<name>` SHALL run `ci.yml`'s jobs and, once they pass and the tag's commit holds `fap_version="<name>"`,
build the app and publish the Release `v<name>`,
with the notes `git-cliff --current --strip all` prints there and `f0_claude_remote.fap` attached.

#### Scenario: Triggers
- **WHEN** `release.yml`'s `on` is read
- **THEN** it holds `push` limited to tags of the form `v<MAJOR>.<MINOR>`, and nothing else

#### Scenario: Version check
- **WHEN** the check runs on a commit whose `application.fam` lacks `fap_version="<name>"`
- **THEN** it exits non-zero, naming the tag, before anything is built

#### Scenario: On GitHub
- **WHEN** `mise run release 0.1` pushes `v0.1`
- **THEN** every job of its `release` run succeeds,
  and `gh release view v0.1` shows a published, immutable Release with those notes and one asset, `f0_claude_remote.fap`

### Requirement: The release's inputs are pinned and its token scoped
`release.yml` SHALL take actions by commit, mise at one version, tools as `mise.lock` records them
and the SDK at `FLIPPER_SDK_VERSION`, restore no cache,
and give write access to the publishing job alone.
The runner's `gh`, like its `cc`, stays unpinned.

#### Scenario: Read off the workflow
- **WHEN** `release.yml` is read
- **THEN** each `uses:` names a 40-hex commit, each `jdx/mise-action` step sets `version: 2026.11.2` and `cache: false`,
  each checkout sets `persist-credentials: false`, and `write` appears in the publishing job's `permissions` alone

# Spec Delta

## Purpose

Cuts a release: the commit that names its version and regenerates its changelogs,
and the GitHub Release its tag publishes.

## ADDED Requirements

### Requirement: The task makes the release commit
`mise run release <name>` on `release/<name>` SHALL write `fap_version="<name>"` into `application.fam`,
regenerate `CHANGELOG.md` and `catalog/changelog.md`, build the app,
and commit those three files alone as `chore: release <name>`.

#### Scenario: First release
- **WHEN** `mise run release 0.1` runs on `release/0.1`, no tag existing
- **THEN** HEAD's subject is `chore: release 0.1`, `git show --name-only HEAD` lists those three files,
  and `application.fam` holds one `fap_version="0.1",` line, right after `fap_category`

#### Scenario: Next release
- **WHEN** `mise run release 0.2` runs on `release/0.2`, after `v0.1`
- **THEN** `application.fam` holds one `fap_version` line, reading `0.2`

#### Scenario: Built first
- **WHEN** the task's output is read
- **THEN** `Target: 7, API: 88.2` precedes the commit's hook lines

### Requirement: The task refuses a wrong release
The task SHALL exit non-zero, with a `release:` line naming the cause and no file or commit changed,
when the name is not `MAJOR.MINOR`, is not the latest tag's `MINOR` plus one (`0.1` with no tag),
is not the branch's `release/<name>`, or the worktree is not clean.

#### Scenario: Refusals
- **WHEN** it runs as `mise run release 0.1.1`, as `mise run release 0.3` after `v0.1`,
  on `develop`, or with a file edited
- **THEN** each run exits non-zero, and HEAD and `git status --short` stay as they were

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
- **WHEN** `v0.1` is pushed after `0.1`'s finish
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

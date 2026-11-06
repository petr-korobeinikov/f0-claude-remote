# changelog Specification

## Purpose

Keeps the record of what each release changes,
generated from the commit history in the release commit,
and gives a GitHub Release the part that is its own.

## Requirements

### Requirement: The changelog is generated
`CHANGELOG.md` at the root SHALL be the generator's output from `cliff.toml`,
at the version pinned in `mise.toml`, written whole and never edited by hand.

#### Scenario: Rerun on a release commit
- **WHEN** `mise exec -- git-cliff --tag v<name> --output CHANGELOG.md` reruns on a release commit, the UTC day it was made
- **THEN** `git diff --exit-code CHANGELOG.md` exits 0

#### Scenario: Hooks pass
- **WHEN** `mise exec -- prek run --files CHANGELOG.md` runs
- **THEN** it exits 0 and changes nothing

### Requirement: The release commit carries the changelog
The release commit, `chore: release <name>` on `release/<name>`,
SHALL regenerate `CHANGELOG.md`, its newest section named `<name>`
and every older section kept below it.

#### Scenario: First release
- **WHEN** `mise exec -- git-cliff --tag v0.1 --output CHANGELOG.md` runs on `release/0.1`, no tag existing
- **THEN** the file opens with `# Changelog` and holds one section, `## 0.1 — <date>`, and no `## Unreleased`

#### Scenario: Next release
- **WHEN** it runs with `--tag v0.2` on `release/0.2`, after `v0.1`
- **THEN** `## 0.2` comes first and lists only commits after `v0.1`, and the `## 0.1` section follows with the same entries

### Requirement: A section lists the release's own work
A section SHALL list each Conventional Commit in its range once, grouped by type,
and leave out merges, other unconventional commits, the release commit and `chore(openspec)` commits.

#### Scenario: Entry count
- **WHEN** a section is generated
- **THEN** its `- ` lines number the subjects in its range that match `^[a-z]+(\(.+\))?!?: `, less those matching `^chore\(openspec\)` or `^chore: release `

#### Scenario: Nothing skipped shows
- **WHEN** the file is generated
- **THEN** no entry reads `Release <name>`, opens with `Merge` or carries the `openspec` scope

### Requirement: Release notes hold one release
The notes for a tag SHALL be that release's section alone, without the file's header,
listing the same entries as its section in `CHANGELOG.md`.

#### Scenario: Notes on a tag
- **WHEN** `mise exec -- git-cliff --current --strip all` runs with `v0.2` checked out
- **THEN** it prints `## 0.2` with its entries, no line of the `0.1` section and no `# Changelog`

#### Scenario: Same entries as the file
- **WHEN** the notes for `v0.2` are compared with `CHANGELOG.md`'s `## 0.2` section at `v0.2`
- **THEN** their `- ` lines are equal

#### Scenario: Untagged commit
- **WHEN** the notes command runs on an untagged commit, two tags existing
- **THEN** it exits non-zero with `No tag exists for the current commit`

### Requirement: The generator stays off the network
A git-cliff run through mise SHALL reach no network and write nothing outside the repository.

#### Scenario: Offline in the project environment
- **WHEN** `mise exec -- printenv GIT_CLIFF_OFFLINE` runs at the root
- **THEN** it prints `true`

#### Scenario: No update check
- **WHEN** `mise exec -- git-cliff --unreleased` runs after a marker file is touched
- **THEN** no file under `~/.cache/update-informer-rs` is newer than the marker

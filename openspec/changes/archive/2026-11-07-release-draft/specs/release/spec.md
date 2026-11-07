# Spec Delta

## MODIFIED Requirements

### Requirement: A version tag publishes a Release
A pushed tag `v<name>` SHALL run `ci.yml`'s jobs and, once they pass and the tag's commit holds `fap_version="<name>"`,
build the app and draft the Release `v<name>`,
with the notes `git-cliff --current --strip all` prints there and `f0_claude_remote.fap` attached.
The run's last job SHALL publish the draft, once every other job has passed.

#### Scenario: Triggers
- **WHEN** `release.yml`'s `on` is read
- **THEN** it holds `push` limited to tags of the form `v<MAJOR>.<MINOR>`, and nothing else

#### Scenario: Version check
- **WHEN** the check runs on a commit whose `application.fam` lacks `fap_version="<name>"`
- **THEN** it exits non-zero, naming the tag, before anything is built

#### Scenario: Published last
- **WHEN** `release.yml`'s jobs are read
- **THEN** the `draft` job creates the Release as a draft, and the `publish` job alone publishes it;
  `publish` needs `flipper-apps-catalog`, which needs `draft`, which needs `ci`, and no job sets `if`

#### Scenario: On GitHub
- **WHEN** `mise run release 0.1` pushes `v0.1`
- **THEN** every job of its `release` run succeeds, `publish` last,
  and `gh release view v0.1` shows a published, immutable Release with those notes and one asset, `f0_claude_remote.fap`

### Requirement: The release's inputs are pinned and its token scoped
`release.yml` SHALL take actions by commit, mise at one version, tools as `mise.lock` records them
and the SDK at `FLIPPER_SDK_VERSION`, restore no cache,
and give write access to the jobs that draft and publish the Release alone.
The runner's `gh`, like its `cc`, stays unpinned.

#### Scenario: Read off the workflow
- **WHEN** `release.yml` is read
- **THEN** each `uses:` names a 40-hex commit, each `jdx/mise-action` step sets `version: 2026.11.2` and `cache: false`,
  each checkout sets `persist-credentials: false`, and `write` appears in the `draft` and `publish` jobs' `permissions` alone

## ADDED Requirements

### Requirement: A tag has one Release
A run SHALL draft no Release for a tag that has one already, draft or published.

#### Scenario: A Release exists
- **WHEN** the `draft` job reaches its last step while `gh release view v<name>` finds a Release
- **THEN** the step exits non-zero, naming the tag, and `gh release create` does not run

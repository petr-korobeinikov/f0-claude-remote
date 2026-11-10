## MODIFIED Requirements

### Requirement: The catalog changelog is generated
The release commit SHALL put git-cliff's section for its release, from `catalog/cliff.toml`, on top of `catalog/changelog.md`,
and keep every section below it as committed.
A section is `v<name>:`, a blank line,
and one `- ` line per `feat` or `fix` commit in its range not scoped `release`, oldest first,
its subject with type, scope and backticks dropped,
or `- Internal changes only` when there is none.

#### Scenario: First release
- **WHEN** the file is read at `v0.1`, written before `release`-scoped commits were left out
- **THEN** it is `v0.1:`, a blank line and one `- ` line per subject in the history matching `^(feat|fix)`

#### Scenario: Next release
- **WHEN** `0.2` is cut after `feat(view): draw the plate` and `fix(release): read the tag`
- **THEN** the file opens with `v0.2:`, a blank line, `- Draw the plate` and a blank line,
  followed by the file as `v0.1` left it

#### Scenario: Release with neither
- **WHEN** `0.2` is cut after a single `chore` commit
- **THEN** the file opens with `v0.2:`, a blank line, `- Internal changes only` and a blank line,
  followed by the file as `v0.1` left it

#### Scenario: Rerun on a release commit
- **WHEN** `mise exec -- git-cliff --config catalog/cliff.toml --current` runs with `v<name>` checked out
- **THEN** the file opens with exactly what it prints

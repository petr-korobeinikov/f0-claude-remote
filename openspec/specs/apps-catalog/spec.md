# apps-catalog Specification

## Purpose

Gives the Flipper Apps Catalog its text from this repository.

## Requirements

### Requirement: Catalog text keeps to the catalog's Markdown
`catalog/description.md` and `catalog/changelog.md` SHALL hold only what the catalog's filter accepts:
paragraphs, `#` and `##` headers, bold, italic, `- ` lists and inline links.

#### Scenario: No refused construct
- **WHEN** each file is searched for a backtick, `<`, an entity, `![`, a `]` not followed by `(`,
  and a line opening with `>`, `###`, four spaces, `---`, `***`, `___` or `===`
- **THEN** no line matches

#### Scenario: Hooks pass
- **WHEN** `mise exec -- prek run --files catalog/description.md catalog/changelog.md` runs
- **THEN** it exits 0 and changes nothing

### Requirement: The description says what the app is
`catalog/description.md` SHALL say what the app does and what each button sends,
and link the site for pairing and keybindings.

#### Scenario: Content
- **WHEN** the file is read
- **THEN** it names OK, Up, Down, Left, Right and Back,
  and links the site's `guide/pairing` and `guide/claude-code-setup` pages

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
- **WHEN** `0.3` is cut after `feat(view): draw the plate` and `fix(release): read the tag`
- **THEN** the file opens with `v0.3:`, a blank line, `- Draw the plate` and a blank line,
  followed by the file as `v0.2` left it

#### Scenario: Release with neither
- **WHEN** `0.3` is cut after a single `chore` commit
- **THEN** the file opens with `v0.3:`, a blank line, `- Internal changes only` and a blank line,
  followed by the file as `v0.2` left it

#### Scenario: Rerun on a release commit
- **WHEN** `mise exec -- git-cliff --config catalog/cliff.toml --current` runs with `v<name>` checked out
- **THEN** the file opens with exactly what it prints

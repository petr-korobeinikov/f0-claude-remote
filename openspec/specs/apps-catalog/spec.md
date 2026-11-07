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
`catalog/changelog.md` SHALL be git-cliff's output from `catalog/cliff.toml`, written whole by the release commit:
every release newest first, as `v<name>:`, a blank line,
and one `- ` line per `feat` or `fix` subject in its range, oldest first, type, scope and backticks dropped,
or `- Internal changes only` when it has none.

#### Scenario: First release
- **WHEN** `0.1`'s release commit is made
- **THEN** the file is `v0.1:`, a blank line and one `- ` line per subject in the history matching `^(feat|fix)`

#### Scenario: Release with neither
- **WHEN** `0.2` is cut after a single `chore` commit
- **THEN** the file opens with `v0.2:`, a blank line and `- Internal changes only`,
  then the `v0.1:` section, unchanged

#### Scenario: Rerun on a release commit
- **WHEN** `mise exec -- git-cliff --config catalog/cliff.toml --tag v<name> --output catalog/changelog.md` reruns on a release commit
- **THEN** `git diff --exit-code catalog/changelog.md` exits 0

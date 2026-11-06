# Spec Delta

## Purpose

Gives the Flipper Apps Catalog its text from this repository,
and prepares each release's submission branch in the owner's fork.

## ADDED Requirements

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
and link the README for pairing and keybindings.

#### Scenario: Content
- **WHEN** the file is read
- **THEN** it names OK, Up, Down, Left, Right and Back,
  and links the README's `#pairing` and `#keys-on-the-mac` on GitHub

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

### Requirement: A release prepares the catalog branch
Once its Release is published, a release SHALL bring the fork's `main` level with upstream `main`,
delete every fork branch `<owner>/f0_claude_remote_*`,
and create `<owner>/f0_claude_remote_<name>` from `main`,
one commit ahead, adding or updating `applications/Bluetooth/f0_claude_remote/manifest.yml` alone.

#### Scenario: First submission
- **WHEN** `v0.1`'s run ends
- **THEN** the fork's `main` equals upstream's, its one `f0_claude_remote_` branch is `_0.1`,
  and that branch's commit reads `Add Claude Remote 0.1`

#### Scenario: Manifest
- **WHEN** that manifest is read
- **THEN** it holds `sourcecode` with `type: git`, `origin` `https://github.com/petr-korobeinikov/f0-claude-remote.git`
  and `commit_sha` equal to `git rev-parse v0.1^{commit}`,
  `description: "@catalog/description.md"`, `changelog: "@catalog/changelog.md"`,
  the screenshot `screenshots/main.png`, and no field `application.fam` supplies

#### Scenario: Next submission
- **WHEN** `v0.2`'s run ends, `0.1` merged upstream
- **THEN** `_0.1` is gone, and `_0.2`'s commit reads `Update Claude Remote to 0.2`

### Requirement: The fork's token stays with the catalog job
The fork's token SHALL be a secret of an environment `catalog` that only `v*` tags may deploy to,
and no other job SHALL reference either.

#### Scenario: Environment
- **WHEN** `release.yml` and the environment's deployment policies are read
- **THEN** one job names `environment: catalog` and `CATALOG_TOKEN`, and the one policy is the tag pattern `v*`

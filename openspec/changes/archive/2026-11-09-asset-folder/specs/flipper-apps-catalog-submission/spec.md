## MODIFIED Requirements

### Requirement: A release prepares the Flipper Apps Catalog branch
Once its Release is drafted, a release SHALL bring the fork's `main` level with upstream `main`,
delete every fork branch `<owner>/f0_claude_remote_*`,
and create `<owner>/f0_claude_remote_<name>` from `main`,
one commit ahead, adding or updating `applications/Bluetooth/f0_claude_remote/manifest.yml` alone.

#### Scenario: First submission
- **WHEN** `v0.1`'s run ends
- **THEN** the fork's `main` equals upstream's, its one `f0_claude_remote_` branch is `_0.1`,
  and that branch's commit reads `Add Claude Remote 0.1`

#### Scenario: Manifest
- **WHEN** the manifest of a release after `v0.1` is read
- **THEN** it holds `sourcecode` with `type: git`, `origin` `https://github.com/petr-korobeinikov/f0-claude-remote.git`
  and `commit_sha` equal to `git rev-parse` of its tag's commit,
  `description: "@catalog/description.md"`, `changelog: "@catalog/changelog.md"`,
  the screenshot `asset/screenshot/main.png`, and no field `application.fam` supplies

#### Scenario: Next submission
- **WHEN** `v0.2`'s run ends, `0.1` merged upstream
- **THEN** `_0.1` is gone, and `_0.2`'s commit reads `Update Claude Remote to 0.2`

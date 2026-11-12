# flipper-apps-catalog-submission Specification

## Purpose

Prepares each release's submission to the Flipper Apps Catalog:
a branch in the owner's fork, from which the owner opens the pull request.

## Requirements

### Requirement: A release prepares the Flipper Apps Catalog branch
Once its Release is drafted, a release SHALL bring the fork's `main` level with upstream `main`,
delete every fork branch `<owner>/f0_claude_remote_*` with no open pull request upstream,
and create `<owner>/f0_claude_remote_<name>` from `main`,
one commit ahead, adding or updating `applications/Bluetooth/f0_claude_remote/manifest.yml` alone.

#### Scenario: First submission
- **WHEN** `v0.1`'s run ends
- **THEN** the fork's `main` equals upstream's, its one `petr-korobeinikov/f0_claude_remote_*` branch is `petr-korobeinikov/f0_claude_remote_0.1`,
  and that branch's commit reads `Add Claude Remote 0.1`

#### Scenario: Manifest
- **WHEN** the manifest of a release after `v0.1` is read
- **THEN** it holds `sourcecode` with `type: git`, `origin` `https://github.com/petr-korobeinikov/f0-claude-remote.git`
  and `commit_sha` equal to `git rev-parse` of its tag's commit,
  `description: "@catalog/description.md"`, `changelog: "@catalog/changelog.md"`,
  the screenshot `asset/screenshot/main.png`, and no field `application.fam` supplies

#### Scenario: Open pull request
- **WHEN** a release's run meets a pull request open upstream from `petr-korobeinikov/f0_claude_remote_0.1`
  and none from `petr-korobeinikov/f0_claude_remote_0.2`
- **THEN** it deletes `petr-korobeinikov/f0_claude_remote_0.2` alone

#### Scenario: Next submission
- **WHEN** a release's run finds `applications/Bluetooth/f0_claude_remote/manifest.yml` on upstream `main`
- **THEN** its branch's commit reads `Update Claude Remote to <name>`

### Requirement: The fork's token stays with one job
The fork's token SHALL be the secret `FLIPPER_APPS_CATALOG_TOKEN`
of an environment `flipper-apps-catalog` that only `v*` tags may deploy to,
not of the repository, and no other job SHALL reference either.

#### Scenario: Environment
- **WHEN** `release.yml`, the environment and the repository's secrets are read
- **THEN** one job names `environment: flipper-apps-catalog` and `FLIPPER_APPS_CATALOG_TOKEN`;
  the environment's one policy is the tag pattern `v*`, and its one secret `FLIPPER_APPS_CATALOG_TOKEN`;
  the repository holds no secret of that name

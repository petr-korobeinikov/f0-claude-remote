# docs-deploy Specification

## Purpose

Publishes the documentation site on GitHub Pages,
with Pages' permissions held by the publishing job alone.

## Requirements

### Requirement: The site publishes from develop
Each push to `develop`, and a manual run, SHALL publish the site to `https://petr-korobeinikov.github.io/f0-claude-remote/`,
and only the publishing job SHALL hold Pages' permissions.

#### Scenario: Workflow
- **WHEN** `docs.yml` is read
- **THEN** it runs on `push` to `develop` and `workflow_dispatch`, grants `contents: read`,
  adds `pages: write` and `id-token: write` in the deploy job alone,
  pins each `uses:` to a 40-hex commit and checks out with `persist-credentials: false`

#### Scenario: Published
- **WHEN** a push to `develop` has run `docs.yml`
- **THEN** the site, its `llms.txt` and `guide/pairing/index.md` answer 200 with that commit's text

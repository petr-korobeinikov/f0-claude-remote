## RENAMED Requirements

- FROM: `### Requirement: The site publishes from develop`
- TO: `### Requirement: The site publishes from main`

## MODIFIED Requirements

### Requirement: The site publishes from main
Each push to `main`, and a manual run on `main`, SHALL publish the site to `https://petr-korobeinikov.github.io/f0-claude-remote/`,
and only the publishing job SHALL hold Pages' permissions.

#### Scenario: Workflow
- **WHEN** `docs.yml` is read
- **THEN** it runs on `push` to `main` and `workflow_dispatch`, grants `contents: read`,
  adds `pages: write` and `id-token: write` in the deploy job alone,
  pins each `uses:` to a 40-hex commit and checks out with `persist-credentials: false`

#### Scenario: Published
- **WHEN** `docs.yml` has run on `main`
- **THEN** the site, its `llms.txt` and `guide/pairing/index.md` answer 200 with that commit's text

## MODIFIED Requirements

### Requirement: The site builds from the repository
`mise run docs-build` SHALL build the site from `docs/` into `.site/` with the Zensical `mise.lock` records,
and fail on a link to a missing page.

#### Scenario: Build
- **WHEN** `mise run docs-build` runs in a fresh clone
- **THEN** it exits 0, `.site/index.html` exists,
  and `.site/assets/screenshot/` holds each file of `asset/screenshot/`, byte for byte

#### Scenario: Dead link
- **WHEN** a page links a missing page
- **THEN** the build exits non-zero and names the link

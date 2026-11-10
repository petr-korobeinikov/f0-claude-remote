## MODIFIED Requirements

### Requirement: The task cuts the release
`mise run release <name>` on `develop` SHALL start `release/<name>`,
write `fap_version="<name>"` into `application.fam`, regenerate `CHANGELOG.md`,
put the release's section on top of `catalog/changelog.md`, build the app,
commit those three files alone as `chore: release <name>`,
finish the branch, which tags `v<name>`, return to `develop`,
and push `main`, `develop` and the tag to `origin` atomically.

#### Scenario: First release
- **WHEN** `mise run release 0.1` runs on `develop`, no tag existing
- **THEN** its output reads `Target: 7, API: 88.2` before the commit's hook lines, ending on `develop`;
  `release/0.1` is gone; `main` and `develop` hold one `chore: release 0.1` commit, changing those three files alone;
  `application.fam` holds one `fap_version="0.1",` line, right after `fap_category`;
  and `git ls-remote origin` shows `main`, `develop` and `v0.1` where `git rev-parse` does

#### Scenario: Next release
- **WHEN** `mise run release 0.2` runs after `v0.1`
- **THEN** `application.fam` holds one `fap_version` line, reading `0.2`

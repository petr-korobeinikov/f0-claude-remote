# Spec Delta

## MODIFIED Requirements

### Requirement: Pushes and pull requests are checked
`ci.yml` SHALL run on each push to `develop` or `main`, on each pull request
and when `release.yml` calls it, and on nothing else.

#### Scenario: Triggers
- **WHEN** the workflow's `on` is read
- **THEN** it holds `push`, limited to `develop` and `main`, `pull_request` and `workflow_call`

#### Scenario: On GitHub
- **WHEN** a commit whose jobs pass under `check-workflow` is pushed to `develop`
- **THEN** its `ci` run ends with `test`, `build` and `hygiene` succeeded

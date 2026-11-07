# contributor-guide Specification

## Purpose

Says how a change is planned, checked and released, on the site rather than in `CONTRIBUTING.md`,
which only points there.

## Requirements

### Requirement: Contributing holds the moved lines
`docs/contributing/workflow.md` SHALL hold `CONTRIBUTING.md`'s `## Workflow`,
`docs/contributing/hooks-and-ci.md` its `## Hooks` and `## CI`,
and `docs/contributing/release.md` its `## Release`, as they stand before this change.

#### Scenario: Moved
- **WHEN** each line of those sections is searched in the three pages
- **THEN** every line is found but headings and the links the move rewrites

### Requirement: CONTRIBUTING.md points to the site
`CONTRIBUTING.md` SHALL hold only a pointer to the site's Contributing pages, as `claude-shim`'s does.

#### Scenario: Pointer
- **WHEN** `CONTRIBUTING.md` is read
- **THEN** it links `https://petr-korobeinikov.github.io/f0-claude-remote/contributing/workflow/` and holds no command

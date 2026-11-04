# Spec Delta

## Purpose

Keeps each source file in the folder of its layer,
so a path names the layer and an include line shows which way a dependency points.

## ADDED Requirements

### Requirement: Every source file sits in its layer's folder
`src/` SHALL hold no file directly and no deeper folder:
each source file and header sits in `src/core/`, `src/ui/` or `src/platform/`.

#### Scenario: Paths
- **WHEN** `git ls-files src | grep -vE '^src/(core|ui|platform)/[^/]+$'` runs
- **THEN** it prints nothing

### Requirement: Includes point down the layers
A file SHALL include nothing from a layer above its own:
the core includes only from its own folder,
and the UI reaches outside its folder only into the core.

#### Scenario: The core
- **WHEN** `git grep -E '#include "[^"]*/' -- src/core` runs
- **THEN** it prints nothing

#### Scenario: The UI
- **WHEN** `git grep -E '#include "[^"]*/' -- src/ui | grep -vF '"../core/'` runs
- **THEN** it prints nothing

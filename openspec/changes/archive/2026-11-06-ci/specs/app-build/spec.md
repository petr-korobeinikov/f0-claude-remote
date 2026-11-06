# Spec Delta

## REMOVED Requirements

### Requirement: The template's workflow stays pinned
**Reason**: `build.yml` is deleted, and `ci.yml` takes its push and pull request runs.
**Migration**: `ci`'s `A run's inputs are pinned` and `The token is read-only` hold its rules for `ci.yml`.

### Requirement: Local workflow runs stay contained
**Reason**: It moves to `ci`, with the workflow it runs.
**Migration**: `ci`'s requirement of the same name, against `ci.yml`.

## MODIFIED Requirements

### Requirement: Build and run gives the daily commands
`docs/development/build-and-run.md` SHALL give `mise run build`, `mise run launch` with what it needs of the device,
`mise exec -- ufbt lint` and `mise run clang-tidy`, and say why `ufbt flash` is no launch.

#### Scenario: As written
- **WHEN** the page's build, lint and clang-tidy commands run in a set-up clone
- **THEN** each exits 0

### Requirement: CLion opens a clone with no plugin
`docs/development/clion.md` SHALL take a set-up clone to navigation, the run configurations and the checks its editor shows:
`clion .`, the compilation database, the two tracked `.idea` files, `Flipper` picked once, and `.clang-tidy`.

#### Scenario: Content
- **WHEN** the page is read
- **THEN** it names `Build`, `Launch`, `Debug` and `Test`, `.idea/misc.xml`, `.idea/debug-profiles.xml`, `Flipper`,
  `.clang-tidy` and `mise run clang-tidy`

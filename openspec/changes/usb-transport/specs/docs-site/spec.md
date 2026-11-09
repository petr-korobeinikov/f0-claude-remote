## MODIFIED Requirements

### Requirement: Pages read from simple to deep
The sidebar SHALL list Quick start, then the Guide's Installation, Pairing, USB, Claude Code setup,
Using the remote and Troubleshooting,
then Development's Toolchain, Build and run, Host tests, CLion, Debug probe and Debugging,
then Internals' Architecture, Bluetooth, USB and Keys in Claude Code,
then Contributing's Workflow, Hooks and CI and Release, in that order,
and each page SHALL carry a one-sentence description.

#### Scenario: Sidebar
- **WHEN** the built sidebar and each page's front matter are read
- **THEN** the sidebar lists those twenty pages in order, each one existing, and each `description` is one sentence

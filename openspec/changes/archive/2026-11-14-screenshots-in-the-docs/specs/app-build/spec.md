## MODIFIED Requirements

### Requirement: The app's pictures live under `asset/`
The repository SHALL keep every picture of the app under `asset/`, in one folder per purpose:
the Apps menu's icon in `asset/icon/`, what the screens draw in `asset/image/`, the screenshots in `asset/screenshot/`.
Only the site's links to the screenshots SHALL stand outside it.

#### Scenario: Paths
- **WHEN** `git ls-files '*.png' | grep -vE '^asset/(icon|image|screenshot)/[^/]+$'` runs
- **THEN** it prints `docs/assets/screenshot/<name>` for each `asset/screenshot/<name>`,
  each a link reading `../../../asset/screenshot/<name>`

#### Scenario: The icon
- **WHEN** `git ls-files asset/icon` runs
- **THEN** it prints `asset/icon/f0_claude_remote.png` alone

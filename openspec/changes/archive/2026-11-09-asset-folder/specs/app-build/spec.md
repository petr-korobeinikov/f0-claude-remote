## ADDED Requirements

### Requirement: The app's pictures live under `asset/`
The repository SHALL keep every picture of the app under `asset/`, in one folder per purpose:
the Apps menu's icon in `asset/icon/`, what the screens draw in `asset/image/`, the screenshots in `asset/screenshot/`.
Only the site's link to a screenshot SHALL stand outside it.

#### Scenario: Paths
- **WHEN** `git ls-files '*.png' | grep -vE '^asset/(icon|image|screenshot)/[^/]+$'` runs
- **THEN** it prints `docs/assets/main.png` alone

#### Scenario: The icon
- **WHEN** `git ls-files asset/icon` runs
- **THEN** it prints `asset/icon/f0_claude_remote.png` alone

### Requirement: The package takes its pictures from `asset/`
The package SHALL carry the icon from `asset/icon/` and every image from `asset/image/`,
and nothing from `asset/screenshot/`.

#### Scenario: Images
- **WHEN** `mise run build` has run
- **THEN** `.ufbt/build/f0_claude_remote/f0_claude_remote_icons.h` declares one `I_<name>` for each `asset/image/<name>.png`,
  and no `I_main` or `I_f0_claude_remote`

### Requirement: A missing picture stops the build
`mise run build` SHALL fail when the icon or an image the screens draw is missing,
so no package ships without it.

#### Scenario: A missing image
- **WHEN** `asset/image/up.png` is missing and `mise run build` runs
- **THEN** it exits non-zero and names `I_up`

#### Scenario: A missing icon
- **WHEN** `asset/icon/f0_claude_remote.png` is missing and `mise run build` runs
- **THEN** it exits non-zero and names the file

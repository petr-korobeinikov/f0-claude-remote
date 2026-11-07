# Proposal

## Why

The docs are two files read on GitHub:
`README.md`, 235 lines of install, pairing, keys and debugging,
and `CONTRIBUTING.md`, 220 lines for contributors.
No page can be searched or fetched by a language model,
and what fifteen archived changes learned sits where no user looks.
`catalog/description.md` links the README's `#pairing` and `#keys-on-the-mac` for the Flipper Apps Catalog,
so their new pages must exist before `v0.1` is submitted.

Settled by the owner on 2026-11-06:

- **A site on GitHub Pages, with `llms.txt`, light and dark,** in English, in the approved tree, simple to deep.
- **Three changes:** `docs` builds the site with Home, Quick start and the Guide;
  `docs-development` moves Debugging and `CONTRIBUTING.md` in; `docs-internals` writes Internals.

Settled by the owner on 2026-11-07:

- **Zensical 0.1.0, theme variant `modern`,** not VitePress as first planned, nor Material for MkDocs:
  Material is in maintenance until its end of life on 2027-05-05, and Zensical is its successor by the same authors.
  Installed as `pypi:zensical` on a private Python 3.14.10 from a wheel-only locked graph,
  as ufbt is here and Zensical is in `staroxi-tales-kmp-app`:
  no npm tree, no second package manager, nothing in a system Python.

Proposed here, for the owner to strike:

- **The site builds into `.site/`,** not Zensical's default `site/`:
  fbt gathers `*.c*`, which takes the theme's `.css`, and `ufbt lint` rejects folders such as `quick-start`;
  both skip dot-folders, as they skip the `.cache/` Zensical leaves, which ignores itself.
  `application.fam`'s sources stay as they are: `docs/` holds no `*.c*` file.
- **The screenshot reaches the site through a tracked symlink,** `docs/assets/main.png` to `screenshots/main.png`:
  Zensical copies only what `docs/` holds, and a strict build does not fail an image outside it;
  `screenshots/main.png` stays where `flipper-apps-catalog-submission`'s manifest names it.
- **The theme's fonts are off,** so a page calls no Google Fonts; the reader's system fonts serve.
- **Deploy on push to `develop`,** not on a release as in `claude-shim`:
  a release here costs a Flipper Apps Catalog pull request, and the site matches the README GitHub shows.
  It may then describe what the latest Release lacks.
- **A `docs` job in `ci.yml`,** so a dead link or a bump fails its own push; like the others, with no prek hook.
- **Installation offers the Release's `.fap` and the source;** the Flipper Apps Catalog route waits for the listing.
  The `.fap` goes through qFlipper's File manager, the one upload Flipper documents,
  into `apps/Bluetooth/`, where `mise run launch` puts it.
- **The README keeps its head, Why, Features, install commands and Debugging;**
  Usage moves to the Guide, and what points at it follows.
- **Before `v0.1`:** the first Flipper Apps Catalog description links the site.

Audit: `staroxi-tales-kmp-app`'s of 2026-11-07, for the same versions on the same Python, Socket dropped as always:

| Package | Version | License | Source | Known vulnerabilities | Code at install |
|---|---|---|---|---|---|
| `zensical` | 0.1.0 | MIT | PyPI, `zensical/zensical` | none | none, abi3 wheel |
| `zensical-ui` | 0.0.41 | MIT | PyPI | none | none, wheel |
| 9 transitive | | BSD, MIT | PyPI | none | none, wheels |
| `actions/upload-pages-artifact` | v5.0.0 `fc324d35` | MIT | GitHub | none | `actions/upload-artifact` by commit |
| `actions/deploy-pages` | v5.0.1 `368f8252` | MIT | GitHub | none | none |

The nine: click 8.5.1, Jinja2 3.1.6, Markdown 3.11.3, MarkupSafe 3.0.4, pathspec 1.1.1, Pygments 2.21.0,
pymdown-extensions 12.3, PyYAML 6.0.3, tomli 2.5.0.
Checked there with pip-audit over the full tree, Snyk, and a sandbox with no host mount and the network cut:
wheels only, and a strict build failed a broken link.
`llms.txt` comes from Zensical's own `llmstxt` plugin, so it adds no package.
Task 1.1 checks that this project's lock resolves the same eleven.

## What Changes

- New: Home, Quick start and five Guide pages under `docs/`, `zensical.toml`,
  tasks `docs-build` and `docs-dev`, `.github/workflows/docs.yml`.
- `mise.toml`, `mise.lock` and `.mise/locks/` gain Zensical;
  `ci.yml`, `.gitignore`, `README.md`, `catalog/description.md` and `CONTRIBUTING.md` follow.

## Capabilities

### New Capabilities

- `docs-site`: building and previewing the site, its theme, search and `llms.txt`, kept out of the app.
- `docs-deploy`: publishing it from `develop`.
- `user-guide`: Home, Quick start, the Guide, and what the README keeps.

### Modified Capabilities

- `ci`: a fourth job builds the site.
- `voice-control`, `prompt-control`, `effort-control`: the `keybindings.json` block moves to the Guide.
- `apps-catalog`: the description links the site, not the README.

## Impact

The owner sets Pages' source to GitHub Actions, once.

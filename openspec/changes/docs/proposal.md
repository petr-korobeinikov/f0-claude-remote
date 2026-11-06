# Proposal

## Why

The docs are two files read on GitHub:
`README.md`, 235 lines of install, pairing, keys and debugging,
and `CONTRIBUTING.md`, 157 lines for contributors.
No page can be searched or fetched by a language model,
and what twelve archived changes learned sits where no user looks.
`release` links the README's `#pairing` and `#keys-on-the-mac` from the Apps Catalog,
so their new pages must exist before `v0.1` is submitted.

Settled by the owner on 2026-11-06:

- **A site on GitHub Pages, with `llms.txt`, light and dark,** in English, in the approved tree, simple to deep.
- **Three changes:** `docs` builds the site with Home, Quick start and the Guide;
  `docs-development` moves Debugging and `CONTRIBUTING.md` in; `docs-internals` writes Internals.

Proposed here, for the owner to strike:

- **VitePress 1.6.4 and `vitepress-plugin-llms` 1.14.0, as in `claude-shim`,**
  not 2.0.0-alpha.21, an alpha, nor MkDocs, a second Python beside ufbt's:
  its theme follows the system's scheme with a switch and searches locally.
- **The npm tree lives in `docs/.vitepress/`:**
  fbt gathers `*.c*` and `ufbt lint` checks names in every folder but dot-folders,
  and the tree holds 206 such files and 160 such folders.
  `application.fam`'s sources drop `docs`, as they drop `test`.
- **`npm ci` from the audited lock, scripts off:**
  the tree's only two swap esbuild's launcher for its binary and rebuild fsevents' shipped one with node-gyp.
- **Deploy on push to `develop`,** not on a release as in `claude-shim`:
  a release here costs a catalog pull request, and the site matches the README GitHub shows.
  It may then describe what the latest Release lacks.
- **A `docs` job in `ci.yml`,** so a dead link or a bump fails its own push; like the others, with no prek hook.
- **Installation offers the Release's `.fap` and the source;** the catalog route waits for the listing.
  The `.fap` goes through qFlipper's File manager, the one upload Flipper documents,
  into `apps/Bluetooth/`, where `mise run launch` puts it.
- **The README keeps its head, Why, Features, install commands and Debugging;**
  Usage moves to the Guide, and what points at it follows.
- **After `release`, before `dependabot` and `v0.1`:**
  the first catalog description links the site, and `dependabot` covers npm.

Audit of 2026-11-06, Socket dropped by the owner:

| Package | Version | License | Source | Known vulnerabilities | Transitive |
|---|---|---|---|---|---|
| `vitepress` | 1.6.4, 2025-08 | MIT | vuejs, 1.97M a week | none | with the plugin, 206 installed, 254 locked; 240 MIT, the rest permissive |
| `vitepress-plugin-llms` | 1.14.0, 2026-09, SLSA provenance | MIT | one maintainer, 102k a week | none | above |
| `actions/upload-pages-artifact` | v5.0.0 `fc324d35` | MIT | GitHub | none | `actions/upload-artifact` by commit |
| `actions/deploy-pages` | v5.0.1 `368f8252` | MIT | GitHub | none | none |

In the tree, OSV and `npm audit` flag vite 5.4.21 eight times,
each needing its dev server on the network, Windows, WSL or vite behind custom middleware, none of which applies here;
esbuild 0.21.5 once, on its own server, which vite never starts;
minimatch 10.2.6 once, for patterns only the plugin and our config supply.
The build reaches none of them.

## What Changes

- New: Home, Quick start and five Guide pages under `docs/`, the site's config and npm tree under `docs/.vitepress/`,
  tasks `docs-build` and `docs-dev`, `.github/workflows/docs.yml`.
- `ci.yml`, `application.fam`, `.gitignore`, `README.md`, `catalog/description.md` and `CONTRIBUTING.md` follow.

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

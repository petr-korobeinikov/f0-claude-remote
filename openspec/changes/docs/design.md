# Design

## Context

See `proposal.md` for why.
No app layer is touched, and `application.fam` keeps its sources.

The plan rests on:

| Fact | Where |
|---|---|
| `variant` defaults to `modern`; `font = false` drops Google Fonts; the palette's `media` follows the system, and the switch is kept in `localStorage` | Zensical 0.1.0's `config.py`, its built pages |
| `build --strict` exits 1 on a link to a missing page, naming file, line and column; a missing image passes | a build in this session |
| `llmstxt` needs `site_url`, writes `llms.txt` from `sections` and `full_output`, and a `<path>/index.md` per listed page, absolute under `site_url` | `config.py`; a build in this session |
| `serve -a 127.0.0.1:<port>` listens there alone, serves under `site_url`'s path and reloads over a WebSocket | a run in this session |
| A build leaves `.cache/`, which holds its own `.gitignore` | a build in this session; `staroxi-tales-kmp-app` |
| A symlink under `docs/` is copied as the file it points to | a build in this session |
| A deploy needs `pages: write`, `id-token: write` and GitHub Actions as Pages' source | GitHub docs |
| `upload-pages-artifact` v5.0.0 pins `upload-artifact` by commit and drops dot-files inside its path | its `action.yml` |
| The `github-pages` environment admits the default branch | `claude-shim`'s lists `develop` |
| fbt gathers `sources` recursively, skipping dot-names; `ufbt lint` walks the app folder, skipping dot-folders and `lib` | `sconsrecursiveglob.py`, SCons `FS.py`, `lint.py:44` |
| `mise run launch` installs to `/ext/apps/Bluetooth/` | `app-build`'s Launch scenario, `fbt_extapps.py:210` |
| qFlipper's File manager uploads a file to the SD card | docs.flipper.net, qFlipper |

Rows 1, 3, 4, 5, 6 and 9 come from sources or a run: a bump can break them.

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** Development, Contributing and Internals; the catalog route; screenshots beyond `main.png`;
a custom theme or colors; math, Mermaid and anything else the theme fetches from a CDN.

## Decisions

| File | Its one task |
|---|---|
| `mise.toml`, `mise.lock`, `.mise/locks/pypi-zensical/` | Pin Zensical and its graph |
| `zensical.toml` | Describes the site |
| `docs/index.md`, `docs/quick-start.md`, `docs/guide/*.md` | Each meets its `user-guide` requirement |
| `docs/assets/main.png` | Symlinks `../../screenshots/main.png` |
| `.mise/tasks/docs-build` | Builds the site |
| `.mise/tasks/docs-dev` | Serves the pages with live reload |
| `.github/workflows/docs.yml` | Publishes the site |
| `.github/workflows/ci.yml` | Gains the `docs` job |
| `.gitignore` | Ignores `/.site/` |
| `README.md`, `catalog/description.md` | Point at the site |
| `CONTRIBUTING.md` | Lists what this change adds |

**`mise.toml`:** `"pypi:zensical" = { version = "0.1.0", python = "3.14.10" }` after `"pypi:ufbt"`,
with a comment: it builds `docs/`, and 3.15 is out, PyYAML 6.0.3 having no `cp315` wheel.
`uv` and the guard block are there already; `mise lock` writes the rest.

**`zensical.toml`,** at the root, where Zensical looks:
`site_name` `F0 Claude Remote`, the tagline as `site_description`,
`site_url` `https://petr-korobeinikov.github.io/f0-claude-remote/`, `site_dir` `.site`, `repo_url` the repository;
`nav` Home, Quick start, then Guide with its five pages in `docs-site`'s order;
`theme.variant` `modern`, written out though it is the default, `theme.language` `en`, `theme.font = false`,
features `navigation.instant`, `content.code.copy`, `search.highlight`;
two palette entries, `default` and `slate`, each with its `prefers-color-scheme` media and a Lucide sun or moon toggle;
`copyright` the README's license line;
`plugins.llmstxt` with `markdown_description` the tagline, `full_output` `llms-full.txt`,
and sections `Start` for `index.md` and `quick-start.md`, `Guide` for `guide/*.md`;
extensions `admonition`, `attr_list`, `tables`, `toc` with permalinks, `pymdownx.highlight`,
and `pymdownx.superfences` without the Mermaid fence.

**Tasks,** `#!/bin/sh` with `set -eu`:
`docs-build` runs `mise exec -- zensical build --strict --clean`,
and `docs-dev` `exec`s `mise exec -- zensical serve -a 127.0.0.1:8000`.

**Pages** open with front matter `description`, in semantic line breaks as the README,
and link each other relatively, so the strict build checks every link.
Home is a plain page: its `# F0 Claude Remote`, the tagline, `assets/main.png`, a link to Quick start, then Why and Features.
Every fact comes from a tracked file:

| Page | Its sources |
|---|---|
| Home, Quick start | README's head, Why, Features, Installation, Usage |
| Installation | README's Installation, `app-build`, `release`'s Release |
| Pairing | README's Pairing, `bluetooth-link`, archived `ble-transport` |
| Claude Code setup | README's keys block and notes, `voice-control`, `effort-control`, `prompt-control` |
| Using the remote | README's keys table, `app-screen`, `button-actions` |
| Troubleshooting | the above and archived `ble-transport`, `effort-actions`, `remote-actions` |

**Workflows:** `ci.yml`'s `docs` job runs checkout and mise-action as the others do, with `install_args: uv pypi:zensical`,
then `mise run docs-build`.
`docs.yml` sets `MISE_AUTO_INSTALL: "false"` and `concurrency: pages` without cancelling;
job `build` repeats those steps, then `upload-pages-artifact@fc324d3547104276b827a68afc52ff2a11cc49c9 # v5.0.0` from `.site`;
job `deploy` takes `environment: github-pages` and runs `deploy-pages@368f82528645a54fb793d4d04e342629a3f51346 # v5.0.1`.

- **`zensical.toml`, not `mkdocs.yml`:** Zensical reads both, the second for migrants, and nothing here migrates.
- **An explicit `nav`:** without it the folder's names order the pages, not simple to deep.
- **`docs.yml` builds for itself,** not from `ci.yml`, which uploads nothing; a push builds twice.
- **No `configure-pages`:** `site_url` is fixed, and Zensical is not among the generators it configures.
- **The screenshot by symlink,** not a copy: one file serves the README, the catalog and the site.
- **Default theme colors,** not Flipper's `#ff8200`: on white it reads 2.5:1, under WCAG AA's 4.5:1 for links.
- **Port 8000,** Zensical's default, bound to 127.0.0.1 by hand: the default `localhost` may also take `::1`.

**Test seam:** the strict build, which a dead link fails in `docs-build`, `ci.yml` and `check-workflow`.
Content is read against `user-guide`, the rest checked in a browser, the deploy after a push.

## Risks / Trade-offs

- `apps-catalog` is not in `openspec/specs/` when this applies → the first task checks it, else stops: `release` comes first.
- The lock resolves other versions than the audit's → task 1.2 stops, and the difference is audited before going on.
- Zensical 0.x changes its configuration between minors → pinned in full; a bump is its own change, rebuilt strictly.
- The strict build passes a missing image → `docs-site`'s `Build` compares the built screenshot with its source.
- A checkout without symlinks, as Git for Windows by default → out of scope: the toolchain is the Mac's and Linux's.
- The theme's bundle carries CDN URLs for features left off → turning one on is a change of its own.
- The owner never takes the Release route → it rests on Flipper's docs and `mise run launch`'s path.
- The registry times out from colima's VM → rerun `check-workflow`.

# Design

## Context

See `proposal.md` for why.
No app layer is touched: `application.fam` only drops `docs` from its sources.

The plan rests on:

| Fact | Where |
|---|---|
| The system's scheme with a switch, local search and failing dead links are defaults | VitePress 1.6.4 docs |
| Each of vite 5.4.21's eight advisories needs its dev server on the network, Windows, WSL or vite behind custom middleware | the advisories on OSV |
| The plugin writes `llms.txt`, `llms-full.txt` and a `.md` per page, absolute under `domain` and `base` | its README; `claude-shim`'s `llms.txt` |
| A deploy needs `pages: write`, `id-token: write` and GitHub Actions as Pages' source | GitHub docs |
| `upload-pages-artifact` v5.0.0 pins `upload-artifact` by commit and drops dot-files | its `action.yml` |
| The `github-pages` environment admits the default branch | `claude-shim`'s lists `develop` |
| fbt gathers `sources` recursively, skipping dot-names; `ufbt lint` walks the app folder, skipping dot-folders and `lib` | `sconsrecursiveglob.py`, SCons `FS.py`, `lint.py:44` |
| `mise run launch` installs to `/ext/apps/Bluetooth/` | `app-build`'s Launch scenario, `fbt_extapps.py:210` |
| qFlipper's File manager uploads a file to the SD card | docs.flipper.net, qFlipper |
| esbuild's postinstall swaps its launcher for its binary, offline; fsevents' rebuilds a shipped binary | the audit; `fsevents` 2.3.3 |

Rows 5, 7 and 10 come from sources: a bump can break them.

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** Development, Contributing and Internals; the catalog route; screenshots beyond `main.png`;
a custom theme.

## Decisions

| File | Its one task |
|---|---|
| `docs/.vitepress/package.json`, `package-lock.json` | Pin the audited tree |
| `docs/.vitepress/.npmrc` | Keeps npm from running scripts or calling home |
| `docs/.vitepress/config.mts` | Describes the site |
| `docs/index.md`, `docs/quick-start.md`, `docs/guide/*.md` | Each meets its `user-guide` requirement |
| `.mise/tasks/docs-build` | Builds the site |
| `.mise/tasks/docs-dev` | Serves the pages with live reload |
| `.github/workflows/docs.yml` | Publishes the site |
| `.github/workflows/ci.yml` | Gains the `docs` job |
| `application.fam` | `sources` gains `"!docs"` |
| `.gitignore` | Ignores what the docs tasks write |
| `README.md`, `catalog/description.md` | Point at the site |
| `CONTRIBUTING.md` | Lists what this change adds |

**The tree:** `package.json` is `private`, `type: module`, both packages exact in `devDependencies`;
the lock is the audit's, sha256 `cfda831f9b36…`, from `f0-claude-remote-working-files/docs-audit/`.
`.npmrc` sets `ignore-scripts`, `save-exact`, and `audit`, `fund` and `update-notifier` off.

**`config.mts`:** title `F0 Claude Remote`, the tagline as description, `base` `/f0-claude-remote/`, `cleanUrls`, `lang` `en-US`;
nav Quick start and Guide, the sidebar `docs-site` orders, `search.provider` `local`, a GitHub link,
the README's license line as footer; `llmstxt({ domain: 'https://petr-korobeinikov.github.io' })` in `vite.plugins`.

**Tasks,** `#!/bin/sh` with `set -eu`, each running `npm ci` in `docs/.vitepress` first:
`docs-build` then runs `docs/.vitepress/node_modules/.bin/vitepress build docs`,
and `docs-dev` `exec`s `vitepress dev docs --host 127.0.0.1`, as `claude-shim`'s `docs:dev`.

**Pages** open with front matter `title` and `description`, in semantic line breaks as the README,
and link each other relatively, so the build checks every link.
Home has `layout: home`, a hero with Quick start then GitHub, and no `features`,
so its Markdown, the screenshot, Why and Features, follows the hero.
Every fact comes from a tracked file:

| Page | Its sources |
|---|---|
| Home, Quick start | README's head, Why, Features, Installation, Usage |
| Installation | README's Installation, `app-build`, `release`'s Release |
| Pairing | README's Pairing, `bluetooth-link`, archived `ble-transport` |
| Claude Code setup | README's keys block and notes, `voice-control`, `effort-control`, `prompt-control` |
| Using the remote | README's keys table, `app-screen`, `button-actions` |
| Troubleshooting | the above and archived `ble-transport`, `effort-actions`, `remote-actions` |

**Workflows:** `ci.yml`'s `docs` job runs checkout and mise-action as the others do, with `install_args: node`,
then `mise run docs-build`.
`docs.yml` sets `MISE_AUTO_INSTALL: "false"` and `concurrency: pages` without cancelling;
job `build` repeats those steps, then `upload-pages-artifact@fc324d3547104276b827a68afc52ff2a11cc49c9 # v5.0.0` from `docs/.vitepress/dist`;
job `deploy` takes `environment: github-pages` and runs `deploy-pages@368f82528645a54fb793d4d04e342629a3f51346 # v5.0.1`.

- **Tasks call `vitepress` by path,** not npm scripts: commands live in `.mise/tasks/` alone.
- **`docs.yml` builds for itself,** not from `ci.yml`, which uploads nothing; a push builds twice.
- **No `configure-pages`:** `base` is fixed, and VitePress is not among the generators it configures.
- **The screenshot by relative path,** not a copy in `docs/public/`: one file serves the README, the catalog and the site.
- **Default theme colors,** not Flipper's `#ff8200`: on white it reads 2.5:1, under WCAG AA's 4.5:1 for links.
- **`cleanUrls`:** Pages serves `guide/pairing` from `pairing.html`, so links drop `.html`.

**Test seam:** the build, which a dead link fails in `docs-build`, `ci.yml` and `check-workflow`.
Content is read against `user-guide`, the rest checked in a browser, the deploy after a push.

## Risks / Trade-offs

- `apps-catalog` is not in `openspec/specs/` when this applies → the first task checks it, else stops: `release` comes first.
- The screenshot outside `docs/` breaks the build → it moves into `docs/public/`, and its users follow.
- esbuild's launcher fails on the Mac → `npm rebuild esbuild` runs that one script, which the sandbox saw stay offline.
- The owner never takes the Release route → it rests on Flipper's docs and `mise run launch`'s path.
- The registry times out from colima's VM → rerun `check-workflow`.
- VitePress 1.x never patches vite's dev server → `docs-dev` listens on 127.0.0.1 alone.

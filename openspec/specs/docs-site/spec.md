# docs-site Specification

## Purpose

Builds the documentation site from `docs/` with Zensical from a locked graph,
read light or dark and offered to language models as Markdown.

## Requirements

### Requirement: The site builds from the repository
`mise run docs-build` SHALL build the site from `docs/` into `.site/` with the Zensical `mise.lock` records,
and fail on a link to a missing page.

#### Scenario: Build
- **WHEN** `mise run docs-build` runs in a fresh clone
- **THEN** it exits 0, `.site/index.html` exists, and `.site/assets/main.png` is byte for byte `screenshots/main.png`

#### Scenario: Dead link
- **WHEN** a page links a missing page
- **THEN** the build exits non-zero and names the link

### Requirement: Pages read from simple to deep
The sidebar SHALL list Quick start, then the Guide's Installation, Pairing, Claude Code setup,
Using the remote and Troubleshooting, in that order,
and each page SHALL carry a one-sentence description.

#### Scenario: Sidebar
- **WHEN** the built sidebar and each page's front matter are read
- **THEN** the sidebar lists those six pages in order, each one existing, and each `description` is one sentence

### Requirement: The site follows the reader's color scheme
The site SHALL use Zensical's `modern` theme, open light or dark as the reader's system prefers,
and offer a switch to the other that holds across pages.

#### Scenario: Scheme
- **WHEN** a page opens with the system dark, the switch is used, then another page opens
- **THEN** the first renders dark, and the second light

### Requirement: Language models get the site as Markdown
The site SHALL serve `llms.txt` at its root, listing every sidebar page by the absolute URL of its Markdown,
`llms-full.txt` with their text, and each page's Markdown at its URL followed by `index.md`.

#### Scenario: Index
- **WHEN** the built `llms.txt` and `llms-full.txt` are read
- **THEN** the first links each sidebar page as `https://petr-korobeinikov.github.io/f0-claude-remote/<path>/index.md`,
  `index.md` as the site's root, and the second holds each page's text once

### Requirement: The site calls no one else
A built page SHALL load nothing from a host other than its own,
but the repository's facts the header shows from `api.github.com`.

#### Scenario: No third-party host
- **WHEN** a page of `docs-dev`'s site opens with the browser's network log on
- **THEN** every request goes to `127.0.0.1:8000`, but those to `api.github.com/repos/petr-korobeinikov/f0-claude-remote`

### Requirement: The site searches itself
The site SHALL search its own pages in the browser, with no outside service.

#### Scenario: Search
- **WHEN** `pairing` is typed into the search of `docs-dev`'s site
- **THEN** the Pairing page is offered

### Requirement: The site reloads as it is edited
`mise run docs-dev` SHALL serve the site to this machine alone
and show each saved edit without a manual reload.

#### Scenario: Live edit
- **WHEN** `mise run docs-dev` runs and an open page's file is saved with a change
- **THEN** the page at `http://127.0.0.1:8000/f0-claude-remote/` shows it without a manual reload

#### Scenario: Loopback only
- **WHEN** `mise run docs-dev` runs
- **THEN** `lsof -nP -iTCP:8000 -sTCP:LISTEN` lists `127.0.0.1:8000` alone

### Requirement: The site stays out of the app
The site's pages, build and cache SHALL reach neither the app's build nor its lint,
and the docs tasks SHALL leave the worktree clean.

#### Scenario: Build and lint
- **WHEN** `mise run build` and `mise exec -- ufbt lint` run after `mise run docs-build`
- **THEN** both exit 0, `compile_commands.json` lists no path under `docs/`, and lint names nothing under `docs/`

#### Scenario: Worktree
- **WHEN** `mise run docs-build` has run in a fresh clone
- **THEN** `git status --short` prints nothing

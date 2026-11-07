# Tasks

No task changes business logic, so none opens with a failing test: the site's build is the test, per the design.
Nothing touches the device.

## 1. Tree

- [x] 1.1 Add `pypi:zensical` to `mise.toml` per the design, locked as `CONTRIBUTING.md` bumps a tool,
  and add `/.site/` to `.gitignore`.
  Check: the new `uv.lock` lists exactly the proposal's eleven packages at its versions, else stop and audit the difference;
  `mise install --locked` exits 0, `mise exec -- zensical --version` prints `0.1.0`; `mise exec -- prek run --all-files` exits 0.
  The lock resolved the eleven at the audit's versions.
  A bare `mise install --locked` fails on the owner's global mise config, unlocked tools; it exits 0 given this project's tools by name.

## 2. Site

- [x] 2.1 Write `zensical.toml`, the tasks `docs-build` and `docs-dev`, the screenshot's symlink and Home per `user-guide`,
  and add both tasks to `CONTRIBUTING.md`'s list.
  Check: `docs-site`'s `Build` holds; a temporary link to `./missing.md` fails as `Dead link` says;
  `Live edit`, `Loopback only` and `No third-party host` hold, with the screenshot on Home.
  `repo_url` fetches from `api.github.com`; the owner kept it and widened `No third-party host` inline.
  Tasks call `zensical` bare, as `build` calls `ufbt`. Rebuild on save and hosts checked by `curl` and the bundle; the browser's view joins 2.3's check.
- [x] 2.2 Run `mise run build` and `mise exec -- ufbt lint` after `mise run docs-build`.
  Check: `docs-site`'s `Build and lint` and `Worktree` hold; `git status --short --ignored` names only `.site/` and `.cache/` beyond what it named before.
  Lint's only warnings name `openspec/` folders, as before this change; `.vscode/compile_commands.json` lists nothing under `docs/`.
- [x] 2.3 Write Quick start and the Guide's five pages per `user-guide`, from the design's sources only.
  Check: each `user-guide` scenario by reading; `Sidebar` and `Index` on the build;
  in `docs-dev`'s site the owner sees the `modern` theme, the scheme follow the system and the switch, and search offer Pairing.
  The owner found the site acceptable. `llms.txt` lists the Guide in sidebar order, page by page, not by `guide/*.md`.
- [x] 2.4 Move the README's Usage out and link the site per `user-guide`; point `catalog/description.md` at the site per this change's `apps-catalog`.
  Check: `README` and `Content` hold, so do `apps-catalog`'s `No refused construct` and `Hooks pass`;
  `rg -n '#pairing|#keys-on-the-mac'` finds nothing outside `openspec/`.
  Beyond the design's files: `src/core/actions.c`'s comment named `README.md` as where the chords are bound; it now names the Guide's page.

## 3. Workflows

- [ ] 3.1 Add the `docs` job to `ci.yml` and write `docs.yml` per the design; add the job to `CONTRIBUTING.md`'s `## CI`.
  Check: `mise exec -- actionlint -pyflakes=` exits 0; `Workflow`, `Commands` and `Pinned references` hold by reading;
  `mise run check-workflow` meets `Local outcome`.

## 4. The owner's setup

- [ ] 4.1 The owner, not the session, sets Pages' source to GitHub Actions:
  `gh api -X POST repos/petr-korobeinikov/f0-claude-remote/pages -f build_type=workflow`.
  Check, by reads alone: `gh api repos/petr-korobeinikov/f0-claude-remote/pages --jq .build_type` prints `workflow`.

## 5. Close

- [ ] 5.1 Check: `mise exec -- prek run --all-files`, `mise exec -- actionlint -pyflakes=`, `mise run test`
  and `mise exec -- openspec validate docs --strict` exit 0; `git status --short` lists only the design's files and this change;
  `wc -w` holds the budgets, three overruns accepted on 2026-11-07.
  `Published` and `On GitHub` await the push of `develop`, which `github-pages` must admit.

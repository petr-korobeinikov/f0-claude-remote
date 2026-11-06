# Tasks

No task changes business logic, so none opens with a failing test: the site's build is the test, per the design.
Nothing touches the device.

## 1. Tree

- [ ] 1.1 Check that `openspec/specs/apps-catalog/spec.md` exists, as `release`'s archive leaves it,
  and that its `The description says what the app is` differs from this change's delta only in the links;
  else stop and raise it. Check: `diff` of the two requirement blocks shows only those lines.
- [ ] 1.2 Copy `package.json` and `package-lock.json` from `f0-claude-remote-working-files/docs-audit/` into `docs/.vitepress/`,
  write `.npmrc`, add `"!docs"` to `application.fam`'s sources and the ignores to `.gitignore`, per the design.
  Check: the lock's `shasum -a 256` starts `cfda831f9b36`; `mise exec -- prek run --all-files` exits 0.

## 2. Site

- [ ] 2.1 Write `config.mts`, the tasks `docs-build` and `docs-dev`, and Home per `user-guide`,
  and add both tasks to `CONTRIBUTING.md`'s list.
  Check: `docs-site`'s `Build` holds; a temporary link to `./missing.md` fails as `Dead link` says;
  `Live edit` and `Loopback only` hold, with the screenshot on Home.
- [ ] 2.2 Run `mise run build` and `mise exec -- ufbt lint` after `mise run docs-build`.
  Check: `docs-site`'s `Build and lint` holds; `git status --short --untracked-files=all docs/.vitepress` lists only this change's four files.
- [ ] 2.3 Write Quick start and the Guide's five pages per `user-guide`, from the design's sources only.
  Check: each `user-guide` scenario by reading; `Sidebar` and `Index` on the build;
  in `docs-dev`'s site the owner sees the scheme follow the system and the switch, and search offer Pairing.
- [ ] 2.4 Move the README's Usage out and link the site per `user-guide`; point `catalog/description.md` at the site per this change's `apps-catalog`.
  Check: `README` and `Content` hold, so do `apps-catalog`'s `No refused construct` and `Hooks pass`;
  `rg -n '#pairing|#keys-on-the-mac'` finds nothing outside `openspec/`.

## 3. Workflows

- [ ] 3.1 Add the `docs` job to `ci.yml` and write `docs.yml` per the design; add the job to `CONTRIBUTING.md`'s `## CI`.
  Check: `mise exec -- actionlint -shellcheck= -pyflakes=` exits 0; `Workflow`, `Commands` and `Pinned references` hold by reading;
  `mise run check-workflow` meets `Local outcome`.

## 4. The owner's setup

- [ ] 4.1 The owner, not the session, sets Pages' source to GitHub Actions:
  `gh api -X POST repos/petr-korobeinikov/f0-claude-remote/pages -f build_type=workflow`.
  Check, by reads alone: `gh api repos/petr-korobeinikov/f0-claude-remote/pages --jq .build_type` prints `workflow`.

## 5. Close

- [ ] 5.1 Check: `mise exec -- prek run --all-files`, `mise exec -- actionlint -shellcheck= -pyflakes=`, `mise run test`
  and `mise exec -- openspec validate docs --strict` exit 0; `git status --short` lists only the design's files and this change;
  `wc -w` holds the budgets. `Published` awaits the owner's push of `develop`,
  after which the `github-pages` environment's branch policies must list `develop`.

# Tasks

Tasks 1.2 and 1.3 act on GitHub, each on the owner's yes;
the rest change files alone.

## 1. The site

- [x] 1.1 Set `.github/workflows/docs.yml:5` to `branches: [main]` under the design's comment, and put the design's lines in `docs/contributing/hooks-and-ci.md:58-59` and after `docs/contributing/release.md`'s `in one atomic push.`. Check: `docs-deploy`'s `Workflow` holds; `mise exec -- actionlint -pyflakes=` and `mise run docs-build` exit 0; `git grep -n 'docs\.yml' -- docs` lists those two new passages alone.
- [x] 1.2 Read `github-pages`' deployment branches with `gh api`; if they refuse `main`, add a branch rule for it. Check: `gh api` shows no rule, or one admitting `main`.

  Refused `main`: one custom policy, `develop`. Added `main` on the owner's yes (policy 66465594); `develop`'s stays.
- [x] 1.3 Run `gh workflow run docs.yml --ref main` and watch the run end. Check: `docs-deploy`'s `Published` at `main`; the run's head is `git rev-parse main`; `https://petr-korobeinikov.github.io/f0-claude-remote/development/host-tests/index.md` names `transport_fake_on_usb`.

  Run 43240987304 on the owner's yes: `workflow_dispatch`, head `4a5bf24` (`main`, `v0.2`), build and deploy success.
  `/`, `llms.txt`, `guide/pairing/index.md` answer 200; `host-tests/index.md` names `transport_fake_on_usb`. The plan's `guide/usb/` 404 check assumed `main` at `v0.1`; fixed in place.

## 2. The Purpose

- [x] 2.1 Set `openspec/specs/docs-deploy/spec.md`'s Purpose to the design's. Check: `git diff` changes the Purpose alone; `mise exec -- openspec validate docs-deploy --type spec --strict` exits 0.

## 3. The docs against the tree

- [x] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

  None false: `hooks-and-ci.md:37`'s `ci.yml` runs on `develop` and `main` as before; the `blob/develop/` links in `docs/internals/` and `workflow.md:64` name files `develop` holds; `README.md` and `SECURITY.md` keep showing `develop`, as the proposal settles.
  True again: `docs/guide/installation.md:19-20`'s latest Release is the Release whose pages the site shows.

## 4. Close

- [x] 4.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate docs-from-main --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

  Both exit 0; `git status --short` lists `docs.yml`, the two contributor pages, `docs-deploy`'s spec and this change's three artifacts.
  `wc -w`: proposal 468, design 538, spec 125, tasks 342 before this note.

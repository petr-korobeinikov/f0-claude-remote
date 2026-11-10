# Tasks

Tasks 1.2 and 1.3 act on GitHub, each on the owner's yes;
the rest change files alone.

## 1. The site

- [ ] 1.1 Set `.github/workflows/docs.yml:5` to `branches: [main]` under the design's comment, and put the design's lines in `docs/contributing/hooks-and-ci.md:58-59` and after `docs/contributing/release.md`'s `in one atomic push.`. Check: `docs-deploy`'s `Workflow` holds; `mise exec -- actionlint -pyflakes=` and `mise run docs-build` exit 0; `git grep -n 'docs\.yml' -- docs` lists those two new passages alone.
- [ ] 1.2 Read `github-pages`' deployment branches with `gh api`; if they refuse `main`, add a branch rule for it. Check: `gh api` shows no rule, or one admitting `main`.
- [ ] 1.3 Run `gh workflow run docs.yml --ref main` and watch the run end. Check: `docs-deploy`'s `Published` at `main`; `https://petr-korobeinikov.github.io/f0-claude-remote/guide/usb/` answers 404.

## 2. The Purpose

- [ ] 2.1 Set `openspec/specs/docs-deploy/spec.md`'s Purpose to the design's. Check: `git diff` changes the Purpose alone; `mise exec -- openspec validate docs-deploy --type spec --strict` exits 0.

## 3. The docs against the tree

- [ ] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 4. Close

- [ ] 4.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate docs-from-main --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

# Tasks

No task changes business logic, so none opens with a failing test:
each check is a command, a read of a file or a read-only `gh` call.
Nothing touches the device.
No task tags, pushes or writes to GitHub: a draft and `publish` first run at `v0.1`, per the design.

## 1. Draft first

- [ ] 1.1 Rename `release.yml`'s `release` job to `draft`, open its last step with the guard, add `--draft`, point `flipper-apps-catalog` at it and add `publish`, per the design. Check: `mise exec -- actionlint -pyflakes=` exits 0; read off the file, `release`'s `Triggers`, `Published last` and `Read off the workflow` hold, and so does the workflow half of `flipper-apps-catalog-submission`'s `Environment`.
- [ ] 1.2 Run the guard's `if` alone, never the `gh release create` after it, through the owner's `gh` in this tree: with `GITHUB_REF_NAME=v0.1`, then with `GH_REPO=cli/cli` and `GITHUB_REF_NAME=v2.103.3`. Check: the first exits 0 and prints nothing; the second prints the `::error::` line naming `v2.103.3` and exits 1, so `A Release exists` holds.
- [ ] 1.3 Rewrite `docs/contributing/release.md`'s sentence on what the tag runs and its paragraph on a failed run, per the design. Check: its job names and commands match `release.yml`; `mise exec -- prek run --files docs/contributing/release.md` and `mise run docs-build` exit 0.

## 2. Close

- [ ] 2.1 Check: `mise exec -- prek run --all-files`, `mise exec -- actionlint -pyflakes=`, `mise run test` and `mise exec -- openspec validate release-draft --strict` exit 0; `git status --short` lists only the design's files and this change; `wc -w` holds the budgets. `On GitHub`, `First submission` and `Manifest` await `v0.1`, `Next submission` `v0.2`.

# Tasks

No task changes business logic, so none opens with a failing test:
each check is a command, a read of a file or an anonymous GET.
Nothing touches the device.
No task pushes, tags, opens a pull request or runs `act`, and none holds a GitHub credential, per the design's test seam.
The commit waits for the owner's review in CLion, a push for the owner's word.
Every scenario closes here, per the design's table; none waits for a release.

## 1. The step

- [ ] 1.1 With `/usr/bin/curl -q -fsS` and no token, GET the fork's `git/matching-refs/heads/petr-korobeinikov/f0_claude_remote_`, upstream's `pulls/1387`, and upstream's `pulls?state=open&head=petr-korobeinikov:<branch>` for each branch listed. Check: #1387's `state` is `open`; `jq length` gives `1` for `petr-korobeinikov/f0_claude_remote_0.1` and `0` for `petr-korobeinikov/f0_claude_remote_0.2`; otherwise stop for a replan.
- [ ] 1.2 Change the step per the design. Check: `mise exec -- actionlint -pyflakes=` exits 0; read off the step, a ref is deleted only when its read prints `0`, and the read is assigned before the test; its one `pulls` call has no `-f`, `-F`, `--input` or `--method`.
- [ ] 1.3 Build the design's dry run. Check: `/usr/bin/curl -q -fsS https://api.github.com` exits 0 outside its sandbox and non-zero inside.
- [ ] 1.4 Dry-run the step as `v0.3`, the fake giving task 1.1's refs and counts and failing the manifest's read on `main`. Check: the log holds one DELETE, of `petr-korobeinikov/f0_claude_remote_0.2`, the create of `petr-korobeinikov/f0_claude_remote_0.3`, a PUT reading `Add Claude Remote 0.3`, and `fork-token` on the fork's calls alone; `manifest.yml` holds `Manifest`'s fields.
- [ ] 1.5 Dry-run it again, the fake answering the manifest's read with a blob `sha`. Check: the PUT reads `Update Claude Remote to 0.3` and carries that `sha`.
- [ ] 1.6 Rewrite `docs/contributing/release.md:42` per the design. Check: `mise exec -- prek run --files docs/contributing/release.md` and `mise run docs-build` exit 0; the page's sentence on the `flipper-apps-catalog` job matches the step.

## 2. Close

- [ ] 2.1 Check: `mise exec -- prek run --all-files`, `mise exec -- actionlint -pyflakes=`, `mise run test`, `mise run docs-build` and `mise exec -- openspec validate flipper-apps-catalog-keep-branch --strict` exit 0; `git status --short` lists only the design's files and this change; `wc -w` holds the budgets.

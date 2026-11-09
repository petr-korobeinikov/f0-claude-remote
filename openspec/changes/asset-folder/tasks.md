# Tasks

## 1. Baseline

- [ ] 1.1 Before any move, `rm -rf .ufbt/build dist` and `mise run build`; in a `mktemp -d`, save the `.fap`, and `f0_claude_remote_icons.c`, `f0_claude_remote_icons.h` and `.fapmeta` from `.ufbt/build/f0_claude_remote/`. Check: the `.fap`'s `sha256` and the count of `mise run test`'s `ok` lines stand in this task's notes.

## 2. The move

- [ ] 2.1 `mkdir -p asset/icon`, `git mv f0_claude_remote.png asset/icon/`, `git mv images asset/image`, then set `fap_icon` and `fap_icon_assets` per the design. Check: after a clean build, the `.fap`'s `sha256` equals 1.1's, the three generated files `cmp` equal, and `mise exec -- ufbt lint` exits 0.
- [ ] 2.2 `git mv screenshots asset/screenshot`, `ln -sfn ../../asset/screenshot/main.png docs/assets/main.png`, then update `README.md` and `release.yml`'s manifest. Check: `mise run docs-build` and `mise exec -- actionlint -pyflakes=` exit 0, and `cmp .site/assets/main.png asset/screenshot/main.png` passes.
- [ ] 2.3 Take `asset/image/up.png` out and `mise run build`, then restore it; the same with `asset/icon/f0_claude_remote.png`. Check: each build exits non-zero, naming `I_up` and the file; after both, a clean build's `sha256` equals 1.1's.

## 3. The rule

- [ ] 3.1 Add the rule's text from the design to `openspec/config.yaml`. Check: `mise exec -- openspec validate asset-folder --strict` exits 0, and `git ls-files | grep -vE '^(docs|openspec|\.github|\.mise|\.claude)/' | xargs -n1 dirname | sort -u` lists no plural folder.

## 4. The proof of a rename

- [ ] 4.1 Run the suites and the hooks. Check: `mise run test` exits 0 with 1.1's count of `ok` lines, and `mise exec -- prek run --all-files` exits 0.
- [ ] 4.2 Read the index. Check: `git diff --cached -M --name-status | grep -c '^R100'` prints 15, `readlink docs/assets/main.png` prints `../../asset/screenshot/main.png`, and the `Paths` and `The icon` scenarios of `app-build` hold.
- [ ] 4.3 Search for the old names. Check: `git grep -nE '(^|[^/])screenshots/|(^|[^/a-z_])images/|"images"|(^|[^/])f0_claude_remote\.png' -- ':!openspec/changes' ':!openspec/specs'` prints nothing.

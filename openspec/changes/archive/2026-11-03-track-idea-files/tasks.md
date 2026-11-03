# Tasks

## 1. Tracked files

- [x] 1.1 Change `.gitignore` per the design; `git mv -f` both files from `ide/` to `.idea/`. Check: `git ls-files .idea` prints exactly the two; `git check-ignore .idea/workspace.xml .idea/vcs.xml .idea/.gitignore` prints all three; `ide/` is gone.
  `git mv` left an empty `ide/` behind in the working checkout; removed with `rmdir`.
- [x] 1.2 Check the final newline of both files: `tail -c2 <file> | xxd` shows one `0a`, preceded by `>`.
- [x] 1.3 Delete `.mise/tasks/setup-ide` and `setup`'s `depends` line. Check: `rg -n 'setup-ide|ide/' --hidden -g '!openspec/changes' -g '!.git' .` finds only the `README.md` bullet that 1.4 removes.
- [x] 1.4 Remove that `README.md` bullet; add the sentence per the design. Check: `rg` from 1.3 finds nothing.

## 2. Clean-clone acceptance

- [x] 2.1 Commit to the branch (collapsed at finish); clone into a `mktemp -d` directory; run `mise trust`, `mise install`, `mise run setup`. Check: all exit 0; `git status --short` empty; `git ls-files .idea` prints the two.
- [x] 2.2 Run `clion .` there; wait for the import; close CLion. Check: `git status --short` empty. If `misc.xml` shows, record the diff, stop, and raise the design's closures with the owner.
  First open and close kept `misc.xml`'s final newline (`3e0a`); no closure needed.
- [x] 2.3 Reopen it. Observe: navigation reaches `sdk_headers`; `Build`, `Launch`, `Debug` and profile `Flipper` listed; `Build` exits 0.
- [x] 2.4 Select `Flipper`; close CLion. Check: `git status --short` empty, `Flipper` still selected on reopen; `git check-ignore` covers everything CLion created under `.idea/`.
  Two closes in the clone, `misc.xml` kept its newline each time; the owner confirmed `Flipper` on reopen.

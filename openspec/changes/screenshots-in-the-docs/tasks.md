# Tasks

`app-build`'s `Paths` and `docs-site`'s `Build` close in task 1.1,
`user-guide`'s `Screenshots` in 2.1, `Screenshot` in 2.2 and `Screens` in 3.1.
The other scenarios the deltas carry hold as written, on text the change leaves alone.
No task takes the device.

## 1. The folder link

- [ ] 1.1 Replace `docs/assets/main.png` with the link `docs/assets/screenshot` to `../../asset/screenshot`; point `docs/index.md` and `using-the-remote.md` at `assets/screenshot/main.png` (`proposal.md`, Why). Check: `mise run docs-build` exits 0 and `cmp` finds each `asset/screenshot/` file equal in `.site/assets/screenshot/`; `Paths`' two commands; `mise run build` and `mise exec -- ufbt lint` exit 0.

## 2. The site

- [ ] 2.1 In `using-the-remote.md`, put `voice-bt.png`, `no-link-bt.png` with `no-link-usb.png`, `voice-cut-bt.png` and `stopped-bt.png` above their screens' paragraphs, alt text per the proposal. Check: `mise run docs-build` exits 0; `grep -o 'screenshot/[a-z-]*\.png' .site/guide/using-the-remote/index.html` prints the scenario's six in order.
- [ ] 2.2 In `usb.md`, put `main-usb.png` above the paragraph on the status row's USB mark. Check: the same `grep` on `.site/guide/usb/index.html` prints `screenshot/main-usb.png` alone.

## 3. The README

- [ ] 3.1 Add `## Screens` after `## Features`: an HTML table of the proposal's three pairs, each `<img>` `width="256"`. Check: `grep -o 'asset/screenshot/[a-z-]*\.png' README.md` prints `main.png`, then the six in the scenario's order; CLion's Markdown preview shows three rows of two.

## 4. Close

- [ ] 4.1 Archive the change. Check: `mise run docs-build`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate --specs --strict` exit 0; `git status --short` lists the proposal's Impact and this change's files alone; `wc -w` holds the budgets.

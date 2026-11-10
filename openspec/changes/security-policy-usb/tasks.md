# Tasks

No task takes the device or the owner's hands:
`usb-link`'s `Name` stands on usb-transport's task 6.1, which saw the Mac list `Claude Remote` on 046D:C529,
and the code under it stays as it is.

## 1. The policy

- [ ] 1.1 Replace `SECURITY.md:21-22` with the design's seven lines. Check: `git diff` changes those two lines alone; the section meets `security-policy`'s two scenarios; `src/platform/transport_usb.c:14-16` sets that name and those IDs; `mise run docs-build` exits 0 and `.site/guide/usb/index.html` holds `id="what-the-mac-asks"`; `mise exec -- prek run --files SECURITY.md` exits 0.

## 2. The docs against the tree

- [ ] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

## 3. Close

- [ ] 3.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate security-policy-usb --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

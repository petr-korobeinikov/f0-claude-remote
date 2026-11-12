# Tasks

No task takes the device or the owner's hands:
`usb-link`'s `Name` stands on usb-transport's task 6.1, which saw the Mac list `Claude Remote` on 046D:C529,
and the code under it stays as it is.

## 1. The policy

- [x] 1.1 Replace `SECURITY.md:21-22` with the design's seven lines. Check: `git diff` changes those two lines alone; the section meets `security-policy`'s two scenarios; `src/platform/transport_usb.c:14-16` sets that name and those IDs; `mise run docs-build` exits 0 and `.site/guide/usb/index.html` holds `id="what-the-mac-asks"`; `mise exec -- prek run --files SECURITY.md` exits 0.
  Every check passed; the built page holds the anchor as planned.

## 2. The docs against the tree

- [x] 2.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.
  None: `README.md:29-30` and `docs/index.md:39-42` speak of the links, not of trust;
  `docs/internals/usb.md:43-47` and `docs/guide/usb.md:22-28` agree with the new lines.

## 3. Close

- [x] 3.1 Check: `mise exec -- prek run --all-files`; `mise exec -- openspec validate security-policy-usb --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

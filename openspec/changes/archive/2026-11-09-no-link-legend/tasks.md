# Tasks

## 1. The legend

- [x] 1.1 Add `view_draw_hold_legend` per the design. Check: `mise run test`, `mise run build` and `mise exec -- ufbt lint` exit 0; `git grep -n view_draw_hold_legend -- src` finds its definition and one call, under `ModeNoLink`.
- [x] 1.2 In `docs/guide/using-the-remote.md`, name `HOLD` in the top row in the `No link` paragraph. Check: `mise run docs-build` exits 0.

## 2. With the owner

- [x] 2.1 With the Mac's Bluetooth off, `mise run launch`, then the cable out. The owner looks at `No link`, holds Right, and looks at USB's `No link`. Observe: `The legend`, `The legend on USB`.

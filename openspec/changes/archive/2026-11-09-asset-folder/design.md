# Design

## Context

See `proposal.md`, Why.
No layer of the app changes and no source file is added:
the move lands in the repository's layout and `application.fam`.
The images are compiled into the `.fap`, so the device reads no PNG,
and a folder's name can reach it only through the build.

| Fact | Source |
|---|---|
| `fap_icon` is embedded in the `.fap`; `fap_icon_assets` names a folder of images "built alongside the app" | Docs: `documentation/AppManifests.md:52,58`, firmware 1.5.1 |
| Both paths resolve from the app's folder, so a subfolder works | Sources alone: `scripts/fbt/elfmanifest.py:55`, `scripts/fbt_tools/fbt_extapps.py:91,243` |
| That folder is gathered recursively | Sources alone: `scripts/fbt_tools/fbt_assets.py:12` |
| The generated `f0_claude_remote_icons.c` holds names and pixels, sorted, and no path | Sources alone: `scripts/assets.py:118` |
| The Flipper Apps Catalog joins `fap_icon` and each screenshot to the repository's root | Their `tools/bundle.py:308,320` |
| `ufbt lint` takes all four new folder names | `scripts/lint.py:13` |

`dist/f0_claude_remote.fap` and its debug ELF hold neither `images` nor `screenshots` as a path today (`strings`).

## Decisions

- **`fap_icon_assets="asset/image"`, not `"asset"`:**
  gathering is recursive, so `"asset"` would build the screenshot and the icon into the package as `I_main` and `I_f0_claude_remote`.
- **The symlink stays, retargeted to `../../asset/screenshot/main.png`, not a copy:**
  Zensical publishes only from `docs/` and writes the link's target as a regular file, and a copy could drift.
- **The proof is the package's bytes, not a look at the device:**
  an equal `sha256` means every board draws what it drew, and looking at the boards would prove the same bytes again.
  No device check.
- **Two clean builds (`rm -rf .ufbt/build dist`), not an incremental one:**
  SCons decides by content, so an incremental build may leave the old `.fap` in place and prove nothing.
- **The rule goes to `openspec/config.yaml`, not a spec:**
  it binds the names of future folders, and no behavior.
  Its text, under Engineering, after the paragraph on `src/`:

  > The app's pictures live in `asset/`, one folder per purpose: `asset/icon/` for the Apps menu's icon,
  > `asset/image/` for what the screens draw and `asset/screenshot/` for the screenshots.
  > Folder names are in the singular, but for `docs/` with the folders under it,
  > and for a name a tool fixes, as `.github/workflows/`.

## Testing

No component to test: the seam is the build's output, the `sha256` of the `.fap` and the generated header.

## Risks / Trade-offs

- The clean builds' `sha256` differ → `cmp` the generated `f0_claude_remote_icons.c` and `.fapmeta` with the baseline's; report the result and stop.
- `docs/assets/main.png` dangles, which `git grep` cannot see → `readlink` and `cmp` against `.site/assets/main.png` in the tasks.
- `_0.1`'s manifest names `screenshots/main.png` → it holds: their bundler reads it at `v0.1`'s `commit_sha`, where the file stands.
- An SDK bump changes a fact read from sources → the build fails, by `A missing picture stops the build`.

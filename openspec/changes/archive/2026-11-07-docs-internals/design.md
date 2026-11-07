# Design

## Context

See `proposal.md` for why.
No app layer is touched: the pages read the code, never change it.

Each page's sources:

| Page | Its sources |
|---|---|
| Architecture | `source-layers`, `host-tests`, `button-actions`, `app-screen`; archived `src-layers`, `app-keymap`, `screen-redesign`, `ble-transport`; `src/` |
| Bluetooth | `bluetooth-link`; archived `ble-transport`: its Documented and Sources lists, its task notes |
| Keys in Claude Code | `voice-control`, `effort-control`, `prompt-control`; archived `effort-actions`, `remote-actions`: their Documented, Sources and Seen lists |

## Goals / Non-Goals

**Goals:** the specs'.

**Non-Goals:** facts no archived change or spec records; a new check of any fact on the device or the Mac.

## Decisions

| File | Its one task |
|---|---|
| `docs/internals/architecture.md` | Shows the layers and a press's path |
| `docs/internals/bluetooth.md` | Explains the app's own profile |
| `docs/internals/keys-in-claude-code.md` | Explains the chords |
| `zensical.toml` | Lists Internals in `nav` and in a `llmstxt` section |

**A mark** ends the sentence that states a fact: its kind in words, then the link.
A firmware link pins the tag and the line, as
`https://github.com/flipperdevices/flipperzero-firmware/blob/1.5.1/targets/f7/ble_glue/gap.c#L143`.
A fact read in Claude Code's package links the archived change that read it, since the package publishes no browsable source.
A seen fact links the archived `tasks.md` that records it.

- **Three kinds, as the archive sorts them,** not one flat list of links:
  a reader bumping the firmware or Claude Code rereads only the second kind.
- **The path as a list:** each file with its thread, the input queue from the GUI thread, the link queue from the Bt thread;
  no Mermaid plugin to audit.
- **Wording from the archive:** the hygiene pass of 2026-11-05 cleaned it, so a page copying its terms carries no private value.

**Test seam:** the build fails a dead internal link;
a script checks that each `git ls-files src` path appears once on Architecture;
`curl -fsSI` checks each external link answers 200.

## Risks / Trade-offs

- `docs-development` lands changed → task 1.1 rebases the sidebar delta on the archived spec.
- An SDK bump moves the firmware's lines → links pin `1.5.1`, so they stay true to it; the bump's change rereads Bluetooth's source facts.
- A Claude Code update changes a package fact → Keys names 2.1.311, and the next tried version rereads that kind.

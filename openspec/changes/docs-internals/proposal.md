# Proposal

## Why

The Guide and Development say what to do; nothing on the site says why the app works as it does.
The reasons sit in seven archived changes:
three layers and a press's path to the HID report, the app's own Bluetooth profile, every command a Ctrl chord.
Three of them sort their facts into what Flipper's or Claude Code's documentation states,
what only firmware 1.5.1's or Claude Code 2.1.311's sources show, and what was seen on the device or the Mac.
The second kind breaks with a bump, and only the archive tells the kinds apart.

Settled by the owner on 2026-11-06:

- **Internals, the third of three changes:** Architecture, Bluetooth and Keys in Claude Code,
  between Development and Contributing.

Proposed here, for the owner to strike:

- **Each fact names its kind and links its source:**
  documented, with the page; firmware sources, with the file and line at tag `1.5.1`;
  Claude Code 2.1.311's package, with the archived change that read it;
  seen, with the archived task that saw it.
- **Pages draw on tracked files and the public sources they cite,** never on notes outside the repository.
- **A press's path is a list of files,** not a diagram:
  Mermaid needs a plugin and its audit, and a list reads alike light and dark.
- **After `docs-development`:** this change edits the sidebar that one extends.

## What Changes

- New: `architecture.md`, `bluetooth.md` and `keys-in-claude-code.md` in `docs/internals/`.
- `config.mts`'s sidebar and nav follow.

## Capabilities

### New Capabilities

- `internals-guide`: the Internals pages, and the kind and source each of their facts carries.

### Modified Capabilities

- `docs-site`: the sidebar adds Internals between Development and Contributing.

## Impact

No dependency. The pages link out to Flipper's firmware at tag `1.5.1` and to Claude Code's documentation.

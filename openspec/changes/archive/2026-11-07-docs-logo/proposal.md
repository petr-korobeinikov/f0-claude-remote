# Proposal

## Why

The site's header shows Zensical's default logo, `lucide/book-open`,
while the project has its own: the app icon `f0_claude_remote.png`, 10×10 at 1 bit,
drawn in Claude Design as `AppIcon`'s variant B, the command's slash and the waves.

Settled by the owner on 2026-11-07:

- **The project's icon replaces the book;** plan, implementation, acceptance and archive in one session and one commit.

Proposed here, for the owner to strike:

- **Variant B, as the app ships it,** not variant A, which Claude Design recommended: the docs show the icon a reader sees on the Flipper.
- **The favicon stays Zensical's:** the request names the logo alone.

## What Changes

- New: `docs/assets/logo.svg`, the icon's pixels.
- `zensical.toml` sets `theme.logo` to it.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

- `docs-site`: the header shows the app's icon.

## Impact

No dependency, no app code.

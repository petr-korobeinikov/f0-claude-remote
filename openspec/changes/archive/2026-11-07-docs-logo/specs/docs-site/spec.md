## ADDED Requirements

### Requirement: The header shows the app's icon
The site's header SHALL show the app icon `f0_claude_remote.png` as its logo,
its black pixels black on Flipper orange `#FF8200`, inside a 1-pixel orange margin,
sharp at any size the header draws it.

#### Scenario: Pixels
- **WHEN** `mise run docs-build` has run, and `.site/assets/logo.svg` and `f0_claude_remote.png` are decoded
- **THEN** the SVG's black squares, less the margin, are the icon's black pixels, and the header's `<img>` points at it

#### Scenario: Header
- **WHEN** a page of `docs-dev`'s site opens, light and then dark
- **THEN** the header shows the icon in place of the book, sharp and whole in both

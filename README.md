<div align="center">

<p align="center">
  <img src="screenshots/main.png" alt="The app's main screen on the device">
</p>

# F0 Claude Remote

*Your voice runs Claude Code. Your Flipper Zero runs the shortcuts.*

**F0 Claude Remote** is a Flipper Zero app
that acts as a Bluetooth remote for Claude Code.

[Documentation](https://petr-korobeinikov.github.io/f0-claude-remote/) ·
[Quick start](https://petr-korobeinikov.github.io/f0-claude-remote/quick-start/) ·
[Debugging](https://petr-korobeinikov.github.io/f0-claude-remote/development/debugging/)

</div>

## Why

Claude Code takes dictation,
but a session still asks for the keyboard:
to send the prompt, stop a turn, pick from a menu,
clear the conversation or change the effort.
This app puts all of that on the Flipper's buttons,
so voice and one hand run the whole session, by feel.

## Features

- **Dictation** —
  hold OK and speak into the prompt;
  nothing is sent until you tap OK.
- **The prompt** —
  Enter, the arrows for history and menus, Esc to stop a turn.
- **Commands** —
  hold a button for `/clear` or the effort level,
  whatever the Mac's keyboard layout, and the draft stays.
- **Feedback** —
  each action confirms itself with a buzz,
  so no buzz means nothing was sent.
- **Bluetooth** —
  to the computer it is a keyboard, paired once,
  that reconnects by itself within a second of a drop.
- **Debugging from CLion** —
  breakpoints in the app's sources on the device, with no Flipper plugin.

## Installation

You need a Flipper Zero on firmware 1.5.1,
and an Apple Silicon Mac with [mise](https://mise.jdx.dev) to build on.
With the device unlocked on USB, on its desktop:

```shell
mise trust
mise install
mise run setup
mise run launch
```

The app then lives under Apps → Bluetooth on the device.
To pair it and bind the keys, follow the
[Quick start](https://petr-korobeinikov.github.io/f0-claude-remote/quick-start/).

Tried only on a Mac.
Nothing in the app itself is specific to macOS,
so other systems should work as long as they accept a Bluetooth keyboard,
but `mise run setup` and the CLion debug profile are macOS-only.

## Contributing

The toolchain, the workflow and the hooks are on the site, from
[Contributing](https://petr-korobeinikov.github.io/f0-claude-remote/contributing/workflow/).

## License

[GPL-3.0](LICENSE). Copyright (C) 2026 Petr Korobeinikov.

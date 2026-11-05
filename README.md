<div align="center">

<p align="center">
  <img src="screenshots/main.png" alt="The app's main screen on the device">
</p>

# F0 Claude Remote

*Your voice runs Claude Code. Your Flipper Zero runs the shortcuts.*

**F0 Claude Remote** is a Flipper Zero app
that acts as a Bluetooth remote for Claude Code.

[Installation](#installation) ·
[Usage](#usage) ·
[Debugging](#debugging)

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

Tried only on a Mac.
Nothing in the app itself is specific to macOS,
so other systems should work as long as they accept a Bluetooth keyboard,
but `mise run setup` and the CLion debug profile are macOS-only.

## Usage

Pair the device once, and bind the keys.

### Pairing

Once per Mac, with the device on its desktop:

1. Run `mise run launch`.
   Not over the device's open Apps browser:
   the pairing dialog waits behind it, and closing the app hangs until the browser ends.
2. In the Mac's Bluetooth settings, press Connect on `Claude Remote` under Nearby Devices,
   and accept the request.
   This takes up to about 30 seconds once the device has advertised for a minute;
   press it again if it times out.
3. Press OK in `Verify code` on the device.
4. Close Keyboard Setup Assistant if it opens: the device has no keys for it.

From then on the Mac connects by itself, within a second of a drop.
To pair again, forget the device on the Mac first.
While the app is open it holds the radio, and on leaving it hands the radio back.

### Keys on the Mac

The device is a Bluetooth keyboard,
so a press acts on whatever the Mac has focused,
meant to be a Claude Code session:

| Button | Sends | In Claude Code |
|---|---|---|
| OK, tap | Enter | Sends the prompt, or picks a menu's option |
| OK, hold | Ctrl+Q, held until the release | Dictates into the prompt, sending nothing |
| Up, tap | Up arrow | Recalls the previous message, or moves up a menu |
| Down, tap | Down arrow | Recalls the next message, or moves down a menu |
| Right, press | Esc | Stops the turn |
| Up, hold | Ctrl+X, Ctrl+F | Sets effort to `high` |
| Down, hold | Ctrl+X, Ctrl+X | Sets effort to `max` |
| Left, hold | Ctrl+X, Ctrl+L | Runs `/clear` |
| Back, hold | Nothing | Leaves the app on the device |

Dictation, effort and `/clear` take one block in `keybindings.json`,
in Claude Code's config directory: `~/.claude`, or what `CLAUDE_CONFIG_DIR` names.
It goes into the file's `bindings` array, or its lines into a `Chat` block already there,
and the file reloads by itself:

```json
{
  "context": "Chat",
  "bindings": {
    "ctrl+q": "voice:pushToTalk",
    "ctrl+x ctrl+f": "command:effort high",
    "ctrl+x ctrl+l": "command:clear",
    "ctrl+x ctrl+x": "command:effort max"
  }
}
```

Tried on Claude Code 2.1.311.
Its schema has no space in a command's name,
so an editor validating against `$schema` marks the two effort lines;
they run all the same.

Ctrl+Q dictates from the Mac's own keyboard too,
since `voice:pushToTalk` takes one key.
Dictation also needs `/voice hold`, once,
and key repeat on, as macOS has it by default:
Claude Code tells a held key by its repeats,
and without them a recording ends about 2.2 s into the hold.
After the release, wait out `Voice: processing…` before tapping OK:
a tap during it sends the interim text and drops the rest.

A stray `/clear` costs one `/resume`,
which lists the previous conversation.

## Debugging

The app runs on the device, and GDB reaches it through the Black Magic probe on the WiFi Devboard.
Only our code is stepped through:
the SDK ships no firmware sources, so firmware calls are stepped over.

### Before a session

The device must run the firmware the SDK pins,
have Settings → System → Debug on, which otherwise disables its debug port,
and stay unlocked:
a PIN lock drops its USB console, and `mise run launch` fails.
Turn auto-lock off in Settings → Desktop for the session.

Attach the board in this order:

1. Pull the board's cable and power the device off.
2. Seat the board on the GPIO header.
3. Power the device on by holding Back, and enter the PIN.
4. Plug the board's cable, then the device's cable, into the Mac.

If the device will not power on,
take the board off, pull its cable, and hold Back for 30 seconds.

Take the board off and pull its cable after the session.
Its GDB port and its HTTP API have no password,
and `/api/v1/wifi/get_credentials` hands out the password of the network it joined.

### In CLion

1. Pick `Flipper` in the Debug Profiles list on the toolbar.
   CLion lists its own profiles first, so a clone needs this once.
2. Set a breakpoint in our sources.
3. Run the `Debug` configuration.
4. Run the `Launch` configuration.
   CLion stops in disassembly at `flipper_application_alloc_thread`:
   the app is loaded and its symbols are in.
5. Press Resume. CLion stops on the breakpoint.
6. Press Stop when done.
   The red `exit code 137 (interrupted by signal 9:SIGKILL)` is CLion ending GDB after it detached,
   and it is harmless.

### In a terminal

```shell
mise run debug    # GDB on the device; leave it with `quit`
mise run launch   # in another terminal
```

Leave with `quit` rather than killing the process:
this session sets the firmware's debug flag and clears it on the way out,
and with the flag left set the next app to start faults the device.

### The probe

A session finds the probe by itself,
over USB first and through `blackmagic.local` when the board has no cable.

Wi-Fi needs one cable fewer:
with the device on its own USB cable, the board runs from the GPIO header,
so skip the board's cable in the attach order and run the session as usual.

For Wi-Fi, put the board on a 2.4 GHz network once, from a phone so the Mac stays online:
join the board's `blackmagic` network, password `iamwitcher`,
open `http://192.168.4.1`, tab WiFi,
set Mode to `STA` with the network's SSID and Pass, press SAVE, then REBOOT.
Holding BOOT for 10 seconds restores the board's own network.

The board needs firmware 0.1.1:
older firmware hung starting USB, so neither USB nor GDB answered.
Flash the checked archive rather than `ufbt devboard_flash`,
which runs a command from an archive it never verifies.
Hold BOOT, press RESET, release BOOT, and the board appears as `ESP32_S2`, here on `/dev/cu.usbmodem01`:

```shell
dir=$(mktemp -d)
curl -fsS -o "$dir/fw.tgz" \
  https://update.flipperzero.one/builds/blackmagic-firmware/0.1.1/blackmagic-firmware-s2-full-0.1.1.tgz
echo "2e63724446004bd359d895d6843bb8f95cc374e27e3e37569b2e035930f845aa  $dir/fw.tgz" | shasum -a 256 -c
tar -xzf "$dir/fw.tgz" -C "$dir" ./bootloader.bin ./blackmagic.bin ./partition-table.bin
.ufbt/toolchain/arm64-darwin/bin/python3 -I -m esptool --chip esp32s2 --port /dev/cu.usbmodem01 \
  --before no_reset --after no_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB \
  0x1000 "$dir/bootloader.bin" 0x10000 "$dir/blackmagic.bin" 0x8000 "$dir/partition-table.bin"
```

Press RESET when it finishes.

## Contributing

The toolchain, the workflow and the hooks are in [CONTRIBUTING.md](CONTRIBUTING.md).

## License

[GPL-3.0](LICENSE). Copyright (C) 2026 Petr Korobeinikov.

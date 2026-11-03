# Proposal

## Why

The app builds, but nothing in the project starts it on the device or steps through its code.
`.run` alone cannot fix that on CLion 2026.2.4.1:
Remote Debug takes its debugger from a Debug Profile,
and it only says `target remote`, while the Black Magic probe needs `extended-remote` and an `attach`.
Scope: steps 8-9 of the untracked start brief, now that the toolchain is in.
Every mechanism here passed a dry run on the hardware in a scratch clone:
firmware 1.5.1, over USB and Wi-Fi, through GDB/MI, and in CLion over USB.

Decisions, settled with the owner on 2026-11-03:

- **Only our code is debugged,**
  not the firmware's:
  firmware calls are stepped over, and `firmware.elf` names their functions and lines,
  so firmware sources stay out.
- **CLion's debugger plus a terminal,**
  not the terminal alone:
  the owner wants breakpoints in the editor, on internal formats an update may break.
- **`setup` copies two files into an ignored `.idea/`,**
  not tracked files there:
  CLion writes them without the final newline the hook demands.

Decisions proposed here, for the owner to strike:

- **`launch` uploads and starts, with no `flash` task,**
  not the brief's four tasks:
  `ufbt flash` rewrites the firmware, and uploading without starting has no consumer.
- **One click selects the `Flipper` profile in each clone,**
  not a generated `workspace.xml`:
  CLion lists IDE-level profiles first, and `workspace.xml` is its private state.
- **The board stays off the device between sessions,**
  not always on:
  its GDB port and HTTP API have no authentication.
- **`README.md` carries the debugging steps,**
  at the owner's request.

## What Changes

- New tasks `build`, `launch`, `debug`; `setup` gains `setup-ide`.
- New `.run/` configurations, `debug/black_magic.gdb`, `ide/` templates.
- `.gitignore` ignores `.idea/`.
- `README.md` documents the device, the board and a debug session.

## Capabilities

### New Capabilities

- `device-debug`: debugs the app on the device, from CLion or a terminal.
- `ide-run-configurations`: gives a clone CLion's build, launch and debug with no setting touched.

### Modified Capabilities

- `app-build`: adds tasks that build the app and start it on the device.
- `ide-navigation`: the first open now finds the `.idea/` that setup wrote.

## Impact

No tool enters `mise.toml`; `esptool` and `pyserial` come with toolchain 39, already audited.
Audit, 2026-11-03, against OSV and the upstream repository:

| Dependency | Version | License | Source | Known vulnerabilities | Transitive artifacts |
|---|---|---|---|---|---|
| `blackmagic-esp32-s2`, board firmware | 0.1.1, 2023-04 | GPL-3.0 | Flipper server `2e637244`, equals its index; tag on GitHub | not indexed | 3 images, no host code |

Findings:

- **The board's old firmware hung starting USB:**
  its API predates 0.1.1, its task list matches 0.0.2, `main` never returned,
  and neither USB nor GDB answered.
  0.1.1 fixed both; the owner approved flashing this board.
- **`ufbt devboard_flash` stays out of the tasks:**
  `requests` 2.32.3 and `urllib3` 2.2.2 carry 10 advisories,
  none reachable through its two plain GETs short of a hostile Flipper server.
  Worse, `wifi_board.py` runs `flash.command` from an archive whose published SHA-256 it never checks;
  `README.md` flashes the checked archive with `esptool` instead.
- **The board exposes the device and the password of the network it joined:**
  `/api/v1/wifi/get_credentials` returns `sta_pass` unauthenticated, and its access point's default password is public.
  Station mode closes the access point; the Wi-Fi settings live on the board, not in the repository.
- **The firmware's debug flag can fault the device:**
  an app started while `furi_hal_debug_gdb_session_active` is set hits `bkpt` with no debugger.
  CLion's session never sets it; a BusFault seen once before that fix did not recur in later sessions.

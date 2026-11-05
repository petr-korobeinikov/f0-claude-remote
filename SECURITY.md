# Security Policy

## Supported Versions

Only the latest published release receives security fixes.

## Reporting a Vulnerability

Please report security vulnerabilities privately
through GitHub's [private vulnerability reporting][report].
Do not open a public issue for security problems.

You can expect an initial response within a few days.
This is a personal project,
so fixes are best-effort.

[report]: https://github.com/petr-korobeinikov/f0-claude-remote/security/advisories/new

## What the App Can Do

To the Mac the device is a Bluetooth keyboard,
paired once in the Mac's settings and confirmed on the device.
A press acts on whatever the Mac has focused.
The app has no lock yet,
so a stray tap on OK can send a draft or approve a permission prompt,
and whoever holds the device holds those keys.

## Debugging Hardware

The WiFi Devboard's GDB port and HTTP API have no authentication,
and its API hands out the password of the network it joined.
Take the board off the device after a session,
as the [README](README.md#before-a-session) says.

## Dependency Security Posture

The shipped artifact is a single `.fap`,
built against the SDK pinned in `mise.toml`.
The app has no third-party dependencies of its own.

The Flipper toolchain bundles Python packages with known advisories,
which cannot be patched here.
They run only on the developer's machine at build time,
over this repository's own files,
and reach neither the `.fap` nor the device.

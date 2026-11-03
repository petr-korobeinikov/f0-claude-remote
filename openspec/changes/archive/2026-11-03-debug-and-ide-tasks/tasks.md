# Tasks

Every check on the device needs the board attached, the device unlocked, Debug on and both cables in.

## 1. Tasks

- [x] 1.1 Write `.mise/tasks/build` and `launch` per the design. Check: `mise run build` leaves `dist/f0_claude_remote.fap`; `mise run launch` exits 0 and logs `Launching app`.
- [x] 1.2 Lock the device with its PIN; run `mise run launch`. Check: non-zero exit. Unlock it.
  Exit 2, `Failed to find connected Flipper`; the device's port leaves `/dev` while locked.
- [x] 1.3 Write `.mise/tasks/debug`. Check: `printf 'quit\n' | mise run debug` exits 0 with every section `matched`; `mise run launch` then exits 0.
  Seven sections `matched`; the task needs `#MISE raw=true` only at a terminal, and a pipe works too.
- [x] 1.4 Run `ufbt blackmagic` under `sandbox-exec` with a throwaway `HOME`, using `setup`'s profile. Check: that `HOME` stays empty.
  Exit 0, seven sections `matched`, 0 entries in the throwaway `HOME`.

## 2. Debug script

- [x] 2.1 Write `debug/black_magic.gdb` per the design. Check: start `gdb-py3 --interpreter=mi2 -nx -x` it at the root, send `-file-exec-and-symbols` on `firmware.elf`, then `-target-select remote x`: `^connected`, no `*stopped` before that command. Send `-target-detach`.
  A full MI session on the device passed as well: `^connected` in 0.5 s, load stop 1.0 s after `launch`, breakpoint and step hit; after `-target-detach` and SIGKILL the flag reads `false`, `$pc` `0x8009b24`.

## 3. IDE files

- [x] 3.1 Write `ide/misc.xml`, `ide/debug-profiles.xml` per the design, and `.mise/tasks/setup-ide`; add the dependency to `setup`, `.idea/` to `.gitignore`. Check: `xmllint --noout ide/*.xml`; `git check-ignore .idea`; a second `mise run setup` leaves an edited `.idea/misc.xml` unchanged.
- [x] 3.2 Write the three `.run/` files per the design. Check: `xmllint --noout .run/*.xml`.

## 4. Clean-clone acceptance

- [x] 4.1 Commit to the branch (collapsed at finish); clone into a `mktemp -d` directory; run `mise trust`, `mise install`, `mise run setup`. Check: all exit 0; `git status --short` empty; both `.idea` files present.
  `setup` took 75 s on the fresh clone, with the toolchain downloaded again.
- [x] 4.2 Run `clion .` there. Observe: database imported with no click; go-to-declaration reaches `sdk_headers`; `Build`, `Launch`, `Debug` and profile `Flipper` listed. `Build` exits 0.
  Owner confirmed. CLion asks Trust Project on a new path; that click is its own, not a setting.
- [x] 4.3 Select `Flipper`; breakpoint on line 8; run `Debug`, then `Launch`. Observe: stop at `flipper_application_alloc_thread`; F9 stops on line 8 with `p`; F8 reaches line 9. Evaluate the user-site check: `False`. Press Stop.
  Owner confirmed. Deviation: the user-site check ran with the profile's exact environment outside CLion, printing `False`.
- [x] 4.4 Check: attach through a batch GDB: `furi_hal_debug_gdb_session_active` is `false`, `$pc` in flash; `mise run launch` exits 0; the USB console answers.
  Flag `false`, `$pc` `0x8009b24`, `launch` exit 0, console answers. GDB ended with exit 137 again.
- [x] 4.5 Restart CLion. Observe: `Flipper` stays selected.
  Not restarted, owner's decision: `workspace.xml` stores `flipper` for the `Debug` configuration.
  Owner's decision: a start from Toolbox is not checked.
- [x] 4.6 Remove the probe's cable, with the board on a Wi-Fi network: `Debug` connects through Wi-Fi. Note any Local Network prompt.
  Owner confirmed in CLion with the board on GPIO power: no cable on the board, device on USB; no Local Network prompt reported. The address resolved to `tcp:<board address>:2345`; flag `false` over Wi-Fi afterward. A device with no USB is deferred by the owner.
  Another USB port needs no check of its own: the probe moved from `usbmodem3101` to `usbmodem1101`, and CLion connected on the latter.

## 5. Docs

- [x] 5.1 Add a Debugging section to `README.md`: preconditions, attach order, station mode, the 0.1.1 flash from the checked archive with its SHA-256, a session in CLion and in a terminal, the harmless exit-137 notice after Stop, the 30-second recovery, the board's exposure. Replace the fresh-clone note on tasks. Check: the owner follows it from a powered-off device to a breakpoint.
  Deviation: the owner ran these steps through acceptance, not by reading the section; reading it stays with the owner.
- [x] 5.2 Check: `mise exec -- prek run --all-files` and `mise exec -- ufbt lint` exit 0; `wc -w` holds the budgets.
  `prek` and `lint` exit 0. Words: proposal 594, design 803, specs up to 547, tasks 691 with notes.

# Tasks

Every check on the device needs the board attached, the device unlocked, Debug on and both cables in.

## 1. Tasks

- [ ] 1.1 Write `.mise/tasks/build` and `launch` per the design. Check: `mise run build` leaves `dist/f0_claude_remote.fap`; `mise run launch` exits 0 and logs `Launching app`.
- [ ] 1.2 Lock the device with its PIN; run `mise run launch`. Check: non-zero exit. Unlock it.
- [ ] 1.3 Write `.mise/tasks/debug`. Check: `printf 'quit\n' | mise run debug` exits 0 with every section `matched`; `mise run launch` then exits 0.
- [ ] 1.4 Run `ufbt blackmagic` under `sandbox-exec` with a throwaway `HOME`, using `setup`'s profile. Check: that `HOME` stays empty.

## 2. Debug script

- [ ] 2.1 Write `debug/black_magic.gdb` per the design. Check: start `gdb-py3 --interpreter=mi2 -nx -x` it at the root, send `-file-exec-and-symbols` on `firmware.elf`, then `-target-select remote x`: `^connected`, no `*stopped` before that command. Send `-target-detach`.

## 3. IDE files

- [ ] 3.1 Write `ide/misc.xml`, `ide/debug-profiles.xml` per the design, and `.mise/tasks/setup-ide`; add the dependency to `setup`, `.idea/` to `.gitignore`. Check: `xmllint --noout ide/*.xml`; `git check-ignore .idea`; a second `mise run setup` leaves an edited `.idea/misc.xml` unchanged.
- [ ] 3.2 Write the three `.run/` files per the design. Check: `xmllint --noout .run/*.xml`.

## 4. Clean-clone acceptance

- [ ] 4.1 Commit to the branch (collapsed at finish); clone into a `mktemp -d` directory; run `mise trust`, `mise install`, `mise run setup`. Check: all exit 0; `git status --short` empty; both `.idea` files present.
- [ ] 4.2 Run `clion .` there. Observe: database imported with no click; go-to-declaration reaches `sdk_headers`; `Build`, `Launch`, `Debug` and profile `Flipper` listed. `Build` exits 0.
- [ ] 4.3 Select `Flipper`; breakpoint on line 8; run `Debug`, then `Launch`. Observe: stop at `flipper_application_alloc_thread`; F9 stops on line 8 with `p`; F8 reaches line 9. Evaluate the user-site check: `False`. Press Stop.
- [ ] 4.4 Check: attach through a batch GDB: `furi_hal_debug_gdb_session_active` is `false`, `$pc` in flash; `mise run launch` exits 0; the USB console answers.
- [ ] 4.5 Restart CLion: `Flipper` stays selected. Quit; start from Toolbox: `Debug` connects. Note whether `Build` finds `mise`.
- [ ] 4.6 Move the probe's cable to another Mac port: `Debug` connects. Remove that cable, with the board on a Wi-Fi network: `Debug` connects through Wi-Fi. Note any Local Network prompt.

## 5. Docs

- [ ] 5.1 Add a Debugging section to `README.md`: preconditions, attach order, station mode, the 0.1.1 flash from the checked archive with its SHA-256, a session in CLion and in a terminal, the harmless exit-137 notice after Stop, the 30-second recovery, the board's exposure. Replace the fresh-clone note on tasks. Check: the owner follows it from a powered-off device to a breakpoint.
- [ ] 5.2 Check: `mise exec -- prek run --all-files` and `mise exec -- ufbt lint` exit 0; `wc -w` holds the budgets.

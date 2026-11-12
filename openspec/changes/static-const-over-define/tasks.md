# Tasks

No task takes the device or the owner's hands:
both waits keep their 200 ms, and task 1.1 shows the code the device runs stays the same.

## 1. The constant

- [x] 1.1 After `mise run build`, save the design's `objdump -d` listing in a `mktemp -d` folder, then write the design's line 8 and give both uses its name. Check: `mise run build` and `mise exec -- ufbt lint` exit 0; the listing matches the saved one; `git grep -n '#define' -- src test` prints `test_run` and `TRANSPORT_FAKE_EVENTS` alone.

  The design's assumption holds: the rebuilt `transport_bluetooth.o` lists the same 163 lines, both waits `movs r0, #200`.

## 2. The rule

- [x] 2.1 Add the design's paragraph to `openspec/config.yaml`, where the design puts it. Check: `git diff -- openspec/config.yaml` adds that paragraph alone; `mise exec -- openspec instructions tasks --change static-const-over-define --json` carries it in `context`.

## 3. The docs against the tree

- [x] 3.1 Read `README.md` and `docs/` against the tree for statements the change makes false, or true again; fix each false one. Check: the findings listed in this task's notes; `mise run docs-build` exits 0.

  None: `docs/internals/bluetooth.md:100` and `docs/internals/usb.md:78` give the 200 ms, not the name; no page states how a constant is declared.

## 4. Close

- [x] 4.1 Check: `mise run test`; `mise run build`; `mise exec -- ufbt lint`; `mise exec -- prek run --all-files`; `mise exec -- openspec validate static-const-over-define --strict` exit 0; `git status --short` lists only this change's files; `wc -w` holds the budgets.

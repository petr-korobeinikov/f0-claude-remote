# Design

## Context

No core or UI lands here: every file is tooling or IDE state,
the seam is the process boundary,
and each spec scenario is a command or an observation.

CLion facts below come from its decompiled classes and from its behavior in the dry run.

## Goals / Non-Goals

**Goals:** close each trap below; keep `ufbt blackmagic` as the path that needs no IDE.

**Non-Goals:** firmware sources; flashing the board from a task; `-Og` builds (the SDK's `-g -Os` stays).

## Decisions

Measured on the hardware unless a source is cited:

| Trap | Closure | Alternative beaten |
|---|---|---|
| Remote Debug ignores `.run`'s `<debugger>` (`RemoteDebugProfileLauncher`) | `gdb` profile in `.idea/debug-profiles.xml` | Embedded GDB Server: needs a target from `.idea/customTargets.xml` |
| `.idea/` before the first open blocks the database import | `.idea/misc.xml` carries the `CompDBSettings` link; import then ran alone | Tracked `.idea`: CLion's `misc.xml` has no final newline |
| CLion sends `target remote`, expects `^connected`; `attach` inside answers `^running` | `define target remote`; a child GDB scans, attaches, detaches; the probe reattaches when the parent connects | `gdb.MICommand`: "name is already in use" |
| Touching the target at GDB start emits `*stopped`; CLion queries a dead thread and ends the session | Nothing runs before `-target-select` | Connecting in the `-x` script |
| Stop is `-target-detach` then SIGKILL (exit 137), so `gdb_exiting` never clears the firmware's session flag | Setter disabled; internal breakpoint on `flipper_application_alloc_thread`, called only by the loader, with the app already mapped | Reattaching at exit |
| The probe's USB name follows the Mac's port (`usbmodem3101`, `usbmodem1101`) | `list_ports.grep("blackmagic")` in GDB's Python | An address in `.run`; a label stands there instead |
| GDB's own lookup of `blackmagic.local` failed once | Python resolves it, as `ufbt` does | Passing the hostname |
| GDB's Python sees `~/.local/lib/python3.11/site-packages`; `fbtenv.sh` does not run for CLion | Profile: `passSystemEnv` off, `PYTHONNOUSERSITE=1` | Inheriting the shell |
| With no `HOME`, GDB warns about its index cache | `HOME` under `.ufbt` | Real `HOME` |
| `cp -n` exits 1 on an existing file | `[ -e … ] \|\| cp` | `cp -n` under `set -e` |

Device preconditions, for `README.md`:
Debug on (`furi_hal_rtc_init_early` otherwise disables SWD),
PIN unlocked (`desktop_lock` calls `cli_vcp_disable`),
board attached with the device off and its cable out.

`debug/black_magic.gdb`, with its comments as written:

```gdb
set confirm off
set pagination off

python
import socket
import subprocess
from serial.tools import list_ports


def black_magic_address():
    ports = sorted(port.device for port in list_ports.grep("blackmagic"))
    if ports:
        return ports[0]
    return "tcp:" + socket.gethostbyname("blackmagic.local") + ":2345"


def connect_black_magic():
    address = black_magic_address()
    # `attach` under -target-select answers ^running, and CLion demands ^connected.
    # The probe reattaches to its last target when the next client connects.
    subprocess.run(
        [".ufbt/toolchain/arm64-darwin/bin/arm-none-eabi-gdb-py3", "-nx", "-batch",
         "-ex", "target extended-remote " + address, "-ex", "monitor swdp_scan",
         "-ex", "attach 1", "-ex", "detach"],
        check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    gdb.execute("target extended-remote " + address)
end

# CLion can only say `target remote`.
define target remote
  python connect_black_magic()
  source .ufbt/current/scripts/debug/flipperapps.py
  # CLion's Stop kills GDB, so this flag would survive, and the firmware hits `bkpt`
  # on the next app start.
  python helper.set_debug_mode(False)
  python helper.set_debug_mode = lambda mode: None
  fap-set-debug-elf-root .ufbt/build
  python gdb.Breakpoint("flipper_application_alloc_thread", internal=True)
end
```

`ide/debug-profiles.xml`, the shape CLion's own serializer writes:

```xml
<project version="4"><component name="Debug Profiles"><debug-profiles><debug-profile>
  <option name="id" value="flipper" /><option name="type" value="gdb" />
  <option name="name" value="Flipper" />
  <settings>
    <option name="arguments" value="-nx -x debug/black_magic.gdb" />
    <option name="env"><map>
      <entry key="HOME" value="$PROJECT_DIR$/.ufbt" />
      <entry key="PYTHONNOUSERSITE" value="1" />
      <entry key="PYTHONPYCACHEPREFIX" value="$PROJECT_DIR$/.ufbt/pycache" />
    </map></option>
    <option name="executable" value="$PROJECT_DIR$/.ufbt/toolchain/arm64-darwin/bin/arm-none-eabi-gdb-py3" />
    <option name="passSystemEnv" value="false" />
    <option name="workingDir" value="$PROJECT_DIR$" />
  </settings>
</debug-profile></debug-profiles></component></project>
```

`.run/Debug.run.xml`: type `CLion_Remote`, `version="1"`,
`remoteCommand="black-magic-probe"`, `symbolFile="$PROJECT_DIR$/.ufbt/current/firmware.elf"`, `sysroot=""`.
`Build` and `Launch`: type `ShConfigurationType`, `SCRIPT_TEXT` `mise run build` and `mise run launch`,
`INTERPRETER_PATH` `/bin/sh`, `SCRIPT_WORKING_DIRECTORY` `$PROJECT_DIR$`,
each path with its `INDEPENDENT_…` option true, `EXECUTE_IN_TERMINAL` and `EXECUTE_SCRIPT_FILE` false.

- `.mise/tasks/build` builds the app through `ufbt`.
- `.mise/tasks/launch` runs `ufbt launch`.
- `.mise/tasks/debug` runs `ufbt blackmagic`, with `#MISE raw=true`.
- `.mise/tasks/setup-ide` copies missing `ide/` files into `.idea/`; `setup` gains `#MISE depends=["setup-ide"]`.
- `debug/black_magic.gdb` connects CLion's GDB to the probe.
- `ide/misc.xml` links the compilation database, as CLion writes it, with a final newline.

## Risks / Trade-offs

- A CLion update changes the profile or `.run` format, or stops ignoring `remoteCommand` → `mise run debug` needs neither.
- The script reaches into `flipperapps.py`'s `helper` → an SDK bump reruns the session checks.
- macOS's Local Network permission blocks CLion's GDB over Wi-Fi → untested in CLion; task 4.6 records it.
- A hot-plugged board, or a powered board on a switched-off device, left the device not starting once → `README.md` gives the order; a 30-second Back press recovered it.
- `Build` and `Launch` need `mise` on `PATH` under a Toolbox start → the console start is the case designed for; the owner chose not to check Toolbox.

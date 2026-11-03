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

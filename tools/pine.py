"""A client for PCSX2's PINE link: reading and writing the running game's memory.

PINE is a setting of PCSX2 ([EmuCore] EnablePINE = true, PINESlot = the TCP port on Windows).
A request is a 32-bit size (of the whole message) followed by commands, each an opcode byte and
its arguments; the reply is a 32-bit size, a result byte (0 when every command worked) and each
command's answer in turn. tools/launcher.py uses it to write new card and effect tables into the
game without restarting it.
"""

import socket
import struct

READ8, READ16, READ32, READ64 = 0, 1, 2, 3
WRITE8, WRITE16, WRITE32, WRITE64 = 4, 5, 6, 7
VERSION, TITLE, ID, STATUS = 8, 0xB, 0xC, 0xF
STATUS_NAMES = {0: "running", 1: "paused", 2: "shut down"}
# PCSX2 takes messages of up to 650,000 bytes; a WRITE64 command is 13.
BATCH = 40000


class PineError(Exception):
    pass


class Pine:
    def __init__(self, port=28011, timeout=5.0):
        self.port = port
        self.timeout = timeout
        self.sock = None

    def connect(self):
        if self.sock is None:
            self.sock = socket.create_connection(("127.0.0.1", self.port), timeout=self.timeout)
        return self

    def close(self):
        if self.sock is not None:
            self.sock.close()
            self.sock = None

    def __enter__(self):
        return self.connect()

    def __exit__(self, *_):
        self.close()

    def _receive(self, n):
        data = b""
        while len(data) < n:
            chunk = self.sock.recv(n - len(data))
            if not chunk:
                raise PineError("PCSX2 closed the link")
            data += chunk
        return data

    def request(self, commands):
        """Sends the commands (bytes each) as one message; returns the answers' bytes."""
        body = b"".join(commands)
        self.connect()
        self.sock.sendall(struct.pack("<I", len(body) + 4) + body)
        size = struct.unpack("<I", self._receive(4))[0]
        reply = self._receive(size - 4)
        if not reply or reply[0] != 0:
            raise PineError("PCSX2 refused the request (is a game running?)")
        return reply[1:]

    def status(self):
        return struct.unpack("<I", self.request([bytes([STATUS])]))[0]

    def title(self):
        answer = self.request([bytes([TITLE])])
        size = struct.unpack_from("<I", answer)[0]
        return answer[4:4 + size].rstrip(b"\0").decode("utf-8", "replace")

    def read32(self, address):
        return struct.unpack("<I", self.request([struct.pack("<BI", READ32, address)]))[0]

    def write32(self, address, value):
        self.request([struct.pack("<BII", WRITE32, address, value & 0xffffffff)])

    def read(self, address, size):
        """size bytes from an address that is a multiple of 8 (size a multiple of 8 too)."""
        out = b""
        for at in range(address, address + size, 8 * BATCH):
            count = min(BATCH, (address + size - at) // 8)
            out += self.request([struct.pack("<BI", READ64, at + 8 * k) for k in range(count)])
        return out

    def write_words(self, words):
        """words: (address, 8 bytes) pairs, each address a multiple of 8."""
        words = list(words)
        for start in range(0, len(words), BATCH):
            self.request([struct.pack("<BI", WRITE64, a) + w for a, w in words[start:start + BATCH]])
        return len(words)


def changed_words(old, new, base, start, end):
    """The 8-byte words of new[start:end] that differ from old, as (address, bytes) pairs."""
    start &= ~7
    for k in range(start, end, 8):
        if old[k:k + 8] != new[k:k + 8]:
            yield base + k, new[k:k + 8].ljust(8, b"\0")

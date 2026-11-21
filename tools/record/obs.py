"""OBS Studio's built-in remote control (obs-websocket 5, on this computer only), with nothing but
Python's standard library: enough to set up a recording scene and start and stop recording.

  setup_config()        before OBS's first start: its remote control on, localhost, no password
  with OBS() as obs:    obs.call("StartRecord") ...  (requests: github.com/obsproject/obs-websocket)
"""
import base64
import json
import os
import socket
import struct
from pathlib import Path

PORT = 4455
CONFIG = Path(os.environ.get("APPDATA", "")) / "obs-studio" / "plugin_config" / "obs-websocket" / "config.json"


def setup_config():
    """Turns the remote control on (no password: it only listens on this computer's own address
    for this setup's scripts). Run while OBS is closed; OBS keeps the file as it is."""
    CONFIG.parent.mkdir(parents=True, exist_ok=True)
    data = json.loads(CONFIG.read_text()) if CONFIG.is_file() else {}
    data.update({"first_load": False, "server_enabled": True, "server_port": PORT,
                 "auth_required": False, "alerts_enabled": False})
    data.setdefault("server_password", "")
    CONFIG.write_text(json.dumps(data, indent=2))


class OBS:
    def __init__(self, host="127.0.0.1", port=PORT, timeout=10):
        self.sock = socket.create_connection((host, port), timeout=timeout)
        key = base64.b64encode(os.urandom(16)).decode()
        self.sock.sendall((f"GET / HTTP/1.1\r\nHost: {host}:{port}\r\nUpgrade: websocket\r\n"
                           f"Connection: Upgrade\r\nSec-WebSocket-Key: {key}\r\nSec-WebSocket-Version: 13\r\n"
                           "Sec-WebSocket-Protocol: obswebsocket.json\r\n\r\n").encode())
        head = b""
        while b"\r\n\r\n" not in head:
            head += self.sock.recv(1)
        if b" 101 " not in head.split(b"\r\n")[0]:
            raise ConnectionError("OBS did not accept the connection: " + head.decode(errors="replace"))
        hello = self._recv()
        if hello.get("op") != 0:
            raise ConnectionError(f"unexpected first message from OBS: {hello}")
        if hello["d"].get("authentication"):
            raise ConnectionError("OBS's remote control asks for a password; turn authentication off")
        self._send({"op": 1, "d": {"rpcVersion": 1, "eventSubscriptions": 0}})
        if self._recv().get("op") != 2:
            raise ConnectionError("OBS did not identify the connection")
        self.n = 0

    def __enter__(self):
        return self

    def __exit__(self, *exc):
        self.sock.close()

    def _send(self, message):
        data = json.dumps(message).encode()
        mask = os.urandom(4)
        n = len(data)
        head = bytes([0x81]) + (bytes([0x80 | n]) if n < 126 else
                                bytes([0x80 | 126]) + struct.pack(">H", n) if n < 65536 else
                                bytes([0x80 | 127]) + struct.pack(">Q", n))
        self.sock.sendall(head + mask + bytes(b ^ mask[i % 4] for i, b in enumerate(data)))

    def _exact(self, n):
        data = b""
        while len(data) < n:
            part = self.sock.recv(n - len(data))
            if not part:
                raise ConnectionError("OBS closed the connection")
            data += part
        return data

    def _recv(self):
        while True:
            b0, b1 = self._exact(2)
            n = b1 & 0x7F
            if n == 126:
                n = struct.unpack(">H", self._exact(2))[0]
            elif n == 127:
                n = struct.unpack(">Q", self._exact(8))[0]
            data = self._exact(n)
            if b0 & 0x0F == 1:
                return json.loads(data)
            if b0 & 0x0F == 8:
                raise ConnectionError("OBS closed the connection")

    def call(self, request, **data):
        self.n += 1
        rid = str(self.n)
        self._send({"op": 6, "d": {"requestType": request, "requestId": rid, "requestData": data}})
        while True:
            m = self._recv()
            if m.get("op") == 7 and m["d"].get("requestId") == rid:
                status = m["d"]["requestStatus"]
                if not status.get("result"):
                    raise RuntimeError(f"OBS {request}: {status.get('comment', status)}")
                return m["d"].get("responseData", {})

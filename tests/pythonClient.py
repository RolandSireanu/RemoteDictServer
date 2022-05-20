# echo-client.py

import socket

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 40000  # The port used by the server

# text = b"Hello, world!"*4096
# text += b"\n";

text = b"MKDIR14/RoSi/root/dir\n"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(text)
    # data = s.recv(1024)

# print(f"Received {data!r}")
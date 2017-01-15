import pytest
import mmap
import socket

def test_socket(benchmark):
    def socket_call():
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    benchmark.pedantic(socket_call, iterations=1000, rounds=100)

def test_bind(benchmark):
    def bind_call():
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind(('', 9090))
    benchmark.pedantic(bind_call, iterations=1000, rounds=100)

def test_mmap(benchmark):
    with open("test_mmap.txt", "wb") as f:
        f.write("Hello, OpenBSD!\n")
    def mmap_call():
        with open("test_mmap.txt", "r+b") as f:
            mm = mmap.mmap(f.fileno(), 0)
            mm.close()
    benchmark.pedantic(mmap_call, iterations=100, rounds=10)

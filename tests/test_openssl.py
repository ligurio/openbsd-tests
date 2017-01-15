import pytest
import subprocess

@pytest.mark.slowtest
def test_x11perf():
    openssl_cmd = ["openssl", "speed"]
    proc = subprocess.Popen(openssl_cmd)
    proc.wait()
    assert proc.returncode == 0

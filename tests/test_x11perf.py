import os
import subprocess
import pytest


def isX11():
    X11PATH = "/usr/X11/"
    if os.path.exists(X11PATH):
        return True
    return False


@pytest.mark.skipif(isX11 is False, reason="Xenocara is not installed")
@pytest.mark.slowtest
def test_x11perf():
    x11perf_cmd = ["x11perf", "-all"]
    proc = subprocess.Popen(x11perf_cmd)
    proc.wait()
    assert proc.returncode == 0

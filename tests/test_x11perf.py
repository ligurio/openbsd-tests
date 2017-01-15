import os
import pytest
import subprocess


def isX11():
    X11PATH = "/usr/X11/"
    if os.path.exists(X11PATH):
        return 0
    return 1


@pytest.mark.skipif('isX11 != 0')
@pytest.mark.slowtest
def test_x11perf():
    x11perf_cmd = ["x11perf", "-all"]
    proc = subprocess.Popen(x11perf_cmd)
    proc.wait()
    assert proc.returncode == 0

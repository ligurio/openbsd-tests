import subprocess
import pytest
from conftest import osname


@pytest.fixture(scope="session")
def make_fusexmp_bin():
    cmd = ["gcc", "fusexmp.c", "-o", "fusexmp"]
    retcode = subprocess.call(cmd)
    assert retcode == 0


@pytest.mark.skipif(osname != "OpenBSD", reason="supported only OpenBSD")
@pytest.mark.usefixtures('make_fusexmp_bin')
def test_fusexmp():
    fusexmp_cmd = ["fusexmp", "speed"]
    proc = subprocess.Popen(fusexmp_cmd)
    proc.wait()
    assert proc.returncode == 0

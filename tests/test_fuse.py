import pytest
import subprocess


@pytest.fixture(scope="session")
def make_fusexmp_bin(request):
    cmd = ["gcc", "fusexmp.c", "-o", "fusexmp"]
    retcode = subprocess.call(cmd)
    assert retcode == 0


@pytest.mark.usefixtures('make_fusexmp_bin')
def test_fusexmp():
    fusexmp_cmd = ["fusexmp", "speed"]
    proc = subprocess.Popen(fusexmp_cmd)
    proc.wait()
    assert proc.returncode == 0

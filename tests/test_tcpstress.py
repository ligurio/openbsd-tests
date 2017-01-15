import pytest
import subprocess


@pytest.fixture(scope="session")
def make_tcpstress_bin(request):
    cmd = ["gcc", "tcpstress.c", "-o", "tcpstress"]
    retcode = subprocess.call(cmd)
    assert retcode == 0


@pytest.mark.usefixtures('make_tcpstress_bin')
def test_tcpstress():
    THREADS = 10
    ACTIVE = 5
    MAX = 10
    HOSTNAME = 'localhost'
    PORT = 1000

    tcpstress_cmd = ['tcpstress', '-t', THREADS,
                     '-a', ACTIVE, '-m', MAX, HOSTNAME, PORT]
    proc = subprocess.Popen(tcpstress_cmd)
    proc.wait()
    assert proc.returncode == 0

import pytest
import random
import subprocess

# kern.version
# kern.ostype
# kern.osrelease
# kern.version
# kern.hostname

# hw.machine
# hw.vendor
# hw.model
# hw.product
# hw.version
# hw.cpuspeed
# hw_cpus_vendor
# hw_cpus_cores
# hw_cpus_mhz
# hw.ncpu
# hw.physmem
# machdep.cpuvendor


def pytest_report_header(config):
    sysinfo = subprocess.check_output(["sysctl", "-n", "hw.machine"])
    return sysinfo


def rnd_string(length):
    number = '0123456789'
    alpha = 'abcdefghijklmnopqrstuvwxyz'
    id = ''
    for i in range(0, length, 2):
        id += random.choice(number)
        id += random.choice(alpha)
    return id


def rnd_integer(maxval):
    return random.randrange(0, maxval, 1)

@pytest.mark.parametrize("key, keytype", [
    ("kern.sugid_coredump", "iteger"),
    ("kern.corefile", "string"),
])
def test_sysctl_fuzz(key, keytype):
    loops = 10
    orig_value = subprocess.check_output(["sysctl", "-n", key]).rstrip()
    print key, "=", orig_value
    for i in range(0, loops):
        value = None
        if keytype == "string":
           length = rnd_integer(1000)
           value = rnd_string(length)
           retcode = subprocess.call(["sysctl", "%s=%s" % (key, value)])
        elif keytype == "integer":
           value = rnd_integer(65536)
           retcode = subprocess.call(["sysctl", "%s=%s" % (key, value)])
           if retcode <> 0:
              pytest.fail("Failed set %s to the value %s" % (key, value))
    retcode = subprocess.call(["sysctl", "%s=%s" % (key, orig_value)])
    if retcode <> 0:
       pytest.fail("Failed revert %s to the original value %s" % (key, orig_value))

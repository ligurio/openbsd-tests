import pytest
import subprocess
from conftest import service
from conftest import osname


@pytest.mark.skipif(osname != "OpenBSD")
def test_adjtime():
    service("ntpd", "start")
    print "Sync time"
    proc = subprocess.Popen(["ntpd", "-s"])
    proc.wait()
    service("ntpd", "stop")
    print "Move time forward for 5 minutes"
    # time=`date "+ %M"`
    # date $(($time+5)) > /dev/null 2>&1 && date "+ %H:%M"
    print "Validate adjustment"
    # ntpd -s && sleep 60 && pkill -TERM ntpd
    # date "+ %H:%M"
    # [ `date "+ %M"` -eq $(($time+1)) ]; then

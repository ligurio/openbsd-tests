import pytest
import subprocess

def service(service_name, action):
    proc_cmd = ["/etc/rc.d/%s" % service_name, action]
    proc = subprocess.Popen(proc_cmd)
    proc.wait()
    assert proc.returncode == 0

def test_adjtime():
    service("ntpd", "start")
    print "Sync time"
    proc = subprocess.Popen(["ntpd", "-s"])
    service("ntpd", "stop")
    print "Move time forward for 5 minutes"
    # time=`date "+ %M"`
    # date $(($time+5)) > /dev/null 2>&1 && date "+ %H:%M"
    print "Validate adjustment"
    # ntpd -s && sleep 60 && pkill -TERM ntpd

	# date "+ %H:%M"
[ `date "+ %M"` -eq $(($time+1)) ]; then

import pytest
import subprocess

# PKG_REQUIRED = zzuf
ZZUF_CMD = "-s0:100 -r0.01"
NUMBER = 20

appconfig = {'smtpd': '/etc/mail/smtpd.conf',
             'ntpd': '/etc/ntpd.conf',
             'ldapd': '/etc/ldapd.conf',
             'dhcpd': '/etc/dhcpd.conf',
             'npppd': '/etc/npppd/npppd.conf',
             'iked': '/etc/iked.conf'}

apps = ['smtpd', 'ntpd', 'ldapd', 'dhcpd', 'npppd', 'iked']


@pytest.fixture(scope='module', params=apps)
def app(request):
    return request.param


def fuzz_config(app):
    zzuf_cmd = ZZUF_CMD.split(' ')
    proc = subprocess.Popen(zzuf_cmd, appconfig[app])
    proc.wait()
    assert proc.returncode == 0


def check_config(app):
    cmd = [app, '-n', '-f', appconfig[app]]
    proc = subprocess.Popen(cmd)
    proc.wait()
    assert proc.returncode == 0


def test_config(app):
    for n in range(NUMBER):
        fuzz_config(app)
        check_config(app)

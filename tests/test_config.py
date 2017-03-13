import pytest
import subprocess

# PKG_REQUIRED = zzuf
ZZUF_OPT = "-s0:100 -r0.01"
NUMBER = 20

config = {'smtpd': '/etc/mail/smtpd.conf',
          'ntpd': '/etc/ntpd.conf',
          'ldapd': '/etc/ldapd.conf',
          'dhcpd': '/etc/dhcpd.conf',
          'npppd': '/etc/npppd/npppd.conf',
          'iked': '/etc/iked.conf'}


def fuzz_config(app):
    zzuf_cmd = ['zzuf'] + ZZUF_OPT.split(' ') + [config[app]]
    proc = subprocess.Popen(zzuf_cmd)
    proc.wait()
    assert proc.returncode == 0


def check_config(app):
    cmd = [app, '-n', '-f', config[app]]
    proc = subprocess.Popen(cmd)
    proc.wait()
    assert proc.returncode == 0


@pytest.mark.parametrize("app", ['smtpd', 'ntpd', 'ldapd', 'dhcpd', 'npppd', 'iked'])
def test_config(app):
    for n in range(NUMBER):
        fuzz_config(app)
        check_config(app)

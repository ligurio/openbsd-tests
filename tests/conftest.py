import subprocess
import os
from settings import DEF_VERSION, DEF_PKG_PATH


def service(service_name, action):
    proc_cmd = ["/etc/rc.d/%s" % service_name, action]
    proc = subprocess.Popen(proc_cmd)
    proc.wait()
    assert proc.returncode == 0


def addpkg(package, version=DEF_VERSION, pkg_path=DEF_PKG_PATH):
    pkg_path = os.path.join(pkg_path, version, 'packages')
    proc = subprocess(['pkg_add', package])
    proc.wait()
    assert proc.returncode == 0

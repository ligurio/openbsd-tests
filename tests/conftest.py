import subprocess


def service(service_name, action):
    proc_cmd = ["/etc/rc.d/%s" % service_name, action]
    proc = subprocess.Popen(proc_cmd)
    proc.wait()
    assert proc.returncode == 0

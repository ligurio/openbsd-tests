#!/usr/local/bin/python

# https://gist.github.com/vincentbernat/952e0ada1333f0e0a69f/
# libcloud, terraform, AWS CloudFormation

"""
$ pip install cs
$ cat $HOME/.cloudstack.ini
[cloudstack]
endpoint = https://api.exoscale.ch/compute
key = mqqdAD39YWxUrNgIDOvxjEgI4TC-Y0dFkMKSMhCGosJh5o2MouFyR-1buIKLXSTERlW334OIYptfjMLaYs
secret = CuAqaKhtdtwGIg1rGfK4CyrtcPb-0NEvl8LozG4nAYtymuJ2f6XU41Rdat_Zs1Ma1BRBaLXzrEXyZ
$ cat regress.sh
echo "Running test"
sleep 10
echo "Teardown"
$ python testenv.py < regress.sh
"""

import os
import cs
import hashlib
import time
import socket
import io
from paramiko.client import SSHClient, AutoAddPolicy
from paramiko.rsakey import RSAKey


zone = 'ch-dk-2'
distribution = 'OpenBSD 6.2 64-bit'
size = 50
serviceoffering = 'Small'

securitygroupname = 'sg-test'
keypairname = 'kp-test'


def securitygroupid(cloudstack, name):
    g = cloudstack.createSecurityGroup(name = name)['securitygroup']
    cloudstack.authorizeSecurityGroupIngress(icmptype=8,
                                             icmpcode=0,
                                             protocol="ICMP",
                                             cidrlist="0.0.0.0/0",
                                             securitygroupid=g['id'])
    cloudstack.authorizeSecurityGroupIngress(protocol="TCP",
                                             cidrlist="0.0.0.0/0",
                                             startport=22,
                                             endport=22,
                                             securitygroupid=g['id'])

    return g['id']
    # Cannot do that, machine has to be purged...
    # teardown: cloudstack.deleteSecurityGroup(id=g['id'])


def keypair(cloudstack, name):
    kp = cloudstack.createSSHKeyPair(name=name)['keypair']

    return kp
    # teardown: cloudstack.deleteSSHKeyPair(name=name)


def vm(cloudstack, keypair, serviceofferingid, securitygroupid,
       templateid, zoneid):

    v = cloudstack.deployVirtualMachine(
        serviceofferingid=serviceofferingid,
        templateid=templateid,
        zoneid=zoneid,
        displayname="test-{}".format(os.getpid()),
        securitygroupids=[securitygroupid],
        keypair=keypair['name'],
        name="test-{}".format(os.getpid()))


def raise_vm(cloudstack, vm):

    now = time.time()
    while True:
        j = cloudstack.queryAsyncJobResult(jobid=vm['jobid'])
        if j['jobstatus'] != 0:
            if j['jobresultcode'] != 0 or j['jobstatus'] != 1:
                raise RuntimeError(
                    'VM was not spawned successfully: {}'.format(j))
            if 'jobresult' not in j:
                raise RuntimeError(
                    'No result after spawning the VM: {}'.format(j))
            v = j['jobresult']['virtualmachine']
            v['ipaddress'] = v['nic'][0]['ipaddress']
            time.sleep(2)       # Let the VM settle
            return v
        time.sleep(1)
        if time.time() - now > 60:
            raise RuntimeError(
                'Unable to deploy VM due to timeout: {}'.format(j))

    return v
    # teardown: cloudstack.destroyVirtualMachine(id=v['id'])


def template(cloudstack, size, zoneid):

    templates = cloudstack.listTemplates(templatefilter='featured',
                                         zoneid=zoneid)['template']
    templates = [t for t in templates
                 if t['name'] == distribution and
                 int(t['size'] / 1024 / 1024 / 1024) == size]

    return templates[0]['id']


def isrunning(instance):
    now = time.time()
    while True:
        try:
            socket.create_connection((instance['ipaddress'], 22), timeout=10)
        except socket.error:
            if time.time() - now < 120:
                continue
            raise
        break


def sshvm(instance):
    with SSHClient() as client:
        client.set_missing_host_key_policy(AutoAddPolicy())
        client.connect(instance['ipaddress'],
                       username="root",
                       timeout=10,
                       password=instance['password'],
                       allow_agent=False,
                       look_for_keys=False)
        return client


def test_connect_with_key(instance, keypair, sshclient):
    sshclient.connect(instance['ipaddress'],
                      username="root",
                      timeout=10,
                      pkey=RSAKey.from_private_key(
                          io.StringIO(keypair['privatekey'])),
                      allow_agent=False,
                      look_for_keys=False)
    stdin, stdout, stderr = sshclient.exec_command('echo hello')
    stdin.close()
    assert stdout.read() == b"hello\n"
    assert stderr.read() == b""


def test_connect_with_password(sshvm):
    stdin, stdout, stderr = sshvm.exec_command('echo hello')
    stdin.close()
    assert stdout.read() == b"hello\n"
    assert stderr.read() == b""


def execute_command(sshvm):

    now = time.time()
    stdin, stdout, stderr = sshvm.exec_command("sleep 10")
    stdin.close()
    print(time.time() - now)


def zoneid(cloudstack, zone):

    for z in cloudstack.listZones()['zone']:
        if z['name'] == zone:
            id = z['id']
            print("zone id is {}".format(id))
            return id


cloudstack = cs.CloudStack(**cs.read_config())
with SSHClient() as client:
    client.set_missing_host_key_policy(AutoAddPolicy())

serviceofferingid = cloudstack.listServiceOfferings()['serviceoffering'][0]['id']
securitygroupid = securitygroupid(cloudstack, securitygroupname)
zoneid = zoneid(cloudstack, zone)
keypair = keypair(cloudstack, keypairname)
templateid = template(cloudstack, size, zoneid)

v = vm(cloudstack, keypair, serviceofferingid, securitygroupid, templateid, zoneid)
instance = raise_vm(cloudstack, v)
isrunning(instance)
ssh_obj = sshvm(instance)
test_connect_with_password(ssh_obj)

#!/usr/bin/python

# https://wiki.libvirt.org/page/VM_lifecycle

from __future__ import print_function
import sys

CONN_STRING = 'qemu:///system'
STATES_FRIENDLY_NAMES = { libvirt.VIR_DOMAIN_NOSTATE: "VIR_DOMAIN_NOSTATE",
                        libvirt.VIR_DOMAIN_RUNNING: "VIR_DOMAIN_RUNNING",
                        libvirt.VIR_DOMAIN_BLOCKED: "VIR_DOMAIN_BLOCKED",
                        libvirt.VIR_DOMAIN_PAUSED: "VIR_DOMAIN_PAUSED",
                        libvirt.VIR_DOMAIN_SHUTDOWN: "VIR_DOMAIN_SHUTDOWN",
                        libvirt.VIR_DOMAIN_SHUTOFF: "VIR_DOMAIN_SHUTOFF",
                        libvirt.VIR_DOMAIN_CRASHED: "VIR_DOMAIN_CRASHED",
                        libvirt.VIR_DOMAIN_PMSUSPENDED: "VIR_DOMAIN_PMSUSPENDED" }


def print_conn_info(conn):

    ver = conn.getVersion()
    if ver is not None:
        print('Version: ' + str(ver))
    ver = conn.getLibVersion()
    if ver is not None:
        print('Libvirt Version: ' + str(ver))
    type = conn.getType()
    if type is not None:
        print('Running hypervisor: %s' % type)
    print ("--------------------------------------------")


def current_state(dom):

    state, reason = dom.state()
    if state in STATES_FRIENDLY_NAMES.keys():
        print('The state of %s is %s' % (dom.name(), STATES_FRIENDLY_NAMES[state]))
        return state
    else:
        print('The state of %s is unknown. The reason code is %s' % (dom.name(), str(reason)))
        return None


def start_action(xmlconfig):

    dom = conn.createXML(xmlconfig, 0)
    if dom is None:
        print('Failed to create a domain from an XML definition.', file=sys.stderr)
        exit(1)

    return dom

    dom = conn.defineXML(xmlconfig, 0)
    if dom is None:
        print('Failed to define a domain from an XML definition.', file=sys.stderr)
        exit(1)


def shutdown_action(dom):

    dom.shutdown()


def define_action(dom):

    dom.destroy()


def undefine_action(dom):

    dom.undefine()


def pause_action(dom):

    dom.pause()


def save_action(dom):

    dom.save()


def restore_action(dom):

    dom.restore()


def suspend_action(dom):

    dom.suspend()


def resume_action(dom):

    dom.resume()


domName = 'sergeyb'

xmlconfig = """
<domain type='qemu'>
  <name>sergeyb</name>
  <memory>2192</memory>
  <currentMemory>2192</currentMemory>
  <vcpu>2</vcpu>
  <os>
    <type arch='i686' machine='pc'>hvm</type>
    <boot dev='cdrom'/>
  </os>
  <devices>
    <emulator>/usr/bin/qemu-system-x86_64</emulator>
    <disk type='file' device='disk'>
      <source file='/tmp/qemu.img'/>
      <target dev='hda'/>
    </disk>
    <interface type='network'>
      <source network='default'/>
    </interface>
    <graphics type='vnc' port='-1'/>
  </devices>
</domain>
"""

conn = libvirt.open(CONN_STRING)
if conn is None:
    print('Failed to open connection to %s' % CONN_STRING, file=sys.stderr)
    exit(1)

print_conn_info(conn)

dom = start_action(xmlconfig)
for i in range(0, 2000):
    print ("Action: SUSPEND %s" % dom.name())
    suspend_action(dom)
    current_state(dom)
    print ("Action: RESUME %s" % dom.name())
    resume_action(dom)
    current_state(dom)

dom.destroy()

conn.close()
exit(0)

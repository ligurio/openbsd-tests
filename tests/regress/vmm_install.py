# vmm-install is a tool for automated testing of operating systems supported by
# OpenBSD vmm(4).
#
# See also:
#
# - [Anita, an Automated NetBSD Installation and Test Application](http://www.gson.org/netbsd/anita/)
# - [Packer](https://github.com/ligurio/openbsd-tests/tree/master/packer-templates)
# - virt-install

# Scenarios:
#
# - Debian Linux https://github.com/vext01/recipes/blob/master/recipes/debian9_inside_vmm.md
# - CentOS Linux https://twitter.com/i/web/status/936108666851180544
# - OpenBSD https://gist.github.com/tvlooy/fd6bc5a77bc03f4d419f395dfcf4f038
# - Alpine Linux https://medium.com/@dave_voutila/docker-on-openbsd-6-1-current-c620513b8110
#
# https://www.openbsd.org/faq/faq16.html

import pexpect
from subprocess import check_call, check_output

def create_machine():
    create_cmd = "vmctl create ~/Downloads/vm1.img - s 500M"
    check_call(create_cmd.split(" "))
    start_cmd = "vmctl start openbsd - c - b / home/sergeyb/Downloads/bsd.rd - m 2G - i 1 - d / home/sergeyb/Download"
    check_call(start_cmd.split(" "))

def stop_machine():
    stop_cmd = "vmctl stop openbsd"
    check_call(stop_cmd.split(" "))

def run_scenario():
    child = pexpect.spawn("cu /dev/ttyp4")
    child.expect('Enter password:')
    child.sendline('password')
    child.expect(pexpect.EOF, timeout=None)
    cmd_show_data = child.before
    cmd_output = cmd_show_data.split('\r\n')
    for data in cmd_output:
        print data

    # see https://github.com/gson1703/anita/blob/master/anita.py#L1200

    child.expect("(I)nstall, (U)pgrade, (A)utoinstall or (S)hell?")
    child.send("I\r")
    child.expect("Terminal type? [vt220]")
    child.send("\r")
    child.expect("System hostname? (short form, e.g. 'foo')")
    child.send("foo\r")
    child.expect("Which network interface do you wish to configure? (or 'done') [vio0]")
    child.send("\r")
    child.expect("IPv4 address for vio0? (or 'dhcp' or 'none') [dhcp]")
    child.send("\r")
    child.expect("IPv6 address for vio0? (or 'autoconf' or 'none') [none]")
    child.send("\r")
    child.expect("Which network interface do you wish to configure? (or 'done') [done]")
    child.send("\r")
    child.expect("DNS domain name? (e.g. 'example.com') [my.domain]")
    child.send("\r")
    child.expect("DNS nameservers? (IP address list or 'none') [none]")
    child.send("\r")
    child.expect("Password for root account? (will not echo)")
    child.send("1q2w3e\r")
    child.expect("Password for root account? (again)")
    child.send("1q2w3e\r")
    child.expect("Start sshd(8) by default? [yes]")
    child.send("\r")
    child.expect("Change the default console to com0? [yes]")
    child.send("\r")
    child.expect("Which speed should com0 use? (or 'done') [9600]")
    child.send("115200\r")
    child.expect("Setup a user? (enter a lower-case loginname, or 'no') [no]")
    child.send("\r")
    child.expect("Allow root ssh login? (yes, no, prohibit-password) [no]")
    child.send("\r")
    child.expect("Which disk is the root disk? ('?' for details) [sd0]")
    child.send("\r")
    child.expect("Use (W)hole disk MBR, whole disk (G)PT or (E)dit? [whole]")
    child.send("\r")
    child.expect("Use (A)uto layout, (E)dit auto layout, or create (C)ustom layout? [a]")
    child.send("\r")
    child.expect("Which disk do you wish to initialize? (or 'done') [done]")
    child.send("\r")
    child.expect("Location of sets? (disk http or 'done') [http]")
    child.send("disk")

    # detach console
    # send: "~^D"

vmm-install is a tool for automated testing of operating systems supported by
OpenBSD vmm(4).


### TODO

- Debian Linux https://github.com/vext01/recipes/blob/master/recipes/debian9_inside_vmm.md
- CentOS Linux https://twitter.com/i/web/status/936108666851180544
- OpenBSD https://gist.github.com/tvlooy/fd6bc5a77bc03f4d419f395dfcf4f038
- Alpine Linux https://medium.com/@dave_voutila/docker-on-openbsd-6-1-current-c620513b8110


#### OpenBSD scenario

```
vmctl create ~/Downloads/vm1.img -s 500M
doas vmctl start "openbsd" -c -b /home/sergeyb/Downloads/bsd.rd  -m 2G -i 1 -d /home/sergeyb/Download
# doas cu /dev/ttyp4
~^D
doas vmctl stop "openbsd"

expect: (I)nstall, (U)pgrade, (A)utoinstall or (S)hell?
send: "I\r"
expect: Terminal type? [vt220]
send: "\r"
expect: "System hostname? (short form, e.g. 'foo')"
send: "foo\r"
expect: "Which network interface do you wish to configure? (or 'done') [vio0]"
send: "\r"
expect: "IPv4 address for vio0? (or 'dhcp' or 'none') [dhcp]"
send: "\r"
expect: "IPv6 address for vio0? (or 'autoconf' or 'none') [none]"
send: "\r"
expect: "Which network interface do you wish to configure? (or 'done') [done]"
send: "\r"
expect: "DNS domain name? (e.g. 'example.com') [my.domain]"
send: "\r"
expect: "DNS nameservers? (IP address list or 'none') [none]"
send: "\r"
expect: "Password for root account? (will not echo)"
send: "1q2w3e\r"
expect: "Password for root account? (again)"
send: "1q2w3e\r"
expect: "Start sshd(8) by default? [yes]"
send: "\r"
expect: "Change the default console to com0? [yes]"
send: "\r"
expect: "Which speed should com0 use? (or 'done') [9600]"
send: "115200\r"
expect: "Setup a user? (enter a lower-case loginname, or 'no') [no]"
send: "\r"
expect: "Allow root ssh login? (yes, no, prohibit-password) [no]"
send: "\r"
expect: "Which disk is the root disk? ('?' for details) [sd0]"
send: "\r"
expect: "Use (W)hole disk MBR, whole disk (G)PT or (E)dit? [whole]"
send: "\r"
expect: "Use (A)uto layout, (E)dit auto layout, or create (C)ustom layout? [a]"
send: "\r"
expect: "Which disk do you wish to initialize? (or 'done') [done]"
send: "\r"
expect: "Location of sets? (disk http or 'done') [http]"
send: "disk"
---
send: "~^D"

```


#### Similar tools

- Packer
- [Anita, an Automated NetBSD Installation and Test Application](http://www.gson.org/netbsd/anita/)
- virt-install

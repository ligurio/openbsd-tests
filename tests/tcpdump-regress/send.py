#/usr/local/bin/env python

import subprocess as sub
import logging
import time
import os
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)

iface = 'em0'

# sudo modprobe dummy
# sudo ip link set name eth10 dev dummy0
# ip link show eth10
# ip link set dev eth10 up

def chck(pkt, re_output, debug=False):

        if os.getuid() != 0:
            print "root priveleges is required"
            exit(1)

        if isinstance(pkt, list):
            packet_count = len(pkt)
        else:
            packet_count = 1

        p = sub.Popen(('doas', 'tcpdump', '-i', iface, '-c', str(packet_count)),
                        stdout=sub.PIPE, stderr=sub.PIPE)
        while True:
            if p.poll() is None:
                break

        # time.sleep(2)
        send(pkt, iface=iface, verbose=1)

	pkt.show()
	print p.communicate()[0]
        p.wait()


def main():

        # 802_11 ???
 
        print "arp"
        packet = ARP(op=ARP.who_has, psrc='192.168.1.2', pdst='192.168.1.1')
        re_output = ""
        chck(packet, re_output)

        # atalk
        # atm

        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/bgp.py
        #print "bgp"
        #packet = IP()/TCP()/BGPUpdate()
        #re_output = ""
        #chck(packet, re_output)

        print "bootp"
        packet = BOOTP()
        re_output = ""
        chck(packet, re_output)

        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/carp.py
        #print "carp"
        #packet = CARP()
        #re_output = ""
        #chck(packet, re_output)

        # cdp
        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/cdp.py
        #print "cdp"
        #packet = CDP()
        #re_output = ""
        #chck(packet, re_output)

        # cnfp
        # decnet

        print "dhcp6"
        packet = DHCP(options=[("message-type", "discover"), "end"])
        re_output = ""
        chck(packet, re_output)

        print "domain"
        packet = IP() / UDP(dport=53) / DNS(qd=DNSQR(qname="google.com"))
        re_output = ""
        chck(packet, re_output)

        packet = TCP(dport=53)
        re_output = ""
        chck(packet, re_output)

        # dvmrp
        # enc
        
        print "ether"
        packet = Ether()
        re_output = ""
        chck(packet, re_output)

        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/etherip.py
        #print "etherip"
        #packet = EtherIP()
        #re_output = ""
        #chck(packet, re_output)

        # fddi
        # frag6
        
        print "gre"
        packet = GRE()
        re_output = ""
        chck(packet, re_output)

        # https://github.com/secdev/scapy/blob/master/scapy/contrib/gtp.py
        #print "gtp"
        #packet = GTP()
        #re_output = ""
        #chck(packet, re_output)

        print "hsrp"
        packet = Ether(src=’00:00:0C:07:AC:02’, dst=’01:00:5E:00:00:02’ ) /
                    IP(dst=’224.0.0.2’, src='attacerIP', ttl=1) / 
                    UDP() /
                    HSRP(priority=230, virtualIP='virtualIP'), inter=3, loop=1)
        re_output = ""
        chck(packet, re_output)

        # iapp ???

        print "icmp"
        packet = IP(dst="192.168.1.1", ttl=128) / ICMP()
        re_output = ""
	tcpdump_opt = "-v ip and 'ip[8]>128'"
        chck(packet, re_output)

        print "icmp6"
        packet = ICMPv6EchoRequest()
        re_output = ""
        chck(packet, re_output)

        # igrp

        # ike
        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/ikev2.py
        #print "ike"
        #packet = IKEv2()
        #re_output = ""
        #chck(packet, re_output)

        print "ip"
        packet = IP()
        re_output = ""
        chck(packet, re_output)

        # ip6
        # ip6opts
        # ipsec
        # ipx
        # isoclns
        # krb

        print "l2tp"
        packet = L2TP()
        re_output = ""
        chck(packet, re_output)

        print "llc"
        packet = LLC()
        re_output = ""
        chck(packet, re_output)

        # lldp
        # https://github.com/secdev/scapy/blob/master/scapy/contrib/lldp.py
        #load_contrib('lldp')
        #print "lldp"
        #packet = LLDP()
        #re_output = ""
        #chck(packet, re_output)

        # lwres

        # https://github.com/secdev/scapy/blob/master/scapy/contrib/mpls.py
        #print "mpls"
        #packet = MPLS()
        #re_output = ""
        #chck(packet, re_output)

        # netbios
        # nfs

        print "ntp"
        packet = IP()/UDP(dport=123)/Raw(load=str("\x17\x00\x03\x2a") + str("\x00")*4)
        re_output = ""
        chck(packet, re_output)

        # null
        # ofp

        # https://github.com/secdev/scapy/blob/master/scapy/contrib/ospf.py
        #load_contrib('ospf')
        #print "ospf"
        #packet = OSPF()
        #re_output = ""
        #chck(packet, re_output)

        # ospf6
        # pflog
        # pfsync
        # pim

        print "ppp"
        packet = PPP()
        re_output = ""
        chck(packet, re_output)

        #print "radius"
        #packet = Radius()
        #re_output = ""
        #chck(packet, re_output)

        print "raw"
        packet = Raw()
        re_output = ""
        chck(packet, re_output)

        print "rip"
        packet = RIP()
        re_output = ""
        chck(packet, re_output)

        # ripng
        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/ripng.py
        #print "ripng"
        #packet = RIPNG()
        #re_output = ""
        #chck(packet, re_output)

        # rt6
        # sl
        # slow

        print "snmp"
        packet = UDP(sport=161) / SNMP(community="private", PDU="")
        re_output = ""
        chck(packet, re_output)

        print "stp"
        packet = STP()
        re_output = ""
        chck(packet, re_output)

        # sunrpc

        print "tcp"
        packet = TCP()
        re_output = ""
        chck(packet, re_output)

        print "tftp"
        packet = IP()/UDP()/TFTP()/TFTP_DATA(block=0)
        re_output = ""
        chck(packet, re_output)

        # timed

        print "udp"
        packet = UDP()
        re_output = ""
        chck(packet, re_output)

        # udpencap

        # vqp
        # https://github.com/secdev/scapy/blob/master/scapy/contrib/vqp.py
        #print "vqp"
        #packet = VQP()
        #re_output = ""
        #chck(packet, re_output)

        print "vrrp"
        packet = UDP()
        re_output = ""
        chck(packet, re_output)

        # wb

if __name__ == "__main__":
        main()

"""
from scapy.all import *
import urllib
import urlparse
import threading
import re

proxy   = "http://my.proxy.server:8080"
proxyIP = "1.2.3.4" # IP address of proxy

# list of URLs
urls = ["http://www.bbc.co.uk/news",
        "http://www.google.co.uk"]

packets = []

# packet callback
def pkt_callback(pkt):
    packets.append(pkt) # save the packet

# monitor function
def monitor(fname):
    del packets[:]
    bpf_filter = "tcp and host " + proxyIP       # set this filter to capture the traffic you want
    sniff(timeout=60, prn=pkt_callback, filter=bpf_filter, store=0)
    f=open(fname+".data", 'w')
    for pkt in packets:
        f.write(repr(pkt))  # or just save the raw packet data instead
        f.write('\n')
    f.close()

for url in urls:
    print "capturing: " + url
    mon = threading.Thread(target=monitor, args=(re.sub(r'\W+', '', url),))
    mon.start()
    data = urllib.urlopen(url, proxies={'http': proxy})
    # this line gets IP address of url host, might be helpful 
    # addr = socket.gethostbyname(urlparse.urlparse(data.geturl()).hostname)
    mon.join()
"""

#/usr/local/bin/env python

import subprocess as sub
import logging
import time
import os
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)

# sudo modprobe dummy
# sudo ip link set name eth10 dev dummy0
# ip link show eth10
# ip link set dev eth10 up

DEF_IFACE = 'em0'

def chck(pkt, re_output, debug=False):

        #if os.getuid() != 0:
        #    print "root priveleges is required"
        #    exit(1)

        #if isinstance(pkt, list):
        #    packet_count = len(pkt)
        #else:
        #    packet_count = 1

        pcap_name = 'sample.pcap'

        wrpcap(pcap_name, pkt)
        tcpdump_cmd = [ 'doas', 'tcpdump', '-r', pcap_name ]
        # tcpdump_cmd = [ 'doas', 'tcpdump', '-i', DEF_IFACE, '-c', str(packet_count) ]
        p = sub.Popen(tcpdump_cmd, stdout=sub.PIPE, stderr=sub.PIPE)

        while True:
            if p.poll() is None:
                break

        # time.sleep(2)
        # send(pkt, iface=DEF_IFACE, verbose=1)

        if debug:
            pkt.show()
            print p.communicate()[0]
        p.wait()

        os.remove(pcap_name)

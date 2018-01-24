#/usr/local/bin/env python

import logging
import os
import subprocess as sub
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)

def chck(pkt, re_output):

        pcap_name = 'sample.pcap'
        wrpcap(pcap_name, pkt)
        tcpdump_cmd = [ 'tcpdump', '-A', '-t', '-r', pcap_name ]
        p = sub.Popen(tcpdump_cmd, stdout=sub.PIPE, stderr=sub.PIPE)
        output = p.communicate()[0]
        retcode = p.wait()

        if retcode != 0 or re_output != output:
            pkt.show()
            print output, re_output
            return 1
        else:
            os.remove(pcap_name)
            return 0

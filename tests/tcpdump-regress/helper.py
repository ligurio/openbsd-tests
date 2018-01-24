#/usr/local/bin/env python

import subprocess as sub
import logging
import time
import os
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

        print output, "\n", re_output
        if retcode != 0 or re_output is not output:
            pkt.show()
            print output, "\n", re_output
            return 1
        else:
            os.remove(pcap_name)
            return 0

#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        print "icmp"
        packet = IP(dst="192.168.1.1", ttl=128) / ICMP()
        re_output = ""
        tcpdump_opt = "-v ip and 'ip[8]>128'"
        helper.chck(packet, re_output, debug=True)


if __name__ == "__main__":
        main()

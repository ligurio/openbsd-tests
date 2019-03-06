#!/usr/bin/env python

import sys
from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    udp_packet = IP() / UDP(dport=53) / DNS(qd=DNSQR(qname="google.com"))
    udp_re_output = ""
    udp_retcode = helper.chck(udp_packet, udp_re_output)

    tcp_packet = TCP(dport=53)
    tcp_re_output = ""
    tcp_retcode = helper.chck(tcp_packet, tcp_re_output)

    if tcp_retcode and udp_retcode:
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == "__main__":

    main()

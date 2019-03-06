#!/usr/bin/env python

from scapy.all import IP, TCP, BGPUpdate
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/bgp.py
        packet = IP()/TCP()/BGPUpdate()
        re_output = ""
        helper.chck(packet, re_output)


if __name__ == "__main__":
    main()

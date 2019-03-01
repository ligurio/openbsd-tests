#!/usr/bin/env python

import sys
from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    packet = IP()/UDP(dport=123)/Raw(load=str("\x17\x00\x03\x2a") + str("\x00")*4)
    re_output = ""
    exitcode = helper.chck(packet, re_output)
    sys.exit(exitcode)


if __name__ == "__main__":
    main()

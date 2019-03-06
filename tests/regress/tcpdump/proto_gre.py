#!/usr/bin/env python

import sys
from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    # packet = GRE()
    packet = IP(dst="")/GRE(proto=254) / \
        "\x83\x1b \x01\x06\x12\x01\xff\x07\xff\xff\xff\xff\xff\xff\xff \xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01\x07 \x00\x00"
    re_output = "[|ether]\n"
    exitcode = helper.chck(packet, re_output)
    sys.exit(exitcode)


if __name__ == "__main__":
    main()

#!/usr/bin/env python

import sys
from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    packet = UDP(sport=161) / SNMP(community="private", PDU="")
    re_output = "00:00:30:0c:02:01 sap 70 > 00:a1:00:a2:00:16 sap 07 rr (r=52,C) len=4\n" \
                ".private\n"
    exitcode = helper.chck(packet, re_output)
    sys.exit(exitcode)


if __name__ == "__main__":
    main()

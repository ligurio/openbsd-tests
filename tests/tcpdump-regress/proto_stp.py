#!/usr/bin/env python

import sys
from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    packet = STP()
    re_output = "00:00:00:00:00:00 > 00:00:00:00:00:00 null I (s=0,r=0,C) len=17\n" \
                ".....................\n"
    exitcode = helper.chck(packet, re_output)
    sys.exit(exitcode)


if __name__ == "__main__":
    main()

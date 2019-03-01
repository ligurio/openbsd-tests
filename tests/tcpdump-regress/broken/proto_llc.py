#!/usr/bin/env python

import sys
from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    packet = LLC()
    re_output = "[|llc]\n" \
                "00:00:00:00:00:00 00:02:00:0e:00:00 0000 0:"
    exitcode = helper.chck(packet, re_output)
    sys.exit(exitcode)


if __name__ == "__main__":
    main()

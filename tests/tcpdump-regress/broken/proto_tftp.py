# /usr/local/bin/env python

import sys
from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    packet = IP()/UDP()/TFTP()/TFTP_DATA(block=0)
    re_output = ""
    exitcode = helper.chck(packet, re_output)
    sys.exit(exitcode)


if __name__ == "__main__":
    main()

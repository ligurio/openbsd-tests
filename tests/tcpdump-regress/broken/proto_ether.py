#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)
import sys

def main():

        packet = Ether()
        re_output = "08:00:27:e6:42:5a Broadcast loopback 14:\n\n"
        exitcode = helper.chck(packet, re_output)
        sys.exit(exitcode)

if __name__ == "__main__":
        main()

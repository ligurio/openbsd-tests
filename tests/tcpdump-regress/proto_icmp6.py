#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)
import sys

def main():

        packet = ICMPv6EchoRequest()
        re_output = "[|ether]\n"
        exitcode = helper.chck(packet, re_output)
        sys.exit(exitcode)

if __name__ == "__main__":
        main()

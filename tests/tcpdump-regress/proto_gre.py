#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)
import sys

def main():

        #packet = GRE()
        packet = IP(dst="")/GRE(proto=254)/"\x83\x1b \x01\x06\x12\x01\xff\x07\xff\xff\xff\xff\xff\xff\xff \xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01\x07 \x00\x00"
        re_output = "[|ether]\n"
        exitcode = helper.chck(packet, re_output)
        sys.exit(exitcode)


if __name__ == "__main__":
        main()

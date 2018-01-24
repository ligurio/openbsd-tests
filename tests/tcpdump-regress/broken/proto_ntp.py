#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)
import sys

def main():

        packet = IP()/UDP(dport=123)/Raw(load=str("\x17\x00\x03\x2a") + str("\x00")*4)
        re_output = ""
        exitcode = helper.chck(packet, re_output)
        sys.exit(exitcode)


if __name__ == "__main__":
        main()

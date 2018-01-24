#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)
import sys

def main():

        packet = STP()
        re_output = "00:00:00:00:00:00 > 00:00:00:00:00:00 null I (s=0,r=0,C) len=17\n" \
                    ".....................\n"
        exitcode = helper.chck(packet, re_output)
        sys.exit(exitcode)

if __name__ == "__main__":
        main()

#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)
import sys

def main():

        packet = TCP()
        re_output = "(000) ret      #65535"
        exitcode = helper.chck(packet, re_output)
        sys.exit(exitcode)

if __name__ == "__main__":
        main()

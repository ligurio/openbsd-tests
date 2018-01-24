#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)

def main():

        # https://github.com/secdev/scapy/blob/master/scapy/contrib/lldp.py
        #load_contrib('lldp')
        #packet = LLDP()
        #re_output = ""
        #chck(packet, re_output)


if __name__ == "__main__":
        main()

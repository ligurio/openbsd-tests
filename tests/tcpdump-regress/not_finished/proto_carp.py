#!/usr/bin/env python

from scapy.all import CARP
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/carp.py
        packet = CARP()
        re_output = ""
        helper.chck(packet, re_output)


if __name__ == "__main__":
    main()

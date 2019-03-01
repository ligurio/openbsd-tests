#!/usr/bin/env python

from scapy.all import CDP
from helper import chck
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/cdp.py
        packet = CDP()
        re_output = ""
        chck(packet, re_output)


if __name__ == "__main__":
    main()

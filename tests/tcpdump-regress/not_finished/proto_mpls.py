#!/usr/bin/env python

from scapy.all import MPLS
from helper import chck
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        # https://github.com/secdev/scapy/blob/master/scapy/contrib/mpls.py
        packet = MPLS()
        re_output = ""
        chck(packet, re_output)


if __name__ == "__main__":
    main()

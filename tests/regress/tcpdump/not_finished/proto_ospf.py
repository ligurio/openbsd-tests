#!/usr/bin/env python

from scapy.all import OSPF
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    # https://github.com/secdev/scapy/blob/master/scapy/contrib/ospf.py
    # load_contrib('ospf')
    print("ospf")
    packet = OSPF()
    re_output = ""
    helper.chck(packet, re_output)


if __name__ == "__main__":
    main()

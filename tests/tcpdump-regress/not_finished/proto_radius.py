#!/usr/bin/env python

from scapy.all import *
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

    print("radius")
    packet = Radius()
    re_output = ""
    helper.chck(packet, re_output)


if __name__ == "__main__":
    main()

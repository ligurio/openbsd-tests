#!/usr/bin/env python

from scapy.all import GTP
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        # https://github.com/secdev/scapy/blob/master/scapy/contrib/gtp.py
        packet = GTP()
        re_output = ""
        helper.chck(packet, re_output)


if __name__ == "__main__":
    main()

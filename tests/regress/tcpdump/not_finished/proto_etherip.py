#!/usr/bin/env python

from scapy.all import EtherIP
import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        # https://github.com/secdev/Scapy/blob/master/scapy/contrib/etherip.py
        packet = EtherIP()
        re_output = ""
        helper.chck(packet, re_output)


if __name__ == "__main__":
    main()

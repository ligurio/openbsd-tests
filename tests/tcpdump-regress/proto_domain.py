#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)


def main():

        print "domain"
        packet = IP() / UDP(dport=53) / DNS(qd=DNSQR(qname="google.com"))
        re_output = ""
        helper.chck(packet, re_output)

        packet = TCP(dport=53)
        re_output = ""
        helper.chck(packet, re_output)


if __name__ == "__main__":

        main()

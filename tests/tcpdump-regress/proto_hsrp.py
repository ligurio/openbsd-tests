#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)

def main():

        print "hsrp"
        #packet = Ether(src="00:0C:07:AC:02", dst="00:5E:00:00:02")  /
        #            IP(dst="224.0.0.2", src='attacerIP', ttl=1)     / 
        #            UDP()                                           /
        #            HSRP(priority=230, virtualIP='virtualIP')
        #re_output = ""
        #helper.chck(packet, re_output)


if __name__ == "__main__":
        main()

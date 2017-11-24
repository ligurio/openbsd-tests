#/usr/local/bin/env python

import helper
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
logging.getLogger("scapy.runtime").setLevel(logging.WARNING)

def main():

	# https://github.com/secdev/Scapy/blob/master/scapy/contrib/carp.py
	print "carp"
	#packet = CARP()
	#re_output = ""
	#helper.chck(packet, re_output)

if __name__ == "__main__":
        main()

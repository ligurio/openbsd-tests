Additional regression tests for OpenBSD 
---------------------------------------

Sergey Bronnikov, BSD License

HOWTO
-----

* ftp -o - http://github.com/ligurio/openbsd-tests/openbsd-tests-latest.tar.gz
		| tar xvzf - && cd openbsd-tests && make

RECOMMENDATIONS FOR HARDWARE
----------------------------

* IPMI
* Hardware watchdog
	* J-Works WDT205
	* Yocto-WatchdogDC
	* Internet WatchDog on eBay
	* WatchDog USB Timer

TODO
-----
* add TAP support (sharness https://github.com/mlafeldt/sharness and bsd.tap.mk)
* installation on node with Linux (depinguinator for OpenBSD?)
* openup http://www.mtier.org/solutions/apps/openup/
* add support of ONLY_FOR_ARCHS and NOT_FOR_ARCHS
* report (http://www.openbsd.org/report.html)
	/var/log/Xorg.0.log
	/var/run/dmesg.boot
	dmesg(8), usbdevs(8), pcidump(8), and acpidump(8)

Additional regression tests for OpenBSD
Sergey Bronnikov, BSD License

HOWTO
	# ftp -o - http://github.com/ligurio/openbsd-tests/openbsd-tests-latest.tar.gz \
		| tar xvzf - && cd openbsd-tests && make

RECOMMENDATIONS FOR HARDWARE
	IPMI https://github.com/miniwark/miniwark-howtos/wiki/Hardware-Watchdog-Timer-setup-on-Ubuntu-12.04
	J-Works WDT205
	Yocto-WatchdogDC
	Internet WatchDog on eBay
	WatchDog USB Timer

TODO
	add TAP support (sharness https://github.com/mlafeldt/sharness and bsd.tap.mk)
	installation on node with Linux (depinguinator for OpenBSD?)
	openup http://www.mtier.org/solutions/apps/openup/
	add support of ONLY_FOR_ARCHS and NOT_FOR_ARCHS

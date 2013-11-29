# $ OpenBSD		$

TEST = isic
PKG_REQUIRED = isic

REGRESS_LOG = ${.CURDIR}/isic-`date +%Y-%m-%d`.log
REGRESS_ROOT_TARGETS  = run-regress-isic-tcp
REGRESS_ROOT_TARGETS += run-regress-isic-udp
REGRESS_ROOT_TARGETS += run-regress-isic-icmp
REGRESS_TARGETS  = run-regress-isic-tcp
REGRESS_TARGETS += run-regress-isic-udp
REGRESS_TARGETS += run-regress-isic-icmp

run-regress-isic-tcp:
	@tcpsic -s rand -d localhost -x 2 -m 1000 -T 30 -t 50 -p 20

run-regress-isic-udp:
	@udpsic -s rand -d localhost -I 90 -U 20 -p 20

run-regress-isic-icmp:
	@icmpsic -s rand -d localhost -i 6 -p 20

.include <../infrastructure/bsd.autotest.mk>

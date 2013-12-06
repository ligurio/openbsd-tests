COMMENT =	OpenSSL

REGRESS_LOG = ${.CURDIR}/openssl.log
REGRESS_TARGETS = run-regress-openssl-md5 run-regress-openssl-rand \
		  run-regress-openssl-usage
REGRESS_SKIP_TARGETS = run-regress-openssl-usage

run-regress-openssl-md5:
	@echo -n "OpenSSL MD5: "
	@openssl md5 Makefile

run-regress-openssl-rand:
	@echo -n "OpenSSL random: "
	@openssl rand -base64 12

run-regress-openssl-usage:
	@echo -n "OpenSSL usage: "
	@openssl help

.include <../infrastructure/mk/bsd.autotest.mk>

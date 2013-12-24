TEST =	openssl

REGRESS_LOG = ${.CURDIR}/${TEST}-`date +%Y-%m-%d-%MM-%SS`.log
REGRESS_TARGETS = run-regress-${TEST}-md5 run-regress-${TEST}-rand

run-regress-${TEST}-md5:
	@echo -n "OpenSSL MD5: "
	@openssl md5 Makefile

run-regress-${TEST}-rand:
	@echo -n "OpenSSL random: "
	@openssl rand -base64 12

.include <../infrastructure/mk/bsd.autotest.mk>

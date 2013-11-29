
REGRESS_LOG = ${.CURDIR}/openssl-`date +%Y-%m-%d`-`uname -m`.log
REGRESS_TARGETS = run-regress-openssl-md5 run-regress-openssl-rand \
		  run-regress-openssl-usage
REGRESS_SKIP_TARGETS = run-regress-openssl-usage

run-regress-openssl-md5:
	@echo "OpenSSL MD5:"
	@time openssl md5 Makefile

run-regress-openssl-rand:
	@echo "OpenSSL random:"
	@time openssl rand -base64 12

run-regress-openssl-usage:
	@echo "OpenSSL usage:"
	@time openssl help

.include <../infrastructure/mk/bsd.autotest.mk>

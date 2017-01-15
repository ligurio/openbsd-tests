# $ OpenBSD $

TEST = config
PKG_REQUIRED = zzuf
REGRESS_LOG = ${.CURDIR}/config-`date +%Y-%m-%d`.log
REGRESS_TARGETS  = run-regress-dhcpd-config
REGRESS_TARGETS += run-regress-iked-config
REGRESS_TARGETS += run-regress-ksh-config
REGRESS_TARGETS += run-regress-ldapd-config
REGRESS_TARGETS += run-regress-nginx-config
REGRESS_TARGETS += run-regress-npppd-config
REGRESS_TARGETS += run-regress-ntpd-config
REGRESS_TARGETS += run-regress-smtpd-config
REGRESS_TARGETS += run-regress-vi-config
REGRESS_SLOW_TARGETS  = run-regress-nginx-config
REGRESS_SLOW_TARGETS += run-regress-smtpd-config

REGRESS_ROOT_TARGETS = run-regress-iked-config
REGRESS_SLOW_TARGETS = run-regress-ldapd-config
OPTIONS = "-s0:100 -r0.01"

run-regress-smtpd-config:
	@echo -n "smtpd: "
	@zzuf ${OPTIONS} smtpd -n -f /etc/mail/smtpd.conf >/dev/null 2>&1
	@test -e smtpd.core && echo "FAIL" || echo "OK"

run-regress-ldapd-config:
	@echo -n "ldapd: "
	@zzuf ${OPTIONS} ldapd -n -f /etc/ldapd.conf >/dev/null 2>&1
	@test -e ldapd.core && echo "FAIL" || echo "OK"

run-regress-dhcpd-config:
	@echo -n "dhcpd: "
	@zzuf ${OPTIONS} dhcpd -n -f /etc/dhcpd.conf >/dev/null 2>&1
	@test -e dhcpd.core && echo "FAIL" || echo "OK"

run-regress-ntpd-config:
	@echo -n "ntpd: "
	@zzuf ${OPTIONS} ntpd -n -f /etc/ntpd.conf >/dev/null 2>&1
	@test -e ntpd.core && echo "FAIL" || echo "OK"

run-regress-nginx-config:
	@echo -n "nginx: "
	@zzuf ${OPTIONS} nginx -t -c /etc/nginx/nginx.conf >/dev/null 2>&1
	@test -e nginx.core && echo "FAIL" || echo "OK"

run-regress-npppd-config:
	@echo -n "npppd: "
	@zzuf ${OPTIONS} npppd -n -f /etc/npppd/npppd.conf >/dev/null 2>&1
	@test -e npppd.core && echo "FAIL" || echo "OK"

run-regress-iked-config:
	@echo -n "iked: "
	@zzuf ${OPTIONS} iked -n -f /etc/iked.conf >/dev/null 2>&1
	@test -e iked.core && echo "FAIL" || echo "OK"

run-regress-ksh-config:
	@echo -n "ksh: "
	@zzuf ${OPTIONS} ksh /etc/ksh.kshrc >/dev/null 2>&1
	@test -e ksh.core && echo "FAIL" || echo "OK"

run-regress-vi-config:
	@echo -n "vi: "
	@zzuf -s0:10 -r0.01 vi Makefile >/dev/null 2>&1
	@test -e vi.core && echo "FAIL" || echo "OK"

.include <../infrastructure/bsd.autotest.mk>

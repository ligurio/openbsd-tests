TEST = x11perf

X11_REQUIRED = Yes

REGRESS_LOG = ${.CURDIR}/${TEST}-`date +%Y-%m-%d-%MM-%SS`.log
REGRESS_TARGETS = run-regress-test-${TEST}
REGRESS_MAX_TIME ?= 3600

run-regress-test-${TEST}:
	@x11perf -all

.include <../infrastructure/mk/bsd.autotest.mk>

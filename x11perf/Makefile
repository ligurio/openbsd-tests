PKG_REQUIRED = x11perf
REGRESS_LOG = ${.CURDIR}/${TEST}-`date +%Y-%m-%d`.log
REGRESS_TARGETS = run-regress-test-x11perf

run-regress-test-x11perf:
        @x11perf -all

.include <../infrastructure/bsd.autotest.mk>

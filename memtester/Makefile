COMMENT= test for memory faults

PKG_REQUIRED = memtester
LOOP = 1
AMOUNT != sysctl -n hw.physmem
REGRESS_LOG = ${.CURDIR}/memtester-`date +%Y-%m-%d`.log
REGRESS_TARGETS = run-regress-test-memtester

run-regress-test-memtester:
	echo "Total RAM: ${AMOUNT}"
	@memtester ${AMOUNT}B ${LOOP} | tee ${REGRESS_LOG}

.include <../infrastructure/mk/bsd.autotest.mk>

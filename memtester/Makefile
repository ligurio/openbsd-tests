COMMENT= test for memory faults

PKG_REQUIRED = memtester
LOOP = 1
AMOUNT != sysctl -n hw.physmem
REGRESS_LOG = ${.CURDIR}/memtester.log
REGRESS_TARGETS = run-regress-test-memtester

run-regress-test-memtester:
	echo "Total RAM: ${AMOUNT}"
	@memtester ${AMOUNT}B ${LOOP}

.include <../infrastructure/mk/bsd.autotest.mk>

# $ 	OpenBSD $

PROG=      tcpstress
LDFLAGS += -lpthread

TEST = ${PROG}
REGRESS_LOG = ${.CURDIR}/tcpstress-`date +%Y-%m-%d`.log
REGRESS_TARGETS=        run-regress-${PROG}

run-regress-${PROG}: ${PROG}
	./${PROG} -t 10 -a 5 -m 10 localhost 100

.include <../infrastructure/bsd.autotest.mk>

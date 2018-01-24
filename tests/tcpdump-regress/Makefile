ARGS !=			cd ${.CURDIR} && ls proto_*.py
TARGETS ?=		${ARGS}
REGRESS_TARGETS =	${TARGETS:S/^/run-regress-/}

.for a in ${ARGS}
run-regress-$a: $a
	@echo '\n======== $@ ========'
	@python $a
.endfor

.include <bsd.regress.mk>

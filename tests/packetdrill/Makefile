PACKETDRILL ?= /usr/local/bin/packetdrill
TESTPLAN ?= testplans/testplan-openbsd.json
PYTHON ?= python3

run:
	${PYTHON} scripts/run-tests.py \
		--testplan ${TESTPLAN} \
		--packetdrill ${PACKETDRILL} \
		--test_timeout 30

discovery:
	${PYTHON} scripts/run-tests.py --discovery

.PHONY: run discovery

PYTEST_OPT += --junit-xml=report-`date +%Y-%m-%d`.xml

run:
	make -C ../tools
	pytest ${PYTEST_OPT}

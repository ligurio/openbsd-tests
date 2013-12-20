ARCH!=uname -m
DATE!=date +%Y-%m-%d_%H-%M
HOSTNAME!=hostname
CLEANFILES+=*.log

REGRESS_DIR ?= /home/sergeyb/source/openbsd-tests
_SHSCRIPT = sh ${REGRESS_DIR}/infrastructure/bin

# See http://www.openbsd.org/report.html
# TODO: packages version
REPORT_LIST+=/var/run/dmesg.boot
.if exists(${X11BASE}/share/mk/bsd.xconf.mk)
	REPORT_LIST+=/var/log/Xorg.0.log
.endif
REPORT_NAME ?= ${.CURDIR}/report_${DATE}_${ARCH}_${HOSTNAME}.log

.MAIN: all
all: preparation upgrade regress report cleanup

preparation:
. if defined(PKG_REQUIRED)
	@for package in ${PKG_REQUIRED}; do \
		(set -e; echo -n "install $${package} "; pkg_add $${package} && echo "done" || exit 1); \
	done;
. endif
. if defined(REPORT_HTTP)
	@pkg_add curl;
. endif

upgrade:
. if !defined(NO_UPDATE)
	@${_SHSCRIPT}/sysupgrade full;
. elif
	echo "Run tests on installed OpenBSD version wo update"
. endif

report:
.if defined(REGRESS_LOG) && exists(${REGRESS_LOG})
	@echo "Make report"
	@${_SHSCRIPT}/tapperize ${REGRESS_LOG} > ${REPORT_NAME}
.endif
. if defined(REPORT_FTP)
	@ftp put ${REPORT_NAME} ${REPORT_FTP}
	@echo "Send report"
. elif defined(REPORT_MAIL)
	@(sysctl -n kern.version && cat ${REPORT_NAME}) | /usr/bin/mail -s "REGRESS: `uname -a`" ${REPORT_MAIL}
	@echo "Send report"
. elif defined(REPORT_HTTP)
	@curl --max-time 10 -T ${REPORT_NAME} ${REPORT_HTTP} > /dev/null 2>&1
	@echo "Send report"
. else
	@echo "Report was not send"
. endif

cleanup:
. if defined(PKG_REQUIRED)
	@for package in ${PKG_REQUIRED}; do \
		(set -e; pkg_delete -D dependencies $${package} || exit 1); \
	done;
. endif

.include <bsd.regress.mk>

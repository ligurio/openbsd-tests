# $	OpenBSD		$

PROG = 	fusexmp
LDFLAGS = -lfuse

PKG_REQUIRED = fsx fsstress

TEST = fuse
REGRESS_LOG = ${.CURDIR}/${TEST}-`date +%Y-%m-%d-%MM-%SS`.log
REGRESS_TARGETS = run-regress-fuse-hdd
REGRESS_ROOT_TARGETS=   run-regress-fuse-hdd

run-regress-fuse-fsstress-hdd: fusexmp
	#@-mkdir /mnt/fuse
	@fusexmp /mnt/fuse -d
	@mkdir /mnt/fuse/tmp/fstest
	fsstress -d /mnt/fuse/tmp/fstest

run-regress-fuse-fsx-hdd: fusexmp
	#@-mkdir /mnt/fuse
	@fusexmp /mnt/fuse -d
	@mkdir /mnt/fuse/tmp/fstest
	fsx # while [ true ]; do sync; done

run-regress-fuse-vnd: fusexmp
	#@-mkdir /mnt/fuse
	mkdir mfs
	mount_mfs -s 800 swap ./mfs
	@fusexmp /mnt/fuse -d
	fsx /mnt/fuse

run-regress-fuse-mfs: fusexmp
	#@-mkdir /mnt/fuse
	mkdir mfs
	mount_mfs -s 800 swap ./mfs
	@fusexmp /mnt/fuse -d
	fsx /mnt/fuse

run-regress-fuse-tmpfs: fusexmp
	@fusexmp
	fsx /mnt/fuse

.include <../infrastructure/mk/bsd.autotest.mk>

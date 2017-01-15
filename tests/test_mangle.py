

import subprocess

@pytest.mark.slowtest
def test_x11perf():
    openssl_cmd = ["openssl", "speed"]
    proc = subprocess.Popen(openssl_cmd)
    proc.wait()
    assert proc.returncode == 0

IMAGE=$(mktemp -t imageXXXXXXXXXX);
MNT=./stress

validate()
{
	vnconfig vnd0 ${IMAGE} > /dev/null 2>&1
	(! ( test -e stress)) && mkdir ${MNT}
	mount /dev/vnd0a ${MNT}
	ls -R ${MNT}
	umount ${MNT}
	vnconfig -u vnd0
}

preparation()
{
	dd if=/dev/zero of=${IMAGE} bs=1m count=700 > /dev/null 2>&1 &&
	vnconfig vnd0 ${IMAGE} > /dev/null 2>&1 &&
	fdisk -iy vnd0 > /dev/null 2>&1 &&
	disklabel -A -w vnd0 > /dev/null 2>&1 &&
	newfs /dev/rvnd0a > /dev/null 2>&1;
	if [ $? -ne 0 ]; then echo "Success"; else echo "Failed"; fi
	vnconfig -u vnd0
}

echo -n "Preparation: "
preparation && echo "OK"
echo -n "Validation: "
validate && echo "OK"

echo -n "Run mangle: "
./mangle ${IMAGE} 1024 && echo "OK"

echo -n "Validation: "
validate && echo "OK"

rm -rf ${IMAGE} ${MNT}

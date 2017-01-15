#!/bin/ksh

level=$1
chunks=$2
mnt=stress

if [ -z $level ] || [ -z $chunks ]; then exit 1; fi

if [ ! -e /usr/local/bin/fsstress ]; 
then 
        echo "fsstress not found, you can install it by 'pkg_add fsstress'";
        exit 1; 
fi

trap 'cleanup' INT

prepare_chunks()
{
	for i in `jot 4 1 4`; 
	do
		echo -n "Chunk $i: "
		dd if=/dev/zero of=disk$i bs=1m count=10 > /dev/null 2>&1 &&
		vnconfig vnd$(($i-1)) disk$i > /dev/null 2>&1 &&
		fdisk -iy vnd$(($i-1)) > /dev/null 2>&1 &&
		printf "a\n\n\n\nRAID\nw\nq\n\n" | disklabel -E vnd$(($i-1)) > /dev/null 2>&1;
		if [ $? -ne 0 ]; then echo "Success"; else echo "Failed"; fi  
	done;
}

stress()
{
	echo "Stress..."
	#mkdir stress && mount /dev/$sd stress && fsstress -d stress -v -p 5 -n 50 -S || exit;
	mkdir stress 
	mount -t ffs /dev/${sd}i stress
	fsstress -d stress -v -p 5 -n 50 -S || exit;
	umount /dev/${sd}i
}

cleanup()
{
	echo "Cleanup..."
	bioctl -d ${sd}
	for v in `vnconfig -l | egrep -v "not in use" | sed 's;^\(vnd[0-9]*\):.*;\1;' | tr "\n" " "`;
	do
	    vnconfig -u $v;
	done;
	rm -f disk1 disk2 disk3 disk4 stress
}

case $level in
	raid0)
		if [ $chunks -lt 2 ]; then echo "Incorrect amount of chunks";  exit 1; fi
		prepare_chunks;
		bioctl -c 0 -l /dev/vnd0a,/dev/vnd1a,/dev/vnd2a,/dev/vnd3a softraid0;
	;;
	raid1)
	if [ $chunks -lt 2 ]; then exit 1; fi
		echo "Panic here"
		#prepare_chunks;
		#bioctl -c 1 -l /dev/vnd0a,/dev/vnd1a,/dev/vnd2a softraid0;
	;;
	raid4)
		if [ $chunks -lt 3 ]; then exit 1; fi
		echo "Panic here"
		#prepare_chunks;
		#bioctl -c 4 -l /dev/vnd0a,/dev/vnd1a,/dev/vnd2a softraid0;
	;;
	raid5)
		if [ $chunks -lt 3 ]; then exit 1; fi
		echo "Panic here"
		#prepare_chunks;
		#bioctl -c 5 -l /dev/vnd0a,/dev/vnd1a,/dev/vnd2a softraid0;
	;;
	concat)
		if [ $chunks -lt 2 ]; then exit 1; fi
		prepare_chunks;
		bioctl -c c -l /dev/vnd0a,/dev/vnd1a,/dev/vnd2a,/dev/vnd3a softraid0;
	;;
	crypto)
		if [ $chunks -lt 1 ]; then exit 1; fi
		prepare_chunks;
		bioctl -c C -l /dev/vnd0a softraid0;
	;;
esac

bioctl softraid0
sd=`bioctl softraid0 | grep RAID | sed 's;^.*\(sd[0-9]\).*;\1;'`
disklabel -R $sd disklabel
newfs /dev/r${sd}i

stress;
cleanup;

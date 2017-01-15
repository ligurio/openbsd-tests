#!/bin/sh

set -eu

export PKG_PATH="$MIRROR/`uname -r`/packages/`uname -p`/"

df -h

COMPONENTS="ports src sys xenocara"
RELEASE=`uname -r`
for c in $COMPONENTS; do
  ftp "$MIRROR/$RELEASE/$c.tar.gz" -o /home/$c.tar.gz
done

pkg_add py-simplejson

dd if=/dev/zero of=/EMPTY bs=1M || true
rm -f /EMPTY

sync

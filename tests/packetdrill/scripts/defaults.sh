#!/bin/sh
# OpenBSD-only packetdrill defaults.

UNAME=$(uname)

if [ "$UNAME" != "OpenBSD" ] ; then
  echo "packetdrill tests are supported on OpenBSD only" >&2
  exit 1
fi

echo "OpenBSD"
. scripts/defaults-openbsd.sh

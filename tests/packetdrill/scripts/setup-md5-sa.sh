#!/bin/sh
# Install the TCP MD5 security associations used by the RFC 2385 tests.
set -e
umask 077
conf=$(mktemp /tmp/tcpmd5.XXXXXX)
trap 'rm -f "$conf"' EXIT
cat > "$conf" <<'CONF'
tcpmd5 from 192.168.0.1 to 192.0.2.1 spi 0x100 authkey 0x736563726574
tcpmd5 from 192.0.2.1 to 192.168.0.1 spi 0x101 authkey 0x736563726574
CONF
/sbin/ipsecctl -f "$conf"

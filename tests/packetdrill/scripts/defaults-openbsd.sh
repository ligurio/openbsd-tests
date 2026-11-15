#!/bin/sh
# Set standard OpenBSD TCP configuration values for packetdrill tests.
#
# These values define the baseline "production-like" behavior.  Individual
# tests that need a different setting (for example ECN or keepalive) must
# change it explicitly and restore it afterwards.

# RFC 1323/7323: window scaling, timestamps, PAWS.
sysctl -q net.inet.tcp.rfc1323=1

# RFC 2018: selective acknowledgements.
sysctl -q net.inet.tcp.sack=1

# RFC 3168: ECN is disabled by default in OpenBSD.  Tests that exercise ECN
# enable it explicitly.
sysctl -q net.inet.tcp.ecn=0

# RFC 3390/6928: initial window.  OpenBSD value 2 uses the RFC 6928 IW10
# formula: min(10*MSS, max(2*MSS, 14600)).
sysctl -q net.inet.tcp.rfc3390=2

# RFC 1122: delayed ACK.  Do not ACK immediately on PUSH.
sysctl -q net.inet.tcp.ackonpush=0

# RFC 1122: keepalive defaults.  Idle time is deliberately long; keepalive
# tests lower these values for the duration of the test.
sysctl -q net.inet.tcp.keepinittime=75
sysctl -q net.inet.tcp.keepidle=7200
sysctl -q net.inet.tcp.keepintvl=75
sysctl -q net.inet.tcp.always_keepalive=0

# Keep packet boundaries predictable: no TCP segmentation offload on tun(4).
sysctl -q net.inet.tcp.tso=0

# RFC 1191: path MTU discovery.
sysctl -q net.inet.ip.mtudisc=1

# Do not let the RST rate limiter interfere with tests that send many RSTs.
# Tests for the limiter itself set a lower value explicitly.
sysctl -q net.inet.tcp.rstppslimit=1000000

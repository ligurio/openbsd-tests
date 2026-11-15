# OpenBSD TCP test notes

## Prerequisites

* OpenBSD 7.9 or newer (amd64).
* Root access (or `doas`) to create `tun0`, change routes and set sysctls.
* Python 3 for the `%{ ... }%` assertions.
* packetdrill from the `net/packetdrill` port.

The port is based on the nplab fork and carries the OpenBSD fixes required
by packetdrill: `TUNSIFMODE` handling, point-to-point `tun(4)` setup,
reading packets from the tun file descriptor, `TCP_INFO` support, and
IPv6 point-to-point addressing.  IPv6 needs a destination address with a
/128 prefix and a route through that peer, otherwise the kernel tries
neighbor discovery on the tun device.  The port also drains the multicast
listener reports queued when IPv6 addresses are configured and ignores
asynchronous MLD reports during the test.  For ICMP errors, the port fills
in the echoed IP addresses and transport ports so the kernel can match the
error to the socket that sent the original datagram.

Install packetdrill from packages:

```sh
doas pkg_add packetdrill
```

If the package is not available yet, install the port from `openbsd-wip`:

```sh
doas make -C /path/to/openbsd-wip/net/packetdrill install
```

## Baseline sysctls

`scripts/defaults-openbsd.sh` is sourced by tests that start with
`` `scripts/defaults.sh` ``.  It sets:

| sysctl | value | why |
|---|---|---|
| `net.inet.tcp.rfc1323` | 1 | window scaling, timestamps, PAWS |
| `net.inet.tcp.sack` | 1 | RFC 2018 SACK |
| `net.inet.tcp.ecn` | 0 | ECN tests enable it explicitly |
| `net.inet.tcp.rfc3390` | 2 | RFC 6928 IW10 |
| `net.inet.tcp.ackonpush` | 0 | delayed ACK |
| `net.inet.tcp.keepidle` | 7200 | long default; keepalive tests override |
| `net.inet.tcp.tso` | 0 | predictable packet boundaries on tun(4) |
| `net.inet.ip.mtudisc` | 1 | RFC 1191 PMTUD |
| `net.inet.tcp.rstppslimit` | 1000000 | avoid RST rate-limit flakiness |

OpenBSD defaults worth remembering: `TCPTV_SRTTDFLT` is 3 seconds, so the
initial retransmission timer for a data connection is 6 seconds
(`TCP_REXMTVAL` derives it from `t_rttvar`), while SYN-ACK retransmissions
start at 3 seconds.  `TCPTV_MSL` is 30 seconds, delayed ACK is 200 ms, and
the default socket send/receive buffers are 16 KiB.

## Supported specifications

OpenBSD's TCP stack explicitly implements or follows:

* RFC 9293 (base TCP; source comments still refer to RFC 793 and RFC 1122)
* RFC 1122 (host requirements: keepalive, delayed ACK, silly window)
* RFC 7323 (window scaling, timestamps, PAWS, RTTM; code calls it RFC 1323)
* RFC 2018 (SACK)
* RFC 6582 (NewReno; code calls it RFC 2582)
* RFC 5681 (slow start, congestion avoidance, fast retransmit/recovery)
* RFC 6298 (RTO calculation; initial data RTO is 6 s, SYN-ACK RTO is 3 s)
* RFC 6928 (IW10; `net.inet.tcp.rfc3390=2`)
* RFC 3168 (ECN; disabled by default)
* RFC 1191 (PMTUD)
* RFC 2385 (TCP MD5; requires an IPsec SA)
* RFC 1948 (ISN generation)

Documented as not supported, or only partially supported:

| RFC | Status |
|---|---|
| RFC 5961 | partial: out-of-window RST is dropped, but there is no challenge ACK |
| RFC 2883 | not supported: no DSACK |
| RFC 6675 | not supported: SACK + NewReno, not the RFC 6675 recovery algorithm |
| RFC 5682 | not supported: no F-RTO |
| RFC 3042 | not supported: no Limited Transmit |
| RFC 3465 | not supported: no Appropriate Byte Counting |
| RFC 7413 | not supported: no TCP Fast Open |
| RFC 5482 | not supported: no `TCP_USER_TIMEOUT` |
| RFC 4898 | not supported: `TCP_INFO` is OpenBSD-specific |
| RFC 6528 | not supported: RFC 1948-style ISN generation |
| RFC 5925 | not supported: no TCP-AO |

## packetdrill and TCP_INFO

The `net/packetdrill` port enables `HAVE_TCP_INFO` and adds the OpenBSD
`tcp_info` layout plus the `tcpi_*` variables used by `%{ ... }%`
assertions.  This makes tests such as `tcpi_snd_cwnd`,
`tcpi_snd_ssthresh`, `tcpi_rto` and `tcpi_rtt` work.

OpenBSD leaves the Linux-compatible `__tcpi_*` fields such as
`__tcpi_lost`, `__tcpi_sacked`, `__tcpi_retrans` and `__tcpi_unacked`
unset.  Ported SACK tests must therefore use packet-level assertions or
OpenBSD-specific fields (`tcpi_snd_cwnd`, `tcpi_snd_ssthresh`, `tcpi_rto`,
`tcpi_rtt`, `tcpi_snd_rexmitpack`, `tcpi_rcv_ooopack`, ...).

## Known packetdrill limitations

Some tests cannot be written with the current packetdrill script syntax:

* **UDP checksum and length errors.** `udp(payload)` always generates a
  valid checksum and length.  Tests for a zero checksum, a bad checksum,
  or a malformed UDP length need raw packet injection support.
* **IP fragmentation.** There is no script syntax for injecting individual
  IPv4 or IPv6 fragments, so UDP/TCP reassembly tests need a packetdrill
  extension or a different test harness.
* **TCP PMTUD.** The parser does not accept the `[seq] icmp ...` form with
  an embedded TCP header.  RFC 1191 tests need a parser fix first.
* **TCP MD5.** `TCP_MD5SIG` requires IPsec security associations; the test
  fixture must configure them with `ipsecctl(8)` before running packetdrill.

## Firewall

packetdrill injects traffic through `tun0`.  If `pf(4)` is enabled on the
test machine, add a rule such as `set skip on tun0` or disable `pf` for the
duration of the run.

## Test plan

`testplans/testplan-openbsd.json` is the authoritative list of tests that are
expected to run on OpenBSD.  Tests that do not apply are kept in the plan
with a `skip_reason`, so that coverage gaps remain visible.

The current plan covers RFC 9293 connection management and resets, RFC 1122
delayed ACK/Nagle/keepalive, RFC 7323 window scaling and timestamps, RFC 2018
SACK, RFC 5681/6928 congestion-window behavior, RFC 6298 RTO, RFC 3168 ECN,
OpenBSD-specific RFC 5961 behavior, IPv4 and IPv6 variants of the basic
TCP and UDP paths, and ICMP/ICMPv6 error handling for TCP and connected UDP
sockets.

Still to add:

* RFC 1191 PMTUD: the packetdrill parser used here cannot parse the
  `[seq] icmp ...` embedded-packet syntax.
* RFC 2385 MD5: needs an IPsec SA configured outside packetdrill.
* RFC 1948 ISN randomization: better suited to a statistical test script
  than to a single packetdrill trace.

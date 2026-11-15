# TCP testcases

Testcases are grouped by the RFC that defines the behavior under test.  The
directory name is the current RFC number, even when the OpenBSD source still
refers to an older document:

* `rfc9293/` - base TCP (obsoletes RFC 793)
* `rfc1122/` - host requirements that are not covered by RFC 9293
* `rfc7323/` - window scaling, timestamps, PAWS, RTTM (obsoletes RFC 1323)
* `rfc2018/` - selective acknowledgements
* `rfc6582/` - NewReno (planned; obsoletes RFC 2582)
* `rfc5681/` - congestion control
* `rfc6298/` - retransmission timeout
* `rfc6928/` - initial window (updates RFC 3390)
* `rfc3168/` - ECN
* `rfc1191/` - path MTU discovery
* `rfc2385/` - TCP MD5 signatures
* `rfc5961/` - blind reset/ACK attack mitigation (partial in OpenBSD)
* `rfc1948/` - ISN generation (planned)

A behavior that is intentionally documented as a deviation from an RFC is
kept in the closest RFC directory and noted in the test header as
`OpenBSD: ...`.

Tests formerly kept in feature-based directories are grouped by the RFC
for the behavior they exercise.  Tests for unsupported features (TFO,
DSACK, PRR, Limited Transmit, `TCP_USER_TIMEOUT`, `SO_ZEROCOPY`, epoll,
splice, GRO, `TCP_INQ`, `TCP_NOTSENT_LOWAT`, Linux TX timestamping, Linux
MTU probing) were removed.

## File naming

Use a descriptive filename.  The RFC section is recorded in the file header:

```
synack-retransmission.pkt
```

## Header

Every test starts with a short header so that the suite can be searched and
reviewed without opening the RFC:

```c
// SPDX-License-Identifier: MIT
// RFC 9293 §3.5: SYN-ACK retransmission.
// Requirement: MUST
// OpenBSD: supported
// Preconditions: none
```

Use `Requirement: MUST`, `SHOULD`, or `MAY` for the normative strength, and
list any non-default sysctl or socket option in `Preconditions`.

## Running

On OpenBSD, see `docs/openbsd.md`.  The OpenBSD test plan is:

```sh
python3 scripts/run-tests.py \
    --testplan testplans/testplan-openbsd.json \
    --packetdrill /usr/local/bin/packetdrill \
    --privilege_cmd doas
```

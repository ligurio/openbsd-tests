# packetdrill tests for OpenBSD

This suite contains [packetdrill](https://github.com/nplab/packetdrill) tests
for the OpenBSD TCP/IP stack.  Tests are grouped by the RFC that defines the
behavior under test; the current OpenBSD plan covers TCP RFC 9293,
RFC 1122, RFC 7323, RFC 2018, RFC 5681, RFC 6298, RFC 6928, RFC 3168,
the OpenBSD-specific parts of RFC 5961, and UDP RFC 768.

The suite is based on
[ligurio/packetdrill-testcases](https://github.com/ligurio/packetdrill-testcases).
Tests formerly kept in feature-based directories are grouped by the RFC for
the behavior they exercise.  Only `testplans/testplan-openbsd.json` is used
for OpenBSD runs.

## Layout

```
docs/openbsd.md       OpenBSD notes, RFC matrix and packetdrill caveats
scripts/              test runner and test defaults
testplans/            testplan-openbsd.json
testcases/tcp/        RFC-based TCP testcases
testcases/udp/        RFC-based UDP testcases
```

See [docs/openbsd.md](docs/openbsd.md) for the supported RFCs, OpenBSD
behavior notes and the `TCP_INFO` details.

## Prerequisites

- OpenBSD 7.9 or newer.
- Root access, or `doas`, to create `tun(4)` and set sysctls.
- Python 3 with the `junit-xml` package.
- packetdrill from the `net/packetdrill` port.

Install packetdrill from packages:

```sh
doas pkg_add packetdrill
```

If the package is not available yet, install the port from `openbsd-wip`:

```sh
doas make -C /path/to/openbsd-wip/net/packetdrill install
```

Install the test runner:

```sh
python3 -m venv venv
. venv/bin/activate
pip install -r scripts/requirements.txt
```

## Running

From this directory:

```sh
make run
```

or directly:

```sh
python3 scripts/run-tests.py \
    --testplan testplans/testplan-openbsd.json \
    --packetdrill /usr/local/bin/packetdrill \
    --test_timeout 30
```

The runner selects `doas` or `sudo` automatically.  If `pf(4)` is enabled,
add `set skip on tun0` or disable `pf` while the tests run.

A single testcase can be run directly:

```sh
doas /usr/local/bin/packetdrill testcases/tcp/rfc9293/three-way-handshake-passive.pkt
```

## Acknowledgements

- Google, Inc.: https://github.com/google/packetdrill
- NPLab (Network Programming Laboratory of Münster University of Applied Sciences):
  https://github.com/nplab/packetdrill
- FreeBSD TCP/IP regression test suite:
  https://github.com/freebsd-net/TCP-IP-Regression-TestSuite
- Michael Tüxen and Netflix, Inc.: https://github.com/freebsd-net/tcp-testsuite

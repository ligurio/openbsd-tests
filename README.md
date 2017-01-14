## OpenBSD regression tests

[![Build Status](https://travis-ci.org/ligurio/openbsd-tests.svg?branch=master)](https://travis-ci.org/ligurio/openbsd-tests)


### Who does need these tests?


### How To Run

```
# ansible exoscale -m raw -a "export PKG_PATH=http://ftp.eu.openbsd.org/pub/OpenBSD/6.0/packages/amd64/; pkg_add py-simplejson" -i inventory
# ansible-playbook exoscale devel.yml -i inventory
```

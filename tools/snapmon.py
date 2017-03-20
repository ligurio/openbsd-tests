#!/usr/bin/env python

# Monitor new OpenBSD snapshots
# Run: python ./snapmon.py

# FIX: requests.exceptions.ConnectionError: ('Connection aborted.',
# error(50, 'Network is down'))

from datetime import datetime, time
import logging
import os
import pprint
import requests

# https://www.openbsd.org/ftp.html
# http://spacehopper.org/mirmon/
MIRROR = "http://mirror.yandex.ru/pub/OpenBSD/snapshots/"
TIMEOUT = 300  # 5 min

LEVELS = {'debug': logging.DEBUG,
          'info': logging.INFO,
          'warning': logging.WARNING,
          'error': logging.ERROR,
          'critical': logging.CRITICAL}
FORMAT = u'%(levelname)-8s[%(asctime)s] %(message)s'
level = LEVELS.get('debug', logging.NOTSET)
logging.basicConfig(level=level, format=FORMAT)

snap = {"alpha": "", "amd64": "", "armish": "", "armv7": "",
        "hppa": "", "i386": "", "landisk": "", "loongson": "",
        "luna88k": "", "macppc": "", "octeon": "", "sgi": "",
        "socppc": "", "sparc": "", "sparc64": "", "zaurus": ""}


def ptime(time):
    return datetime.strptime(time, "%a, %d %b %Y %X GMT")

pp = pprint.PrettyPrinter(indent=4)
while True:
    for arch in snap.keys():
        url = os.path.join(MIRROR, arch)
        if requests.get(url).status_code != 200:
            logging.error("Directory not found %s")
            continue
        url = os.path.join(url, "BUILDINFO")
        r = requests.get(url)
        if r.status_code != 200:
            logging.error("BUILDINFO not found")
            continue

        mtime = r.headers["Last-Modified"]
        ctime = time.strftime('%a, %d %b %Y %X GMT')

        if snap[arch] == "":
            snap[arch] = mtime
            logging.info("[NEW SNAPSHOT] %s, builded %s ago" %
                         (arch, ptime(ctime) - ptime(mtime)))
        else:
            logging.debug(snap[arch])
            logging.debug(mtime)
            logging.debug(("> %s") % (snap[arch] > mtime))
            logging.debug(ptime(ctime) - ptime(mtime))
            if (ptime(snap[arch]) > ptime(mtime)):
                logging.info("[NEW SNAPSHOT] %s, builded %s ago %s" %
                             (arch, ptime(ctime) - ptime(mtime), mtime))
                snap[arch] = mtime

    # pp.pprint(snap)
    logging.info("Sleeping (%s sec)" % TIMEOUT)
    time.sleep(TIMEOUT)

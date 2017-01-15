#!/usr/bin/env python

import os
import re
import sys

DEFAULT_PROMISE = ["wpath", "cpath", "rpath"]


def main():
    if len(sys.argv) == 2:
        filename = sys.argv[1]
        promise = DEFAULT_PROMISE
    elif len(sys.argv) == 3:
        filename = sys.argv[1]
        promise = sys.argv[2].split(" ")
    else:
        sys.exit("ERROR: Please specify a source file and promise.")

    if not os.path.exists(filename):
        sys.exit('ERROR: %s was not found.' % sys.argv[1])

    if not os.path.isfile(filename):
        sys.exit('ERROR: %s is not a file.' % sys.argv[1])

    buf = ""
    # Example: pledge("stdio rpath proc exec", NULL)
    with open(filename, 'r') as source:
        for line in source:
            line = line.rstrip()
            pledge_match = re.findall(".*pledge\(\"(.*)\",.*\)", line)
            pledge_num = len(pledge_match)
            print filename, ":", pledge_match
            if pledge_num > 0:
                for p in promise:
                    promise_match = re.findall(
                        ".*pledge\(\"(.*%s.*)\",.*\)" % p, line)
                    promise_num = len(promise_match)
                    if promise_num < pledge_num:
                        line = line.replace(
                            pledge_match[0], "%s %s" % (pledge_match[0], p))
            buf = "{}{}\n".format(buf, line)

    source = open(filename, 'w')
    source.write(buf)
    source.close()

if __name__ == "__main__":
    exit(main())

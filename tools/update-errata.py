#!/usr/bin/env python3

# Update metrics/errata.csv from the official OpenBSD errata pages:
#   https://www.openbsd.org/errata.html
#
# Releases older than --from are copied verbatim, since their pages contain
# manually curated non-patch entries (Patch == "-") and comments which cannot
# be reconstructed from the HTML. Releases starting from --from are rebuilt
# from https://www.openbsd.org/errata{rel}.html.
#
# Usage:
#   tools/update-errata.py [--from 6.0] [--file metrics/errata.csv]

import argparse
import csv
import re
import sys
import urllib.request

INDEX_URL = "https://www.openbsd.org/errata.html"
ERRATA_URL = "https://www.openbsd.org/errata{}.html"

LI_SPLIT = re.compile(r'<li id="[^"]*">')
PATCH_LINK = re.compile(
    r'href="(https://ftp\.openbsd\.org/pub/OpenBSD/patches/[^"]+'
    r'\.patch(?:\.sig)?)"')
ARCH = re.compile(r'</strong>\s*(?:&nbsp;)?\s*<i>([^<]+)</i>')
CATEGORY = re.compile(r'<strong>\d+[^<:]*:\s*([A-Za-z ]+?)\s+FIX')
PATCH_NAME = re.compile(r'/([^/]+)\.patch(?:\.sig)?$')


def fetch(url):
    req = urllib.request.Request(url, headers={'User-Agent': 'update-errata'})
    with urllib.request.urlopen(req, timeout=60) as resp:
        return resp.read().decode('utf-8', 'replace')


def version(release):
    major, minor = release.split('.')
    return (int(major), int(minor))


def releases_from_index():
    html = fetch(INDEX_URL)
    numbers = sorted({int(n) for n in re.findall(r'errata(\d+)\.html', html)})
    result = []
    for n in numbers:
        s = str(n)
        result.append('%s.%s' % (s[:-1], s[-1]))
    return result


def norm_arch(value):
    value = value.strip()
    if value.lower() == 'all architectures':
        return 'All'
    value = re.sub(r'\s+and\s+', ', ', value)
    return ', '.join(p for p in re.split(r'[,\s]+', value) if p)


def parse_release(release):
    html = fetch(ERRATA_URL.format(release.replace('.', '')))
    rows = []
    for block in LI_SPLIT.split(html)[1:]:
        link = PATCH_LINK.search(block)
        if not link:
            continue
        arch = ARCH.search(block)
        category = CATEGORY.search(block)
        rows.append([
            release,
            norm_arch(arch.group(1)) if arch else 'All',
            link.group(1),
            '',
            category.group(1).title() if category else '',
        ])
    rows.sort(key=patch_number)
    return rows


def patch_name(row):
    m = PATCH_NAME.search(row[2])
    return m.group(1) if m else None


def patch_number(row):
    m = re.search(r'(\d+)', patch_name(row) or '')
    return int(m.group(1)) if m else 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--file', default='metrics/errata.csv')
    parser.add_argument('--from', dest='from_release', default='6.0',
                        help='first release to rebuild (default: 6.0)')
    args = parser.parse_args()

    with open(args.file, newline='') as fh:
        rows = list(csv.reader(fh))

    header, data = rows[0], rows[1:]
    threshold = version(args.from_release)

    comments = {}
    existing = {}
    for row in data:
        if len(row) < 5:
            continue
        existing.setdefault(row[0], []).append(row)
        if row[2] != '-':
            comments[patch_name(row)] = row[3]

    updated = [r for r in data if version(r[0]) < threshold]
    for release in releases_from_index():
        if version(release) < threshold:
            continue
        parsed = parse_release(release)
        if not parsed:
            updated.extend(existing.get(release, []))
            continue
        parsed.extend(r for r in existing.get(release, []) if r[2] == '-')
        for row in parsed:
            row[3] = comments.get(patch_name(row), '')
        updated.extend(parsed)

    with open(args.file, 'w', newline='') as fh:
        writer = csv.writer(fh)
        writer.writerow(header)
        writer.writerows(updated)

    return 0


if __name__ == '__main__':
    sys.exit(main())

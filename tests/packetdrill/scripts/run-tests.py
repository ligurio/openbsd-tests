#!/usr/bin/env python3
"""Run packetdrill tests from an OpenBSD test plan."""

import argparse
import json
import logging
import os
import shlex
import shutil
import subprocess
import sys
import time

DEFAULT_TEST_TIMEOUT = 30
DEFAULT_TEST_DIR = "testcases"
DEFAULT_PACKETDRILL = "/usr/local/bin/packetdrill"
SUPPORTED_FILE_EXTENSIONS = (".pkt",)

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s [%(levelname)s] %(message)s")


def default_privilege_cmd():
    """Return the command used to run packetdrill as root."""
    if os.geteuid() == 0:
        return ""
    for command in ("sudo", "doas"):
        if shutil.which(command):
            return command
    return ""


def run_testcase(test_path, test_timeout, packetdrill, privilege_cmd):
    cmd = shlex.split(privilege_cmd) if privilege_cmd else []
    cmd.extend([packetdrill, "--verbose", test_path])
    logging.debug("Command: %s", " ".join(cmd))

    start = time.monotonic()
    try:
        result = subprocess.run(cmd,
                                timeout=test_timeout,
                                capture_output=True,
                                text=True)
    except subprocess.TimeoutExpired:
        duration = time.monotonic() - start
        return None, "", "timeout", duration

    duration = time.monotonic() - start
    if result.stderr:
        logging.info("ERROR OUTPUT:")
        logging.info(result.stderr)
    return result.returncode, result.stdout, result.stderr, duration


def search_testcases(root_path):
    tests = []
    for root, dirs, files in os.walk(root_path):
        dirs.sort()
        for name in sorted(files):
            if os.path.splitext(name)[1] in SUPPORTED_FILE_EXTENSIONS:
                tests.append({"path": os.path.join(root, name)})
    return tests


def load_testcases(args):
    if args.testplan:
        try:
            with open(args.testplan, "r", encoding="utf-8") as plan:
                return json.load(plan)
        except (OSError, ValueError) as error:
            logging.error("cannot read test plan %s: %s", args.testplan, error)
            raise SystemExit(1)

    if not os.path.isdir(args.test_dir):
        logging.error("test directory %s does not exist", args.test_dir)
        raise SystemExit(1)
    return search_testcases(args.test_dir)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--test_dir", default=DEFAULT_TEST_DIR,
                        help="directory with testcases")
    parser.add_argument("--test_timeout", default=DEFAULT_TEST_TIMEOUT,
                        type=int, help="per-test timeout in seconds")
    parser.add_argument("--testplan", help="path to the JSON test plan")
    parser.add_argument("--packetdrill", default=DEFAULT_PACKETDRILL,
                        help="path to the packetdrill binary")
    parser.add_argument("--privilege_cmd", default=None,
                        help="command used to run packetdrill as root "
                             "(default: sudo or doas; empty string disables)")
    parser.add_argument("--filter", help="run tests whose path contains this string")
    parser.add_argument("--list", action="store_true",
                        help="list selected tests and exit")
    parser.add_argument("--discovery", action="store_true",
                        help="list testcases found in --test_dir and exit")
    args = parser.parse_args()

    if args.discovery:
        for test in search_testcases(args.test_dir):
            print(test["path"])
        return 0

    testcases = load_testcases(args)
    if args.filter:
        testcases = [test for test in testcases
                     if args.filter in test["path"]]
    if not testcases:
        logging.error("no tests selected")
        return 1

    if args.list:
        for test in testcases:
            print(test["path"])
        return 0

    privilege_cmd = (default_privilege_cmd()
                     if args.privilege_cmd is None else args.privilege_cmd)

    passed = 0
    failed = 0
    skipped = 0
    for testcase in testcases:
        test_path = testcase["path"]
        if testcase.get("skip_reason"):
            logging.info("SKIPPED (%s): %s", test_path, testcase["skip_reason"])
            skipped += 1
            continue

        timeout = testcase.get("timeout", args.test_timeout)
        rc, _stdout, _stderr, _duration = run_testcase(
            test_path, timeout, args.packetdrill, privilege_cmd)
        if rc == 0:
            logging.info("PASSED (%s)", test_path)
            passed += 1
        else:
            logging.info("FAILED (%s)", test_path)
            failed += 1

    logging.info("Passed %d, failed %d, skipped %d out of %d.",
                 passed, failed, skipped, len(testcases))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())

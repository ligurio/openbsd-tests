#!/usr/bin/env python

import argparse
import subprocess
import shutil
import os
import time
import sys
import socket
import logging
from junit_xml import TestSuite, TestCase
import json

DEFAULT_TEST_TIMEOUT = 20
DEFAULT_TEST_DIR = "testcases"
DEFAULT_REPORT_NAME = "report.xml"
DEFAULT_PACKETDRILL = "/usr/local/bin/packetdrill"

SUPPORTED_FILE_EXTENSIONS = [".pkt"]

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s [%(levelname)s] %(message)s')


def default_privilege_cmd():
    """Return the command used to run packetdrill as root.

    An empty string means "run directly", which is correct when the test
    runner itself is root.  OpenBSD normally provides doas instead of sudo.
    """
    if os.geteuid() == 0:
        return ""
    for command in ("sudo", "doas"):
        if shutil.which(command):
            return command
    return ""


def run_testcase(test_path, test_timeout=DEFAULT_TEST_TIMEOUT,
                 packetdrill=DEFAULT_PACKETDRILL, privilege_cmd=""):
    cmd = []
    if privilege_cmd:
        cmd.append(privilege_cmd)
    cmd.extend([packetdrill, "--verbose", test_path])
    logging.debug("Command: {}".format(" ".join(cmd)))
    start = time.time()
    try:
        result = subprocess.run(cmd,
                                timeout=test_timeout,
                                capture_output=True,
                                universal_newlines=True)
    except subprocess.TimeoutExpired:
        logging.info("Timeout is exceed ({} sec)".format(test_timeout))
        return None, "", "timeout", time.time() - start

    execution_time = time.time() - start

    if result.stderr:
        logging.info("ERROR OUTPUT:")
        logging.info(result.stderr)

    return result.returncode, result.stdout, result.stderr, execution_time


def search_testcases(root_path):
    tests = []
    for root, dirs, files in os.walk(root_path):
        for f in files:
            extension = os.path.splitext(f)[1]
            if extension in SUPPORTED_FILE_EXTENSIONS:
                test = {"path": os.path.join(root, f)}
                tests.append(test)

    return tests


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--test_dir", help="directory with testcases",
                        default=DEFAULT_TEST_DIR)
    parser.add_argument("--test_timeout", help="test execution timeout",
                        default=DEFAULT_TEST_TIMEOUT, type=int)
    parser.add_argument("--discovery", action='store_true',
                        help="discovery testcases")
    parser.add_argument("--testplan", help="path to a testing plan")
    parser.add_argument("--report_file", help="path to report file",
                        default=DEFAULT_REPORT_NAME)
    parser.add_argument("--packetdrill", help="path to packetdrill binary",
                        default=DEFAULT_PACKETDRILL)
    parser.add_argument("--privilege_cmd", default=None,
                        help="command used to run packetdrill as root "
                             "(default: sudo or doas; empty string to disable)")
    args = parser.parse_args()

    if args.privilege_cmd is None:
        privilege_cmd = default_privilege_cmd()
    else:
        privilege_cmd = args.privilege_cmd

    if not os.path.isdir(args.test_dir):
        logging.warning("Path %s is not a directory" % args.test_dir)
        sys.exit(1)

    testcases = []
    if args.testplan:
        with open(args.testplan, 'r') as plan:
            testcases = json.loads(plan.read())
    else:
        testcases = search_testcases(args.test_dir)
        if args.discovery:
            for test in testcases:
                logging.info("Found testcase: {}".format(test))
            sys.exit(0)

    tests = []
    overall_rc = 0
    for testcase in testcases:
        test_path = testcase["path"]
        if testcase.get("skip_reason"):
            test = TestCase(test_path)
            test.add_skipped_info(testcase["skip_reason"])
            status_str = "SKIPPED"
        else:
            timeout = testcase.get("timeout", args.test_timeout)
            rc, stdout, stderr, duration = run_testcase(
                test_path, timeout, args.packetdrill, privilege_cmd)
            test = TestCase(test_path, '', duration, stdout, stderr)
            status_str = "PASSED"
            if rc != 0:
                overall_rc += 1
                test.add_failure_info(message=stderr, output=stderr)
                status_str = "FAILED"
        logging.info("{} ({})".format(status_str, test_path))
        tests.append(test)

    try:
        hostname = socket.gethostbyaddr(socket.gethostname())[0]
    except socket.gaierror:
        hostname = ""

    suite = TestSuite(
        name="suite",
        test_cases=tests,
        hostname=hostname
    )

    with open(args.report_file, 'w') as output:
        output.write(TestSuite.to_xml_string([suite]))
        logging.info("Test output is written to %s", args.report_file)

    logging.info("Failed {} tests out of {}.".format(overall_rc, len(testcases)))
    sys.exit(overall_rc)


if __name__ == "__main__":
    main()

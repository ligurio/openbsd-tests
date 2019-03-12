import pytest
import random
import subprocess

def make_random_c_program():
    CSMITH_ARGS = "--seed %d" % random.randint(1, 1000)
    PROG_SOURCE_PATH = "/tmp/sample.c"
    PROG_BIN_PATH = "/tmp/sample"

    prog_text = subprocess.check_output(["csmith", CSMITH_ARGS.split(" ")])
    with open(PROG_SOURCE_PATH, "w") as sample:
        sample.write(prog_text)

    subprocess.check_call(["gcc", "-I/usr/include/csmith/", "-w", PROG_SOURCE_PATH, "-o", PROG_BIN_PATH])
    subprocess.check_call([PROG_BIN_PATH])


def test_random_c_program():

    ITER_NUMBER = 100
    for i in range(0, ITER_NUMBER):
        make_random_c_program()


def test_setup_openbsd():

    # vmm_install


def test_start_stop():

    # vmm_states

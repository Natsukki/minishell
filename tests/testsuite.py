#!/usr/bin/env python3

from pathlib import Path
from argparse import ArgumentParser
from difflib import unified_diff
import sys
import subprocess as sp
from termcolor import colored
import yaml
import decimal
from subprocess import TimeoutExpired

valgrindpath = "/bin/valgrind"

print("")
print("Minishell Testsuite")
# k = 17 * (
#     (2**17)**0 * 0b11100000000000000 +
#     (2**17)**1 * 0b00100000000000000 +
#     (2**17)**2 * 0b00100000000000000 +
#     (2**17)**3 * 0b11111000000000000 +
#     (2**17)**4 * 0b00100000000000000 +
#     (2**17)**5 * 0b00000000000000000 +
#     (2**17)**6 * 0b01001000000000000 +
#     (2**17)**7 * 0b10011000000000000 +
#     (2**17)**8 * 0b10011000000000000 +
#     (2**17)**9 * 0b01101000000000000 +
# 0)
# # or if you prefer, k=int('4j6h0e8x4fl0deshova5fsap4gq0glw0lc',36)

# def f(x,y):
#     return y // 17 // 2**(x * 17 + y % 17) % 2 > 0.5
# for y in range(k + 16, k + 11, -1):
#     print("".join(" @"[f(x, y)] for x in range(10)))
print("\n")

def run_shell(args, stdin, time):
    time = float(time)
    return sp.run(args, capture_output=True, text=True, input=stdin, timeout=time)

def diff(ref, student):
    ref = ref.splitlines(keepends=True)
    student = student.splitlines(keepends=True)
    return ''.join(unified_diff(ref, student, fromfile="ref", tofile="student"))

def test(binary, stdin, sanity, category, list, time):
    student = run_shell([binary], stdin, time)
    ref = run_shell(["bash", "--posix"], stdin, time)
    assert ref.stdout == student.stdout, f"stdout differs:\n{diff(ref.stdout, student.stdout)}"
    assert ref.stderr == student.stderr, f"stdout differs:\n{diff(ref.stderr, student.stderr)}"
    assert ref.returncode == student.returncode, f"Exited with {student.returncode}, got {ref.returncode}"


if __name__ == "__main__":
    parser = ArgumentParser(description="42 Testsuite")
    parser.add_argument('bin', metavar='BIN')
    parser.add_argument('-s', '--sanity', action='store_true')
    parser.add_argument('-l', '--list', action='store_true')
    parser.add_argument('-c', '--category')
    parser.add_argument('-t', '--timeout')
    passed = 0
    failed = 0
    args = parser.parse_args(sys.argv[1:])
    sanity = args.sanity
    category = args.category
    list = args.list
    timeout = args.timeout
    binary = Path(args.bin).absolute()
    time = 90000
    if (timeout):
        time = float(timeout)
    if (sanity):
            [binary].insert(0, "valgrind --quiet --leak-check=full --error-exitcode=125 --trace-children=yes")
    with open("tests.yml", "r") as tests_files:
        content = yaml.safe_load(tests_files)
    for t in content:
        if (sanity):
            print(f"{colored('Tested with valgrind', 'yellow')}")
        elif (list):
            print(f"{colored('Category name:', 'yellow')}", t["category"])
        elif (category):
            if (t["category"] != category):
                continue
        try:
            test(binary, t["stdin"], sanity, category, list, time)
        except AssertionError as err:
            print(f"[{colored('KO', 'red')}]", t["name"])
            print(err)
            failed = failed + 1
        except TimeoutExpired:
            failed = failed + 1
            print(f"[{colored('KO', 'red')}] process timed out")

        else:
            print(f"[{colored('OK', 'green')}]", t["name"])
            passed = passed + 1
print("\n")
print(f"[{colored('---TESTS PASSED---', 'green')}]\n", passed)
print(f"[{colored('---TESTS FAILED---', 'red')}]\n", failed)
print(f"[{colored('---TOTAL TESTS---', 'blue')}]\n" + str(passed +failed) + " (" + str(round((passed/(passed + failed))*100)) + "%)\n")

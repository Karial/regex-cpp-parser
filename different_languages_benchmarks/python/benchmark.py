import timeit
import re
import random


def check_times1():
    global test_string1

    regex = re.compile("a*")
    regex.fullmatch(test_string1)


def check_times2():
    global test_string2

    regex = re.compile("\\d+")
    regex.fullmatch(test_string2)


def check_times3():
    global test_string3

    regex = re.compile('[abcdef1-4]+')
    regex.fullmatch(test_string3)


def check_times4():
    global test_string4

    regex = re.compile('x')
    regex.fullmatch(test_string4)


test_string1 = 'a' * 1000000 + '#'
test_string2 = '0123456789' * 100000 + '#'
test_string3 = 'abcdef1234' * 100000 + '#'
test_string4 = '-12345.213' * 100000

print("Check times 1 : %.1f ms" % (1000 * timeit.Timer(check_times1).timeit(10)))
print("Check times 2 : %.1f ms" % (1000 * timeit.Timer(check_times2).timeit(10)))
print("Check times 3 : %.1f ms" % (1000 * timeit.Timer(check_times3).timeit(10)))
print("Check times 4 : %.1f ms" % (1000 * timeit.Timer(check_times4).timeit(10)))

# -*- coding: utf-8 -*-

import sys
import uatraits
import json


SUA = 0
SPROCESS = 1

def dicts_equals(d1, d2):
    i1 = set(d1) - set(d2)
    i2 = set(d2) - set(d1)
    if len(i1) != 0 or len(i2) != 0:
        return False
    for x in d1:
        if d1[x] != d2[x]:
            return False
    return True


if sys.version_info < (2,6):
    json_read = json.read
else:
    json_read = json.loads


if __name__ == "__main__":
    try:
        print "Try to make detector..."
        d = uatraits.detector(sys.argv[1])
        print "OK - detector created."
        cover_path = sys.argv[2]
    except IndexError:
        print "usage: %s <browser.xml> <cover.txt>" % sys.argv[0]
        exit(2)
    r = 0
    cover = open(cover_path, "r")
    state = SUA
    for s in cover.xreadlines():
        if state == SUA:
            ua = s.strip()
            state = SPROCESS
        else:
            test = json_read(s)
            answer = d.detect(str(ua))
            if not dicts_equals(test, answer):
                print "* UA", ua
                print test
                print answer
                print
                r = 1
            state = SUA
    if r == 0:
        print "Passed."
    exit(r)


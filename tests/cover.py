# -*- coding: utf-8 -*-

import sys
import uatraits
import json


def dicts_equals(d1, d2):
    i1 = set(d1) - set(d2)
    i2 = set(d2) - set(d1)
    if len(i1) != 0 or len(i2) != 0:
        return False
    for x in d1:
        if d1[x] != d2[x]:
            return False
    return True


if __name__ == "__main__":
    try:
        d = uatraits.detector(sys.argv[1])
        cover_path = sys.argv[2]
    except IndexError:
        print "usage: %s <browser.xml> <cover.txt>" % sys.argv[0]
        exit(2)
    r = 0
    with open(cover_path, "r") as cover:
        for s in cover.xreadlines():
            test = json.loads(s)
            ua = test["UA"]
            del test["UA"]
            answer = d.detect(str(ua))
            if not dicts_equals(test, answer):
                print "* UA", ua
                print test
                print answer
                print
                r = 1
    exit(r)


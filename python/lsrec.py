#!/usr/bin/env python3

import sys
import os

def lsrec(startdir):
    pathsarr = []

    for dirpath, dirnames, files in os.walk(startdir):
        for f in dirnames + files:
            path = os.path.join(dirpath, f)
            pathsarr.append(path)
    return pathsarr

if __name__ == '__main__':
    startdir = ''
    if len(sys.argv) < 2:
        startdir = '.'
    else:
        startdir = sys.argv[1]

    pathsarr = lsrec(startdir)

    print(pathsarr)
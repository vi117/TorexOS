#!/usr/bin/python
import sys
import os

if len(sys.argv) != 2:
    sys.stderr.write("argument error")
    sys.exit(-1)
else:
    size = os.path.getsize(sys.argv[1])
    padding = 512 - (size % 512)
    if padding == 512:
        padding = 0
    f = open(sys.argv[1],"a")
    for i in range(padding):
        f.write('\0')
sys.exit(0)
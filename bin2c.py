#!/usr/bin/env python

import sys

def bytes_from_stream(stream, chunksize=8192):
    while True:
        chunk = stream.read(chunksize)
        if chunk:
            for b in chunk:
                yield b
        else:
            break

varname = sys.argv[1]
infile = sys.argv[2]
outfile = sys.argv[3]

i = 0
with open(infile, "rb") as inf:
    with open(outfile, "w") as outf:
        print("unsigned char " + varname + "[] = {", file=outf)
        for b in bytes_from_stream(inf):
            print(', ' if i > 0 else '  ', file=outf, end='')
            print('0x{:02x}'.format(b), file=outf, end='')
            i += 1
            if i % 16 == 0:
                print(file=outf)
        print('\n};', file=outf)

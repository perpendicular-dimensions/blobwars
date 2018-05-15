#!/usr/bin/python3
import fileinput
import re

for filename in ['credits']:
    finput = fileinput.input(filename)
    foutput = open(filename + ".md", "w")

    for line in finput:
        line = line.strip()
        try:
            (skip, heading, text) = line.split(maxsplit=2)
        except ValueError:
            continue

        skip = int(skip)

        while skip > 20:
            print("", file=foutput)
            skip -= 20

        if heading != "0":
            print ("# " + text, file=foutput)
        else:
            print (text, file=foutput)

    finput.close()
    foutput.close()

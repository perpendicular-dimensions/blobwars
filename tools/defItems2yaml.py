#!/usr/bin/python3
import fileinput
import re

prog = re.compile('(\S+)\s+"([^"]*)"\s+(\S+)\s+(\S+)')
weapons = {}

for filename in ['defItems']:
    finput = fileinput.input(filename)
    foutput = open(filename + ".yaml", "w")

    for line in finput:
        line = line.strip()

        if not line:
            continue

        if line == "@EOF@":
            break

        m = prog.match(line)

        if not m:
            print("no match for", line)
            continue

        print("{}: {{name: '{}', sprite: {}, value: {}}}".format(m.group(1), m.group(2), m.group(3), int(m.group(4))), file=foutput)

    finput.close()
    foutput.close()

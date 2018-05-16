#!/usr/bin/python3
import fileinput
import re

prog = re.compile('"([^"]*)"\s+"([^"]*)"\s+(\S+)\s+(\S+)\s+"([^"]*)"')
levels = {}

for filename in ['hub']:
    finput = fileinput.input(filename)
    foutput = open(filename + ".yaml", "w")

    for line in finput:
        line = line.strip()

        if not line:
            continue

        m = prog.match(line)

        if not m:
            print("no match for", line)
            continue

        if m.group(1) == "@EOF@":
            break

        requires = ""
        if m.group(5) != "@none@":
            requires = ", requires: '{}'".format(m.group(5))


        print("'{}': {{map: {}, x: {}, y: {}{}}}".format(m.group(1), m.group(2), int(m.group(3)), int(m.group(4)), requires), file=foutput)

    finput.close()
    foutput.close()

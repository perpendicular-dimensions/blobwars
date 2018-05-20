#!/usr/bin/python3
import fileinput
import re

prog = re.compile('"([^"]*)"\s+(\S+)\s+(\S+)\s+(\S+)\s+"([^"]*)"\s+(\S+)\s+(\S+)\s+(\S+)')
weapons = {}

for filename in ['defEnemies']:
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

        if m.group(8) == "0":
            flags = ""
        else:
            flags = ", flags: [{}]".format(", ".join(m.group(8).split("+")))

        print("- {{name: '{}', spriteRight: {}, spriteLeft: {}, spriteDeath: {}, weapon: '{}', health: {}, value: {}{}}}".format(m.group(1), m.group(2), m.group(3), m.group(4), m.group(5), int(m.group(6)), int(m.group(7)), flags), file=foutput)

    finput.close()
    foutput.close()

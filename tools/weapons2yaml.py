#!/usr/bin/python3
import yaml
import fileinput
import re
import glob

prog = re.compile('(\S+)\s+"([^"]*)"\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)')
weapons = {}

for filename in ['weapons']:
    finput = fileinput.input(filename)
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

        weapon = {}
        id = int(m.group(1))
        weapon["name"] = m.group(2)
        weapon["id"] = int(m.group(3))
        weapon["damage"] = int(m.group(4))
        weapon["health"] = int(m.group(5))
        weapon["dx"] = int(m.group(6))
        weapon["dy"] = int(m.group(7))
        weapon["reload"] = int(m.group(8))
        weapon["spriteRight"] = m.group(9)
        weapon["spriteLeft"] = m.group(10)
        weapon["fireSound"] = int(m.group(11))
        weapon["flags"] = m.group(12).split('+')

        weapons[id] = weapon
    finput.close()

    with open(filename + ".yaml", "w") as file:
        yaml.dump(weapons, file, default_flow_style=False, allow_unicode=True)

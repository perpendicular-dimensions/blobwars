#!/usr/bin/python3
import yaml
import fileinput
import re
import glob
from collections import OrderedDict

prog = re.compile('(\S+)\s+(\S+)\s+(\S+)\s+"([^"]*)"\s+"([^"]*)"\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)')
widgets = []

def ordered_dict_presenter(dumper, data):
    return dumper.represent_dict(data.items())

yaml.add_representer(OrderedDict, ordered_dict_presenter)

for widgetfile in glob.glob("*Widgets"):
    finput = fileinput.input(widgetfile)
    for line in finput:
        line = line.strip()

        if not line:
            continue

        if line == "END":
            break

        m = prog.match(line)

        if not m:
            print("no match for", line)
            continue

        widget = OrderedDict()
        widget["id"] = m.group(2)
        widget["group"] = m.group(3)
        widget["type"] = m.group(1)
        widget["title"] = m.group(4)
        if widget["type"] == "RADIO":
            widget["options"] = m.group(5).split('|')
        widget["x"] = int(m.group(6))
        widget["y"] = int(m.group(7))
        if "SLIDER" in widget["type"]:
            widget["min"] = int(m.group(8))
            widget["max"] = int(m.group(9))

        widgets.append(widget)
    finput.close()

    with open(widgetfile + ".yaml", "w") as file:
        # Ensure one line per item
        if type(widgets) is list:
            for item in widgets:
                print("-", yaml.dump(item, default_flow_style=True, width=1000, allow_unicode=True), end='', file=file)

#!/usr/bin/python3
import yaml
from yaml.representer import SafeRepresenter
import fileinput
import re
from collections import OrderedDict

# Coaxing PyYAML to output folded blocks
class folded_str(str): pass

def change_style(style, representer):
    def new_representer(dumper, data):
        scalar = representer(dumper, data)
        scalar.style = style
        return scalar
    return new_representer


represent_folded_str = change_style('>', SafeRepresenter.represent_str)
yaml.add_representer(folded_str, represent_folded_str)

def ordered_dict_presenter(dumper, data):
    return dumper.represent_dict(data.items())

yaml.add_representer(OrderedDict, ordered_dict_presenter)

for filename in ['ending']:
    finput = fileinput.input(filename)
    foutput = open(filename + ".yaml", "w")

    title = None
    image = None
    duration = None
    lines = []
    scenes = []
    scene = None

    for line in finput:
        line = line.strip()
        #print(line)

        if not line:
            continue

        if not title:
            assert line[0] == '[' and line[-1] == ']'
            title = line[1:-1]
            scenes = []
            continue
        
        if line == "NEW" or line == "END":
            if scene:
                scene["text"] = folded_str(" ".join(lines))
                scenes.append(scene)

            image = None
            duration = None
            lines = []
            scene = OrderedDict()

            if line == "END":
                yaml.dump({title: scenes}, foutput, default_flow_style=False, width=100, explicit_end=False, allow_unicode=True)
                print("", file=foutput)
                title = None

            continue

        if duration is None:
            try:
                duration = int(line)
                scene["duration"] = duration
            except ValueError:
                if line != "@none@":
                    scene["image"] = line
            continue

        lines.append(line)

    finput.close()
    foutput.close()

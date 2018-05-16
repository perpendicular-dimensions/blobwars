#!/usr/bin/python3
import yaml
from yaml.representer import SafeRepresenter
import fileinput

blurbs = []

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

yaml.add_representer(dict, ordered_dict_presenter)

for filename in ["introText"]:
    finput = fileinput.input(filename)
    assert finput.readline().strip() == "--dummy entry---"
    assert finput.readline().strip() == ""

    while True:
        blurb = {}

        tokens = finput.readline().split()
        if tokens[2] == "-1":
            break

        x = int(tokens[0])
        y = int(tokens[1])
        delay = int(tokens[2])

        if x:
            blurb["dx"] = x
        if y:
            blurb["dx"] = y

        tokens = finput.readline().split()
        if tokens[0] == "SPAWN":
            blurb["spawn"] = tokens[1]

        lines = []
        for i in range(3):
            line = finput.readline().strip()
            if line == "@none@":
                continue
            lines.append(line)

        if lines:
            blurb["text"] = folded_str(" ".join(lines))

        if delay:
            blurb["delay"] = delay

        blurbs.append(blurb)

        assert finput.readline().strip() == ""

    with open(filename + ".yaml", "w") as file:
        yaml.dump(blurbs, file, default_flow_style=False, width=100, allow_unicode=True)

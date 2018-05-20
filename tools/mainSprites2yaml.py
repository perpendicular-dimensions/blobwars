#!/usr/bin/python3
import yaml
import fileinput

sprites = {}

for line in fileinput.input("mainSprites"):
    words = line.split()
    if len(words) < 4:
        continue
    sprite = {}
    if int(words[1]):
        sprite["hue"] = int(words[1])
    if int(words[2]):
        sprite["sat"] = int(words[2])
    if int(words[3]):
        sprite["val"] = int(words[3])
    frames = []
    i = 4
    while words[i] != "@none@":
        frames.append({"file": words[i], "time": int(words[i + 1])})
        i += 2
    sprite["frames"] = frames
    sprites[words[0]] = sprite

with open("mainSprites.yaml", "w") as file:
    yaml.dump(sprites, file)

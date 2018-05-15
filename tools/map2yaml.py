#!/usr/bin/python3
import yaml
import fileinput
import re
import glob
import base64

def ordered_dict_presenter(dumper, data):
    return dumper.represent_dict(data.items())

yaml.add_representer(dict, ordered_dict_presenter)

maps = [
    'arcticWastes',
    'assimilator',
    'caves1',
    'caves2',
    'caves3',
    'caves4',
    'comm',
    'finalBattle',
    'floodedTunnel1',
    'floodedTunnel2',
    'floodedTunnel3',
    'floodedTunnel4',
    'grasslands1',
    'grasslands2',
    'grasslands3',
    'hq',
    'icecave1',
    'icecave2',
    'introMap',
    'practice',
    'spaceStation',
    'supply',
    'tomb1',
    'tomb2',
    'tomb3',
    'tomb4',
]

prog = re.compile('"[^"]*"|\S+')

def unquote(w):
    if w and w[0] == '"':
        return w[1:-1]
    else:
        return w

for filename in maps:
    finput = fileinput.input(filename)

    data = {}
    trains = []
    doors = []
    switches = []
    items = []
    obstacles = []
    objectives = []
    enemies = []
    mias = []
    spawnpoints = []
    teleporters = []
    traps = []
    linedefs = []
    spawnableEnemies = []
    defEnemies = []
    sprites = []

    # Read and dump map data
    blocks = []

    for line in finput:
        line = line.strip()

        if not line:
            break

        row = bytearray()
        for block in line.split():
            row.append(int(block))

        blocks.append(row)
    
    # Read other data
    for line in finput:
        line = line.strip()

        if not line or line[0] == '/':
            continue

        if line == "@EOF@":
            break
        
        m = [unquote(w) for w in prog.findall(line)]

        if m[1] == "STAGENAME":
            assert m[0] == "EMH"
            data["stageName"] = m[2]
        elif m[1] == "PREVIOUSLY_VISITED":
            assert m[0] == "EMH"
            data["previouslyVisited"] = True
        elif m[1] == "TIMELIMIT":
            assert m[0] == "X"
            data["timelimit"] = 60 * int(m[2]) + int(m[3])
        elif m[1] == "TRAIN":
            train = {
                "name": m[2],
                "startX": int(m[3]),
                "startY": int(m[4]),
                "endX": int(m[5]),
                "endY": int(m[6]),
                "pause": int(m[7]),
                "atStart": m[8] == "TR_AT_START",
                "active": m[9] == "ACTIVE",
            }
            trains.append(train)
        elif m[1] == "DOOR":
            door = {
                "name": m[2],
                "type": m[3],
                "startX": int(m[4]),
                "startY": int(m[5]),
                "endX": int(m[6]),
                "endY": int(m[7]),
            }
            if m[8] == "ACTIVE":
                door["active"] = True
            doors.append(door)
        elif m[1] == "SWITCH":
            switch = {
                "name": m[2],
                "type": m[6],
                "x": int(m[7]),
                "y": int(m[8]),
            }
            if m[3] != "@none@":
                switch["target"] = m[3]
            if m[4] != "@none@":
                switch["requires"] = m[4]
            if m[5] != "@none@":
                switch["message"] = m[5]
            if m[9] == "ACTIVE":
                switch["active"] = True
            switches.append(switch)
        elif m[1] == "ITEM":
            item = {
                "type": int(m[2]),
                "name": m[3],
                "x": int(m[4]),
                "y": int(m[5]),
                "sprite": m[6],
            }
            items.append(item)
        elif m[1] == "OBSTACLE":
            obstacle = {
                "name": m[2],
                "x": int(m[3]),
                "y": int(m[4]),
                "sprite": m[5],
            }
            obstacles.append(obstacle)
        elif m[1] == "OBJECTIVE":
            objective = {
                "description": m[2],
                "target": m[3],
            }
            if int(m[4]) != 1:
                objective["count"] = int(m[4])
            if m[5] == "OBJ_OPTIONAL":
                objective["optional"] = True
            objectives.append(objective)
        elif m[1] == "START":
            assert m[0] == "EMH"
            data["start"] = {
                    "x": int(m[2]),
                    "y": int(m[3]),
            }
        elif m[1] == "ENEMY":
            enemy = {
                "name": m[2],
                "x": int(m[3]),
                "y": int(m[4]),
            }
            enemies.append(enemy)
        elif m[1] == "MIA":
            mia = {
                "name": m[2],
                "x": int(m[3]),
                "y": int(m[4]),
                "type": m[5],
            }
            mias.append(mia)
            pass
        elif m[1] == "REQUIREDMIAS":
            assert m[0] == "EMH"
            data["requiredMIAs"] = int(m[2])
        elif m[1] == "LINEDEF":
            linedef = {
                "x": int(m[5]),
                "y": int(m[6]),
                "width": int(m[7]),
                "height": int(m[8]),
            }
            if m[2] != "@none@":
                linedef["name"] = m[2]
            if m[3] != "@none@":
                linedef["target"] = m[3]
            if m[4] != "@none@":
                linedef["message"] = m[4]
            if m[9] == "ACTIVE":
                linedef["active"] = True
            linedefs.append(linedef)
        elif m[1] == "SPAWNPOINT":
            spawnpoint = {
                "name": m[2],
                "x": int(m[3]),
                "y": int(m[4]),
                "type": m[5],
                "active": m[9] == "ACTIVE",
            }
            if m[6] != "SPW_NOSUBTYPE":
                spawnpoint["subtype"] = m[6]
            if int(m[7]) != 0:
                spawnpoint["min"] = int(m[7])
            if int(m[8]) != 1:
                spawnpoint["max"] = int(m[8])
            spawnpoints.append(spawnpoint)
        elif m[1] == "SPAWNABLE_ENEMY":
            assert m[0] == "EMH"
            spawnableEnemies.append(m[2])
        elif m[1] == "TELEPORTER":
            teleporter = {
                "name": m[2],
                "x": int(m[3]),
                "y": int(m[4]),
                "destX": int(m[5]),
                "destY": int(m[6]),
                "active": m[7] == "ACTIVE",
            }
            teleporters.append(teleporter)
        elif m[1] == "TRAP":
            trap = {
                "name": m[2],
                "type": m[3],
                "damage": int(m[4]),
                "startX": int(m[6]),
                "startY": int(m[7]),
                "endX": int(m[8]),
                "endY": int(m[9]),
                "wait1": int(m[10]),
                "wait2": int(m[11]),
                "sprite": m[12],
                "active": m[13] == "ACTIVE",
            }
            if int(m[5]):
                trap["speed"] = int(m[5])
            traps.append(trap)
        elif m[1] == "SPRITE":
            # Ignore difficulty level
            sprite = {
                "name": m[2],
            }
            if m[3] != "0":
                sprite["hue"] = int(m[3])
            if m[4] != "0":
                sprite["sat"] = int(m[4])
            if m[5] != "0":
                sprite["val"] = int(m[5])
            frames = []
            i = 6
            while m[i] != "@none@":
                frames.append({
                    "file": m[i],
                    "time": int(m[i + 1]),
                })
                i += 2
            sprite["frames"] = frames
            sprites.append(sprite)
        elif m[1] == "DEFENEMY":
            enemy = {
                "name": m[2],
                "spriteRight": m[3],
                "spriteLeft": m[4],
                "spriteDeath": m[5],
                "weapon": m[6],
                "health": int(m[7]),
                "value": int(m[8]),
                "flags": m[9].split('+'),
            }
            defEnemies.append(enemy)
        elif m[1] == "TILESET":
            assert m[0] == "EMH"
            data["tileset"] = m[2]
        elif m[1] == "CLIPPING":
            assert m[0] == "EMH"
            clipping = {}
            if m[2] != "-1":
                clipping["left"] = int(m[2])
            if m[3] != "-1":
                clipping["right"] = int(m[3])
            if m[4] != "-1":
                clipping["up"] = int(m[4])
            if m[5] != "-1":
                clipping["down"] = int(m[5])
            data["clipping"] = clipping
        elif m[1] == "AMBIENCE":
            assert m[0] == "EMH"
            data["ambience"] = m[2]
        elif m[1] == "WATERLEVEL":
            assert m[0] == "EMH"
            data["waterLevel"] = int(m[2])
        elif m[1] == "ALPHATILES":
            assert m[0] == "EMH"
            assert m[-1] == "-1"
            data["alphaTiles"] = [int(tile) for tile in m[2:-1]]
        elif m[1] == "BACKGROUND":
            assert m[0] == "EMH"
            data["background"] = m[2]
        elif m[1] == "MUSIC":
            assert m[0] == "EMH"
            data["music"] = m[2]
        else:
            print("could not parse", line)
            pass

    if sprites:
        data["sprites"] = sprites
    if defEnemies:
        data["defEnemies"] = defEnemies
    if mias:
        data["mias"] = mias
    if objectives:
        data["objectives"] = objectives
    if items:
        data["items"] = items
    if doors:
        data["doors"] = doors
    if trains:
        data["trains"] = trains
    if linedefs:
        data["linedefs"] = linedefs
    if teleporters:
        data["teleporters"] = teleporters
    if switches:
        data["switches"] = switches
    if obstacles:
        data["obstacles"] = obstacles
    if traps:
        data["traps"] = traps
    if spawnableEnemies:
        data["spawnableEnemies"] = spawnableEnemies
    if spawnpoints:
        data["spawnpoints"] = spawnpoints
    if enemies:
        data["enemies"] = enemies

    finput.close()

    with open(filename + ".yaml", "w") as file:
        #yaml.dump(data, file, default_flow_style=False, allow_unicode=True)
        for (key, value) in data.items():
            if type(value) == list and not (len(value) == 0 or type(value[0]) == int or type(value[0]) == str):
                print("\n{}:".format(key), file=file)
                for item in value:
                    print("-", yaml.dump(item, default_flow_style=True, width=1000, explicit_end=False, allow_unicode=True), file=file, end='')
            else:
                print("{}: {}".format(key, yaml.dump(value, default_flow_style=True, width=1000, explicit_end=False, allow_unicode=True).replace("\n...", "")), file=file, end='')

        # Dump the map data
        print("\ndata: |", file=file)
        for row in blocks:
            print("  {}".format(base64.b16encode(row).decode()), file=file)

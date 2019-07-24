/*
Copyright (C) 2004-2011 Parallel Realities
Copyright (C) 2011-2015 Perpendicular Dimensions

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "headers.h"

static bool allowAtSkillLevel(const YAML::Node &data)
{
	if (!data["difficulty"])
		return true;

	for (auto &&difficulty: data["difficulty"])
	{
		if (game.skill == 0 && difficulty.as<std::string>() == "easy")
			return true;
		if (game.skill == 1 && difficulty.as<std::string>() == "medium")
			return true;
		if (game.skill >= 2 && difficulty.as<std::string>() == "hard")
			return true;
	}

	return false;
}

void getMapTokens(const YAML::Node &data)
{
	auto stageName = data["stageName"].as<std::string>();
	map.setName(stageName);
	game.setStageName(stageName);
	bool previouslyCleared = false;

	if (data["previouslyVisited"].as<bool>(false))
	{
		previouslyCleared = gameData.stagePreviouslyCleared(stageName);

		if (previouslyCleared)
		{
			debug(("Reading Persistence Data...\n"));
			//persistent = &map.persistents[stageName];
			//persistent_it = persistent->begin();
		}
	}

	if (game.skill == 3 && data["timeLimit"])
	{
		int limit = data["timeLimit"].as<int>();
		map.remainingMinutes = limit / 60;
		map.remainingSeconds = limit % 60;
	}

	for (auto &&sprite: data["sprites"])
		loadSprite(sprite["name"].as<std::string>(), sprite);

	for (auto &&enemy: data["defEnemies"])
		if (allowAtSkillLevel(enemy))
			loadEnemy(enemy);

	auto tileSet = data["tileset"].as<std::string>();
	map.evalTileset(tileSet);
	graphics.loadMapTiles(tileSet);

	for (auto &&train: data["trains"])
		if (allowAtSkillLevel(train))
			map.addTrain(train["name"].as<std::string>(), train["startX"].as<int>(), train["startY"].as<int>(), train["endX"].as<int>(), train["endY"].as<int>(), train["pause"].as<int>(), train["atStart"].as<bool>(), train["active"].as<bool>());

	for (auto &&door: data["doors"])
		if (allowAtSkillLevel(door))
			map.addDoor(door["name"].as<std::string>(), engine.getValueOfDefine(door["type"].as<std::string>()), door["startX"].as<int>(), door["startY"].as<int>(), door["endX"].as<int>(), door["endY"].as<int>(), door["active"].as<bool>(false));

	for (auto &&sw: data["switches"])
		if (allowAtSkillLevel(sw))
			map.addSwitch(sw["name"].as<std::string>(), sw["target"].as<std::string>(""), sw["requires"].as<std::string>(""), sw["message"].as<std::string>(""), engine.getValueOfDefine(sw["type"].as<std::string>()), sw["x"].as<int>(), sw["y"].as<int>(), sw["active"].as<bool>(false));

	for (auto &&item: data["items"])
		if (allowAtSkillLevel(item))
			addItem(item["type"].as<int>(), item["name"].as<std::string>(), item["x"].as<int>(), item["y"].as<int>(), item["sprite"].as<std::string>(), 60, 1, 0, false);

	for (auto &&obstacle: data["obstacles"])
		if (allowAtSkillLevel(obstacle))
			addObstacle(obstacle["name"].as<std::string>(), obstacle["x"].as<int>(), obstacle["y"].as<int>(), obstacle["sprite"].as<std::string>());

	for (auto &&objective: data["objectives"])
		if (allowAtSkillLevel(objective))
			map.addObjective(objective["description"].as<std::string>(), objective["target"].as<std::string>(), objective["count"].as<int>(1), !objective["optional"].as<bool>(false));

	int startX = data["start"]["x"].as<int>();
	int startY = data["start"]["y"].as<int>();
	player.place(startX, startY);
	game.setCheckPoint(startX, startY);
	game.setObjectiveCheckPoint();

	if (!engine.devNoMonsters)
	{
		for (auto &&enemy: data["enemies"])
		{
			std::string name;

			if (game.skill == 0 && map.waterLevel != -1)
				name = "Aqua Blob";
			else
				name = enemy["name"].as<std::string>();

			addEnemy(name, enemy["x"].as<int>(), enemy["y"].as<int>(), 0);
		}
	}

	for (auto &&mia: data["mias"])
	{
		if (allowAtSkillLevel(mia))
		{
			addMIA(mia["name"].as<std::string>(), mia["x"].as<int>(), mia["y"].as<int>(), engine.getValueOfDefine(mia["type"].as<std::string>()));
			map.totalMIAs++;
		}
	}

	map.requiredMIAs = data["requiredMIAs"].as<int>(0);

	for (auto &&linedef: data["lineDefs"])
		if (allowAtSkillLevel(linedef))
			addLineDef(linedef["name"].as<std::string>(""), linedef["target"].as<std::string>(""), linedef["mesage"].as<std::string>(""), linedef["x"].as<int>(), linedef["y"].as<int>(), linedef["width"].as<int>(), linedef["height"].as<int>(), linedef["active"].as<bool>(false));

	for (auto &&sp: data["spawnpoints"])
		if (allowAtSkillLevel(sp))
			map.addSpawnPoint(sp["name"].as<std::string>(), sp["x"].as<int>(), sp["y"].as<int>(), engine.getValueOfDefine(sp["type"].as<std::string>()), engine.getValueOfDefine(sp["subtype"].as<std::string>("SPW_NOSUBTYPE")), sp["min"].as<int>(0), sp["max"].as<int>(1), sp["active"].as<bool>());

	for (auto &&enemy: data["spawnableEnemies"])
		map.setAllowableEnemy(getDefinedEnemy(enemy.as<std::string>()));

	for (auto &&teleporter: data["teleporters"])
		if (allowAtSkillLevel(teleporter))
			addTeleporter(teleporter["name"].as<std::string>(), teleporter["x"].as<int>(), teleporter["y"].as<int>(), teleporter["destX"].as<int>(), teleporter["destY"].as<int>(), teleporter["active"].as<bool>());

	for (auto &&trap: data["traps"])
		if (allowAtSkillLevel(trap))
			addTrap(trap["name"].as<std::string>(), engine.getValueOfDefine(trap["type"].as<std::string>()), trap["damage"].as<int>(), trap["speed"].as<int>(), trap["startX"].as<int>(), trap["startY"].as<int>(), trap["endX"].as<int>(), trap["endY"].as<int>(), trap["wait1"].as<int>(), trap["wait2"].as<int>(), trap["sprite"].as<std::string>(), trap["active"].as<bool>());

	auto clipping = data["clipping"];
	if (clipping)
		map.setClipping(clipping["left"].as<int>(-1), clipping["right"].as<int>(-1), clipping["up"].as<int>(-1), clipping["down"].as<int>(-1));

	if (data["ambience"])
		audio.loadSound(SND_AMBIENCE, data["ambience"].as<std::string>());

	if (data["waterLevel"])
	{
		if (game.skill == 0)
		{
			map.waterLevel = 281;
			map.requiredWaterLevel = 222;
		}
		else if (previouslyCleared)
		{
			map.waterLevel = 281;
		}
		else
		{
			map.waterLevel = data["waterLevel"].as<int>();
		}
	}

	for (auto &&tile: data["alphaTiles"])
		SDL_SetAlpha(graphics.tile[tile.as<int>()], 130);

	graphics.loadBackground(data["background"].as<std::string>());

	if (data["music"])
		audio.loadMusic(data["music"].as<std::string>());

	/*
	We need to make sure the player doesn't appear in a wall that was previously
	destroyed. Things like this aren't stored so we will just remove a block they
	make have been "in"
	*/
	if (previouslyCleared)
	{
		int x = game.checkPointX;
		int y = game.checkPointY;

		x = x >> BRICKSHIFT;
		y = y >> BRICKSHIFT;

		if ((map.data[x][y] >= MAP_BREAKABLE) && (map.data[x][y] <= MAP_BREAKABLE2))
		{
			map.data[x][y] = MAP_AIR;
		}
	}
}

std::string getActiveState(bool active)
{
	if (active)
	{
		return "ACTIVE";
	}

	return "INACTIVE";
}

void createPersistentMapData()
{
	auto &persistent = map.createPersistent(map.name);
	persistent.clear();

	if (perfectlyCompleted())
	{
		debug(("createPersistentMapData :: Perfect - Skipping\n"));
		return;
	}

	char skill;

	switch (game.skill)
	{
	case 0:
		skill = 'E';
		break;
	case 1:
		skill = 'M';
		break;
	default:
		skill = 'H';
		break;
	}

	std::string line = fmt::format("{} START {} {}\n", skill, game.checkPointX, game.checkPointY);
	persistent.push_back(line);

	std::string define[3];

	for (auto &&ent: map.enemies)
	{
		line = fmt::format("{} ENEMY \"{}\" {} {}\n", skill, ent.name, ent.x, ent.y);
		persistent.push_back(line);
	}

	for (auto &&ent: map.items)
	{
		// Don't save items that are dying...
		if (ent.flags & ENT_DYING)
		{
			continue;
		}

		line = fmt::format("{} ITEM {} \"{}\" {} {} {}\n", skill, ent.id, ent.name, ent.x, ent.y, ent.sprite[0]->name);
		persistent.push_back(line);
	}

	for (auto &&ent: map.obstacles)
	{
		line = fmt::format("{} OBSTACLE \"{}\" {} {} {}\n", skill, ent.name, ent.x, ent.y, ent.sprite[0]->name);
		persistent.push_back(line);
	}

	for (auto &&swt: map.switches)
	{
		define[0] = engine.getDefineOfValue("SWT_", swt.type);
		define[1] = getActiveState(swt.activated);

		line = fmt::format("{} SWITCH \"{}\" {} \"{}\" \"{}\" {} {} {} {}\n", skill, swt.name, swt.linkName, swt.requiredObjectName, swt.activateMessage, define[0], swt.x, swt.y, define[1]);
		persistent.push_back(line);
	}

	for (auto &&train: map.trains)
	{
		if (train.type != TR_TRAIN)
		{
			if (train.type >= TR_SLIDEDOOR)
			{
				define[0] = engine.getDefineOfValue("_SLIDE", train.type);
			}
			else
			{
				define[0] = engine.getDefineOfValue("_DOO", train.type);
			}

			define[1] = getActiveState(train.active);
			line = fmt::format("{} DOOR {} {} {} {} {} {} {}\n", skill, train.name, define[0], train.startX, train.startY, train.endX, train.endY, define[1]);
		}
		else
		{
			define[0] = engine.getDefineOfValue("TR_A", train.waitAtStart);
			define[1] = getActiveState(train.active);
			line = fmt::format("{} TRAIN {} {} {} {} {} {} {} {}\n", skill, train.name, train.startX, train.startY, train.endX, train.endY, train.getPause(), define[0], define[1]);
		}

		persistent.push_back(line);
	}

	for (auto &&trap: map.traps)
	{
		define[0] = engine.getDefineOfValue("TRAP_TYPE", trap.type);
		define[1] = getActiveState(trap.active);
		line = fmt::format("{} TRAP {} {} {} {} {} {} {} {} {} {} {} {}\n", skill, trap.name, define[0], (int)trap.damage, (int)trap.speed, (int)trap.startX, (int)trap.startY, (int)trap.endX, (int)trap.endY, (int)trap.waitTime[0], (int)trap.waitTime[1], trap.sprite->name, define[1]);
		persistent.push_back(line);
	}

	for (auto &&teleporter: map.teleports)
	{
		define[0] = getActiveState(teleporter.active);
		line = fmt::format("{} TELEPORTER {} {} {} {} {} {}\n", skill, teleporter.name, (int)teleporter.x, (int)teleporter.y, (int)teleporter.destX, (int)teleporter.destY, define[0]);
		persistent.push_back(line);
	}

	for (auto &&lineDef: map.lines)
	{
		define[0] = getActiveState(lineDef.activated);
		line = fmt::format("{} LINEDEF \"{}\" {} \"{}\" {} {} {} {} {}\n", skill, lineDef.name, lineDef.linkName, lineDef.activateMessage, (int)lineDef.x, (int)lineDef.y, (int)lineDef.width, (int)lineDef.height, define[0]);
		persistent.push_back(line);
	}

	for (auto &&spawnPoint: map.spawns)
	{
		define[0] = engine.getDefineOfValue("SPW_", spawnPoint.spawnType);

		if (contains(define[0], "HAZARD"))
		{
			define[1] = engine.getDefineOfValue("HAZARD_", spawnPoint.spawnSubType);
		}
		else
		{
			define[1] = engine.getDefineOfValue("SPW_", spawnPoint.spawnSubType);
		}

		define[2] = getActiveState(spawnPoint.active);
		line = fmt::format("{} SPAWNPOINT {} {} {} {} {} {} {} {}\n", skill, spawnPoint.name, (int)spawnPoint.x, (int)spawnPoint.y, define[0], define[1], (int)(spawnPoint.minInterval / 60), (int)(spawnPoint.maxInterval / 60), define[2]);
		persistent.push_back(line);
	}

	for (int i = 0; i < 10; i++)
	{
		if (!map.getSpawnableEnemy(i).empty())
		{
			line = fmt::format("{} SPAWNABLE_ENEMY \"{}\"\n", skill, map.getSpawnableEnemy(i));
			persistent.push_back(line);
		}
	}

	if (map.waterLevel != -1)
	{
		line = fmt::format("{} WATERLEVEL {}\n", skill, (int)map.waterLevel);
		persistent.push_back(line);
	}
}

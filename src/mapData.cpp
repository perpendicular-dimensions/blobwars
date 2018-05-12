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

void getMapTokens(split &it)
{
	char skillLevel[10];
	char mapEntity[255];
	char string[10][1024];
	int param[10];
	bool allowAtSkillLevel = false;

	bool previouslyCleared = false;

	std::string_view token;

	std::vector<std::string> *persistent;
	std::vector<std::string>::iterator persistent_it;

	while (true)
	{
		if (!previouslyCleared)
		{
			token = *it;
			++it;
		}
		else
		{
			if (persistent_it == persistent->end())
				break;

			token = *persistent_it++;
		}

#if USEPAK
		graphics.showLoading(1, 100);
		graphics.delay(1);
#endif

		allowAtSkillLevel = false;

		scan(token, "%s", skillLevel);

		if ((contains(skillLevel, "E")) && (game.skill == 0))
		{
			allowAtSkillLevel = true;
		}

		if ((contains(skillLevel, "M")) && (game.skill == 1))
		{
			allowAtSkillLevel = true;
		}

		if ((contains(skillLevel, "H")) && (game.skill >= 2))
		{
			allowAtSkillLevel = true;
		}

		if ((contains(skillLevel, "X")) && (game.skill == 3))
		{
			allowAtSkillLevel = true;
		}

		// This is just for practice missions
		if (game.skill == -1)
			allowAtSkillLevel = true;

		// Ignore comments
		if (contains(skillLevel, "//"))
			allowAtSkillLevel = false;

		if (strcmp("@EOF@", skillLevel) == 0)
		{
			break;
		}

		if (allowAtSkillLevel)
		{
			scan(token, "%*s %s", mapEntity);

			if (strcmp("STAGENAME", mapEntity) == 0)
			{
				scan(token, "%*s %*s %*c %[^\"] %*c", string[0]);

				map.setName(string[0]);
				game.setStageName(string[0]);
			}
			else if (strcmp("PREVIOUSLY_VISITED", mapEntity) == 0)
			{
				previouslyCleared = gameData.stagePreviouslyCleared(map.name);

				if (previouslyCleared)
				{
					debug(("Reading Persistence Data...\n"));
					persistent = &map.persistents[map.name];
					persistent_it = persistent->begin();
				}
			}
			else if (strcmp("TIMELIMIT", mapEntity) == 0)
			{
				debug(("Loading Time Limit: %s\n", token));
				scan(token, "%*s %*s %d %d", &param[0], &param[1]);
				map.remainingMinutes = param[0];
				map.remainingSeconds = param[1];
			}
			else if (strcmp("TRAIN", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s %d %d %d %d %d %s %s", string[0], &param[0], &param[1], &param[2], &param[3], &param[4], string[1], string[2]);
				map.addTrain(string[0], param[0], param[1], param[2], param[3], param[4], engine.getValueOfDefine(string[1]), engine.getValueOfDefine(string[2]));
			}
			else if (strcmp("DOOR", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s %s %d %d %d %d %s", string[0], string[1], &param[0], &param[1], &param[2], &param[3], string[2]);
				map.addDoor(string[0], engine.getValueOfDefine(string[1]), param[0], param[1], param[2], param[3], engine.getValueOfDefine(string[2]));
			}
			else if (strcmp("SWITCH", mapEntity) == 0)
			{
				scan(token, "%*s %*s %*c %[^\"] %*c %s %*c %[^\"] %*c %*c %[^\"] %*c %s %d %d %s", string[0], string[1], string[2], string[3], string[4], &param[0], &param[1], string[5]);
				map.addSwitch(string[0], string[1], string[2], string[3], engine.getValueOfDefine(string[4]), param[0], param[1], engine.getValueOfDefine(string[5]));
			}
			else if (strcmp("ITEM", mapEntity) == 0)
			{
				scan(token, "%*s %*s %d %*c %[^\"] %*c %d %d %s", &param[0], string[0], &param[1], &param[2], string[1]);

				addItem(param[0], string[0], param[1], param[2], string[1], 60, 1, 0, false);

				if (param[0] >= ITEM_MISC)
				{
					map.totalItems++;
				}
			}
			else if (strcmp("OBSTACLE", mapEntity) == 0)
			{
				scan(token, "%*s %*s %*c %[^\"] %*c %d %d %s", string[0], &param[0], &param[1], string[1]);

				addObstacle(string[0], param[0], param[1], string[1]);
			}
			else if (strcmp("OBJECTIVE", mapEntity) == 0)
			{
				scan(token, "%*s %*s %*c %[^\"] %*c %*c %[^\"] %*c %d %s", string[0], string[1], &param[0], string[2]);

				map.addObjective(string[0], string[1], param[0], engine.getValueOfDefine(string[2]));
			}
			else if (strcmp("START", mapEntity) == 0)
			{
				scan(token, "%*s %*s %d %d", &param[0], &param[1]);

				player.place(param[0], param[1]);

				game.setCheckPoint(param[0], param[1]);
				game.setObjectiveCheckPoint();
			}
			else if (strcmp("ENEMY", mapEntity) == 0)
			{
				if (!engine.devNoMonsters)
				{
					scan(token, "%*s %*s %*c %[^\"] %*c %d %d", string[0], &param[0], &param[1]);

					if ((game.skill == 0) && (map.waterLevel != -1))
					{
						addEnemy("Aqua Blob", param[0], param[1], 0);
					}
					else
					{
						addEnemy(string[0], param[0], param[1], 0);
					}
				}
			}
			else if (strcmp("MIA", mapEntity) == 0)
			{
				scan(token, "%*s %*s %*c %[^\"] %*c %d %d %s", string[0], &param[0], &param[1], string[1]);
				addMIA(string[0], param[0], param[1], engine.getValueOfDefine(string[1]));
				map.totalMIAs++;
			}
			else if (strcmp("REQUIREDMIAS", mapEntity) == 0)
			{
				scan(token, "%*s %*s %d", &param[0]);
				map.requiredMIAs = param[0];
			}
			else if (strcmp("LINEDEF", mapEntity) == 0)
			{
				scan(token, "%*s %*s %*c %[^\"] %*c %s %*c %[^\"] %*c %d %d %d %d %s", string[0], string[1], string[2], &param[0], &param[1], &param[2], &param[3], string[3]);

				addLineDef(string[0], string[1], string[2], param[0], param[1], param[2], param[3], engine.getValueOfDefine(string[3]));
			}
			else if (strcmp("SPAWNPOINT", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s %d %d %s %s %d %d %s", string[0], &param[0], &param[1], string[1], string[2], &param[2], &param[3], string[3]);
				map.addSpawnPoint(string[0], param[0], param[1], engine.getValueOfDefine(string[1]), engine.getValueOfDefine(string[2]), param[2], param[3], engine.getValueOfDefine(string[3]));
			}
			else if (strcmp("SPAWNABLE_ENEMY", mapEntity) == 0)
			{
				scan(token, "%*s %*s %*c %[^\"] %*c", string[0]);
				map.setAllowableEnemy(getDefinedEnemy(string[0]));
			}
			else if (strcmp("TELEPORTER", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s %d %d %d %d %s", string[0], &param[0], &param[1], &param[2], &param[3], string[1]);
				addTeleporter(string[0], param[0], param[1], param[2], param[3], engine.getValueOfDefine(string[1]));
			}
			else if (strcmp("TRAP", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s %s %d %d %d %d %d %d %d %d %s %s", string[0], string[1], &param[0], &param[1], &param[2], &param[3], &param[4], &param[5], &param[6], &param[7], string[2], string[3]);
				addTrap(string[0], engine.getValueOfDefine(string[1]), param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], string[2], engine.getValueOfDefine(string[3]));
			}
			else if (strcmp("SPRITE", mapEntity) == 0)
			{
				scan(token, "%*s %*s %[^\n\r]", string[0]);
				loadSprite(string[0]);
			}
			else if (strcmp("DEFENEMY", mapEntity) == 0)
			{
				scan(token, "%*s %*s %[^\n\r]", string[0]);
				loadEnemy(string[0]);
			}
			else if (strcmp("TILESET", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s", string[0]);
				map.evalTileset(string[0]);
				graphics.loadMapTiles(string[0]);
			}
			else if (strcmp("CLIPPING", mapEntity) == 0)
			{
				scan(token, "%*s %*s %d %d %d %d", &param[0], &param[1], &param[2], &param[3]);
				map.setClipping(param[0], param[1], param[2], param[3]);
			}
			else if (strcmp("AMBIENCE", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s", string[0]);
				audio.loadSound(SND_AMBIANCE, string[0]);
			}
			else if (strcmp("WATERLEVEL", mapEntity) == 0)
			{
				scan(token, "%*s %*s %d", &param[0]);

				map.requiredWaterLevel = param[0];

				if (!previouslyCleared)
				{
					map.waterLevel = param[0];
				}
				else
				{
					map.waterLevel = 281;
				}

				if (game.skill == 0)
				{
					map.waterLevel = 281;
					map.requiredWaterLevel = 222;
				}
			}
			else if (strcmp("ALPHATILES", mapEntity) == 0)
			{
				auto it = split(token, ' ').begin();
				++it;
				++it;

				for (; it != it.end(); ++it)
				{
					param[0] = stoi(*it);

					if (param[0] == -1)
						break;

					debug(("Setting Alpha for Tile %d\n", param[0]));

					SDL_SetAlpha(graphics.tile[param[0]], 130);
				}
			}
			else if (strcmp("BACKGROUND", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s", string[0]);
				graphics.loadBackground(string[0]);
			}
			else if (strcmp("MUSIC", mapEntity) == 0)
			{
				scan(token, "%*s %*s %s", string[0]);
				audio.loadMusic(string[0]);
			}
			else if (allowAtSkillLevel)
			{
				graphics.showErrorAndExit("Symbol '%s' not recognised or not implemented in map data", mapEntity);
			}
		}
		else
		{
			//debug(("Ignoring Line: %s\n", token));
		}
	}

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

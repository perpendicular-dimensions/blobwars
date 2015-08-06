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

#include "mapData.h"

void getMapTokens()
{
	char skillLevel[10];
	char mapEntity[255];
	char string[10][1024];
	int param[10];
	bool allowAtSkillLevel = false;
	
	bool previouslyCleared = false;
	
	char *token = NULL;
	
	Persistant *persistant = NULL;
	PersistData *persistData = NULL;
	
	while (true)
	{
		if (!previouslyCleared)
		{
			token = strtok(NULL, "\n");
		}
		else
		{
			persistData = (PersistData*)persistData->next;
			
			if (persistData == NULL)
				break;
			
			token = persistData->data;
		}

		#if USEPAK
		graphics.showLoading(1, 100);
		graphics.delay(1);
		#endif

		allowAtSkillLevel = false;

		sscanf(token, "%s", skillLevel);

		if ((strstr(skillLevel, "E")) && (game.skill == 0))
		{
			allowAtSkillLevel = true;
		}

		if ((strstr(skillLevel, "M")) && (game.skill == 1))
		{
			allowAtSkillLevel = true;
		}

		if ((strstr(skillLevel, "H")) && (game.skill >= 2))
		{
			allowAtSkillLevel = true;
		}
		
		if ((strstr(skillLevel, "X")) && (game.skill == 3))
		{
			allowAtSkillLevel = true;
		}
			
		// This is just for practice missions
		if (game.skill == -1)
			allowAtSkillLevel = true;

		// Ignore comments
		if (strstr(skillLevel, "//"))
			allowAtSkillLevel = false;

		if (strcmp("@EOF@", skillLevel) == 0)
		{
			break;
		}

		if (allowAtSkillLevel)
		{
			sscanf(token, "%*s %s", mapEntity);

			if (strcmp("STAGENAME", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %*c %[^\"] %*c", string[0]);

				map.setName(string[0]);
				game.setStageName(string[0]);
			}
			else if (strcmp("PREVIOUSLY_VISITED", mapEntity) == 0)
			{
				previouslyCleared = gameData.stagePreviouslyCleared(map.name);
				
				if (previouslyCleared)
				{
					debug(("Reading Persistance Data...\n"));
					persistant = map.getPersistant(map.name);
					persistData = (PersistData*)persistant->dataList.getHead();
				}
			}
			else if (strcmp("TIMELIMIT", mapEntity) == 0)
			{
				debug(("Loading Time Limit: %s\n", token));
				sscanf(token, "%*s %*s %d %d", &param[0], &param[1]);
				map.remainingMinutes = param[0];
				map.remainingSeconds = param[1];
			}
			else if (strcmp("TRAIN", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s %d %d %d %d %d %s %s", string[0], &param[0], &param[1], &param[2], &param[3], &param[4], string[1], string[2]);
				map.addTrain(string[0], param[0], param[1], param[2], param[3], param[4], engine.getValueOfDefine(string[1]), engine.getValueOfDefine(string[2]));
			}
			else if (strcmp("DOOR", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s %s %d %d %d %d %s", string[0], string[1], &param[0], &param[1], &param[2], &param[3], string[2]);
				map.addDoor(string[0], engine.getValueOfDefine(string[1]), param[0], param[1], param[2], param[3], engine.getValueOfDefine(string[2]));
			}
			else if (strcmp("SWITCH", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %*c %[^\"] %*c %s %*c %[^\"] %*c %*c %[^\"] %*c %s %d %d %s", string[0], string[1], string[2], string[3], string[4], &param[0], &param[1], string[5]);
				map.addSwitch(string[0], string[1], string[2], string[3], engine.getValueOfDefine(string[4]), param[0], param[1], engine.getValueOfDefine(string[5]));
			}
			else if (strcmp("ITEM", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %d %*c %[^\"] %*c %d %d %s", &param[0], string[0], &param[1], &param[2], string[1]);
				
				addItem(param[0], string[0], param[1], param[2], string[1], 60, 1, 0, false);

				if (param[0] >= ITEM_MISC)
				{
					map.totalItems++;
				}
			}
			else if (strcmp("OBSTACLE", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %*c %[^\"] %*c %d %d %s", string[0], &param[0], &param[1], string[1]);

				addObstacle(string[0], param[0], param[1], string[1]);
			}
			else if (strcmp("OBJECTIVE", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %*c %[^\"] %*c %*c %[^\"] %*c %d %s", string[0], string[1], &param[0], string[2]);

				map.addObjective(string[0], string[1], param[0], engine.getValueOfDefine(string[2]));
			}
			else if (strcmp("START", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %d %d", &param[0], &param[1]);

				player.place(param[0], param[1]);

				game.setCheckPoint(param[0], param[1]);
				game.setObjectiveCheckPoint();

			}
			else if (strcmp("ENEMY", mapEntity) == 0)
			{
				if (!engine.devNoMonsters)
				{
					sscanf(token, "%*s %*s %*c %[^\"] %*c %d %d", string[0], &param[0], &param[1]);
					
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
				sscanf(token, "%*s %*s %*c %[^\"] %*c %d %d %s", string[0], &param[0], &param[1], string[1]);
				addMIA(string[0], param[0], param[1], engine.getValueOfDefine(string[1]));
				map.totalMIAs++;
			}
			else if (strcmp("REQUIREDMIAS", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %d", &param[0]);
				map.requiredMIAs = param[0];
			}
			else if (strcmp("LINEDEF", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %*c %[^\"] %*c %s %*c %[^\"] %*c %d %d %d %d %s", string[0], string[1], string[2], &param[0], &param[1], &param[2], &param[3], string[3]);

				addLineDef(string[0], string[1], string[2], param[0], param[1], param[2], param[3], engine.getValueOfDefine(string[3]));
			}
			else if (strcmp("SPAWNPOINT", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s %d %d %s %s %d %d %s", string[0], &param[0], &param[1], string[1], string[2], &param[2], &param[3], string[3]);
				map.addSpawnPoint(string[0], param[0], param[1], engine.getValueOfDefine(string[1]), engine.getValueOfDefine(string[2]), param[2], param[3], engine.getValueOfDefine(string[3]));
			}
			else if (strcmp("SPAWNABLE_ENEMY", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %*c %[^\"] %*c", string[0]);
				map.setAllowableEnemy(getDefinedEnemy(string[0]));
			}
			else if (strcmp("TELEPORTER", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s %d %d %d %d %s", string[0], &param[0], &param[1], &param[2], &param[3], string[1]);
				addTeleporter(string[0], param[0], param[1], param[2], param[3], engine.getValueOfDefine(string[1]));
			}
			else if (strcmp("TRAP", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s %s %d %d %d %d %d %d %d %d %s %s", string[0], string[1], &param[0], &param[1], &param[2], &param[3], &param[4], &param[5], &param[6], &param[7], string[2], string[3]);
				addTrap(string[0], engine.getValueOfDefine(string[1]), param[0], param[1], param[2], param[3], param[4], param[5], param[6], param[7], string[2], engine.getValueOfDefine(string[3]));
			}
			else if (strcmp("SPRITE", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %[^\n\r]", string[0]);
				loadSprite(string[0]);
			}
			else if (strcmp("DEFENEMY", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %[^\n\r]", string[0]);
				loadEnemy(string[0]);
			}
			else if (strcmp("TILESET", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s", string[0]);
				map.evalTileset(string[0]);
				graphics.loadMapTiles(string[0]);
			}
			else if (strcmp("CLIPPING", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %d %d %d %d", &param[0], &param[1], &param[2], &param[3]);
				map.setClipping(param[0], param[1], param[2], param[3]);
			}
			else if (strcmp("AMBIENCE", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s", string[0]);
				audio.loadSound(SND_AMBIANCE, string[0]);
			}
			else if (strcmp("WATERLEVEL", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %d", &param[0]);
				
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
				for (int i = 0 ; i < 15 ; i++)
					token++;

				while (true)
				{
					sscanf(token, "%d", &param[0]);

					if (param[0] == -1)
						break;
						
					debug(("Setting Alpha for Tile %d\n", param[0]));

					SDL_SetAlpha(graphics.tile[param[0]], 130);

					while (true)
					{
						token++;

						if (*token == ' ')
							break;
					}
				}
			}
			else if (strcmp("BACKGROUND", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s", string[0]);
				graphics.loadBackground(string[0]);
			}
			else if (strcmp("MUSIC", mapEntity) == 0)
			{
				sscanf(token, "%*s %*s %s", string[0]);
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

const char *getActiveState(bool active)
{
	if (active)
	{
		return "ACTIVE";
	}
	
	return "INACTIVE";
}

void createPersistantMapData()
{
	if (!engine.loadDefines())
	{
		graphics.showErrorAndExit("Could not load map define list '%s'", "data/defines.h");
	}
	
	Persistant *persistant = (Persistant*)map.createPersistant(map.name);
	persistant->clear();
	
	if (perfectlyCompleted())
	{
		debug(("createPersistantMapData :: Perfect - Skipping\n"));
		return;
	}
	
	char line[1024];
	line[0] = 0;
	
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
	
	snprintf(line, sizeof line, "%c START %d %d\n", skill, (int)game.checkPointX, (int)game.checkPointY);
	persistant->addLine(line);

	Entity *ent;
	Switch *swt;
	Train *train;
	Trap *trap;
	Teleporter *teleporter;
	LineDef *lineDef;
	SpawnPoint *spawnPoint;
	
	char *define[3];
	
	ent = (Entity*)map.enemyList.getHead();
	
	while (ent->next != NULL)
	{
		ent = (Entity*)ent->next;
		snprintf(line, sizeof line, "%c ENEMY \"%s\" %d %d\n", skill, ent->name, (int)ent->x, (int)ent->y);
		persistant->addLine(line);
	}
	
	ent = (Entity*)map.itemList.getHead();
	
	while (ent->next != NULL)
	{
		ent = (Entity*)ent->next;
		
		// Don't save items that are dying...
		if (ent->flags & ENT_DYING)
		{
			continue;
		}
		
		snprintf(line, sizeof line, "%c ITEM %d \"%s\" %d %d %s\n", skill, ent->id, ent->name, (int)ent->x, (int)ent->y, ent->sprite[0]->name);
		persistant->addLine(line);
	}
	
	ent = (Entity*)map.obstacleList.getHead();
	
	while (ent->next != NULL)
	{
		ent = (Entity*)ent->next;
		snprintf(line, sizeof line, "%c OBSTACLE \"%s\" %d %d %s\n", skill, ent->name, (int)ent->x, (int)ent->y, ent->sprite[0]->name);
		persistant->addLine(line);
	}
	
	swt = (Switch*)map.switchList.getHead();
	
	while (swt->next != NULL)
	{
		swt = (Switch*)swt->next;
		define[0] = engine.getDefineOfValue("SWT_", swt->type);
		define[1] = (char*)getActiveState(swt->activated);
		
		snprintf(line, sizeof line, "%c SWITCH \"%s\" %s \"%s\" \"%s\" %s %d %d %s\n", skill, swt->name, swt->linkName, swt->requiredObjectName, swt->activateMessage, define[0], (int)swt->x, (int)swt->y, define[1]);
		
		persistant->addLine(line);
	}
	
	train = (Train*)map.trainList.getHead();
	
	while (train->next != NULL)
	{
		train = (Train*)train->next;
		
		if (train->type != TR_TRAIN)
		{
			if (train->type >= TR_SLIDEDOOR)
			{
				define[0] = engine.getDefineOfValue("_SLIDE", train->type);
			}
			else
			{
				define[0] = engine.getDefineOfValue("_DOO", train->type);
			}
			
			define[1] = (char*)getActiveState(train->active);
			snprintf(line, sizeof line, "%c DOOR %s %s %d %d %d %d %s\n", skill, train->name, define[0], train->startX, train->startY, train->endX, train->endY, define[1]);
		}
		else
		{
			define[0] = engine.getDefineOfValue("TR_A", train->waitAtStart);
			define[1] = (char*)getActiveState(train->active);
			snprintf(line, sizeof line, "%c TRAIN %s %d %d %d %d %d %s %s\n", skill, train->name, train->startX, train->startY, train->endX, train->endY, train->getPause(), define[0], define[1]);
		}
				
		persistant->addLine(line);
	}
	
	trap = (Trap*)map.trapList.getHead();
	
	while (trap->next != NULL)
	{
		trap = (Trap*)trap->next;
		define[0] = engine.getDefineOfValue("TRAP_TYPE", trap->type);
		define[1] = (char*)getActiveState(trap->active);
		snprintf(line, sizeof line, "%c TRAP %s %s %d %d %d %d %d %d %d %d %s %s\n", skill, trap->name, define[0], (int)trap->damage, (int)trap->speed, (int)trap->startX, (int)trap->startY, (int)trap->endX, (int)trap->endY, (int)trap->waitTime[0], (int)trap->waitTime[1], trap->sprite->name, define[1]);
		persistant->addLine(line);
	}
	
	teleporter = (Teleporter*)map.teleportList.getHead();
	
	while (teleporter->next != NULL)
	{
		teleporter = (Teleporter*)teleporter->next;
		define[0] = (char*)getActiveState(teleporter->active);
		snprintf(line, sizeof line, "%c TELEPORTER %s %d %d %d %d %s\n", skill, teleporter->name, (int)teleporter->x, (int)teleporter->y, (int)teleporter->destX, (int)teleporter->destY, define[0]);
		persistant->addLine(line);
	}
	
	lineDef = (LineDef*)map.lineList.getHead();
	
	while (lineDef->next != NULL)
	{
		lineDef = (LineDef*)lineDef->next;
		define[0] = (char*)getActiveState(lineDef->activated);
		snprintf(line, sizeof line, "%c LINEDEF \"%s\" %s \"%s\" %d %d %d %d %s\n", skill, lineDef->name, lineDef->linkName, lineDef->activateMessage, (int)lineDef->x, (int)lineDef->y, (int)lineDef->width, (int)lineDef->height, define[0]);
		persistant->addLine(line);
	}
	
	spawnPoint = (SpawnPoint*)map.spawnList.getHead();
	
	while (spawnPoint->next != NULL)
	{
		spawnPoint = (SpawnPoint*)spawnPoint->next;
		define[0] = engine.getDefineOfValue("SPW_", spawnPoint->spawnType);
		
		if (strstr(define[0], "HAZARD"))
		{
			define[1] = engine.getDefineOfValue("HAZARD_", spawnPoint->spawnSubType);
		}
		else
		{
			define[1] = engine.getDefineOfValue("SPW_", spawnPoint->spawnSubType);
		}
			
		define[2] = (char*)getActiveState(spawnPoint->active);
		snprintf(line, sizeof line, "%c SPAWNPOINT %s %d %d %s %s %d %d %s\n", skill, spawnPoint->name, (int)spawnPoint->x, (int)spawnPoint->y, define[0], define[1], (int)(spawnPoint->minInterval / 60), (int)(spawnPoint->maxInterval / 60), define[2]);
		persistant->addLine(line);
	}
	
	for (int i = 0 ; i < 10 ; i++)
	{
		if (map.getSpawnableEnemy(i) != NULL)
		{
			snprintf(line, sizeof line, "%c SPAWNABLE_ENEMY \"%s\"\n", skill, map.getSpawnableEnemy(i));
			persistant->addLine(line);
		}
	}
	
	if (map.waterLevel != -1)
	{
		snprintf(line, sizeof line, "%c WATERLEVEL %d\n", skill, (int)map.waterLevel);
		persistant->addLine(line);
	}
	
	// We don't need this anymore. Remove it to free up some memory...
	engine.defineList.clear();
}

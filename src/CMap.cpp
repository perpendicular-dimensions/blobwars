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

Map::Map()
{
	clear();
	
	for (int i = 0 ; i < 10 ; i++)
	{
		boss[i] = NULL;
	}
}


void Map::clear()
{
	fightingGaldov = false;
	bossNextThink = 0;

	foundItems = totalItems = 0;
	foundMIAs = totalMIAs = 0;
	requiredMIAs = 0;

	limitLeft = limitUp = 0;
	limitRight = ((MAPWIDTH - 40) * BRICKSIZE);
	limitDown = ((MAPHEIGHT - 30) * BRICKSIZE);

	name[0] = 0;

	for (int x = 0 ; x < MAPWIDTH ; x++)
		for (int y = 0 ; y < MAPHEIGHT ; y++)
			data[x][y] = 0;

	for (int i = 0 ; i < 10 ; i++)
		allowableEnemy[i] = NULL;

	maxAllowableEnemies = 0;

	trainList.clear();
	itemList.clear();
	bulletList.clear();
	enemyList.clear();
	miaList.clear();
	obstacleList.clear();
	particleList.clear();
	switchList.clear();
	spawnList.clear();
	effectList.clear();
	objectiveList.clear();
	teleportList.clear();
	lineList.clear();
	trapList.clear();
	
	remainingMinutes = 0;
	remainingSeconds = 0;
	
	waterLevel = requiredWaterLevel = -1;
	isBossMission = false;
	isIceLevel = false;
	isBlizzardLevel = false;
	isCavesTileset = false;
	isGrasslandsTileset = false;
	bossNextThink = 0;
	mainBossPart = NULL;
	doBossLevelAction = NULL;
	bossEnergyMeterBit = 0;
	
	for (int i = 0 ; i < 10 ; i++)
	{
		if (boss[i] != NULL)
		{
			delete boss[i];
		}
		boss[i] = NULL;
	}
	
	windPower = windChangeTime = 0;
}

void Map::destroy()
{
	clear();
	
	persistantList.clear();
}

bool Map::isPracticeMission()
{
	if (strstr(name, "Practice"))
		return true;
		
	return false;
}

bool Map::isValid(int x, int y)
{
	if (x >= 0 && y >= 0 && x < MAPWIDTH && y < MAPHEIGHT)
		return true;

	return false;
}

bool Map::isSolid(int x, int y)
{
	if (!isValid(x, y))
		return false;

	if ((data[x][y] >= MAP_BREAKABLE) && (data[x][y] < MAP_DECORATION))
	{
		return true;
	}

	return false;
}

bool Map::isBreakable(int x, int y)
{
	if (!isValid(x, y))
		return false;

	if ((data[x][y] >= MAP_BREAKABLE) && (data[x][y] <= MAP_BREAKABLE2))
	{
		return true;
	}

	return false;
}

bool Map::isNoReset(int x, int y)
{
	if (!isValid(x, y))
		return false;

	if ((data[x][y] >= MAP_NORESET) && (data[x][y] < MAP_DECORATION))
	{
		return true;
	}

	return false;
}

bool Map::isLiquid(int x, int y)
{
	if (!isValid(x, y))
		return false;

	if (data[x][y] == 0)
	{
		return false;
	}
	else if ((data[x][y] >= MAP_WATER) && (data[x][y] <= MAP_LAVA))
	{
		return true;
	}
	else if ((data[x][y] >= MAP_WATERANIM) && (data[x][y] <= MAP_LAVAANIM))
	{
		return true;
	}

	return false;
}

bool Map::isTopLayer(int x, int y)
{
	if (!isValid(x, y))
		return false;

	if (data[x][y] >= MAP_TOPLAYER)
	{
		return true;
	}

	return false;
}

Persistant *Map::getPersistant(const char *name)
{
	Persistant *p = (Persistant*)persistantList.getHead();
	
	while (p->next != NULL)
	{
		p = (Persistant*)p->next;
		
		if (strcmp(p->stageName, name) == 0)
		{
			return p;
		}
	}
	
	return NULL;
}

Persistant *Map::createPersistant(const char *name)
{
	Persistant *p = (Persistant*)persistantList.getHead();
	
	while (p->next != NULL)
	{
		p = (Persistant*)p->next;
		
		if (strcmp(p->stageName, name) == 0)
		{
			return p;
		}
	}
	
	debug(("Creating %s in persistance list...\n", name));
	
	p = new Persistant();
	p->setName(name);
	
	persistantList.add(p);
	
	return p;
}

void Map::destroyPersistant(const char *name)
{
	Persistant *p = (Persistant*)persistantList.getHead();
	
	while (p->next != NULL)
	{
		p = (Persistant*)p->next;
		
		if (strcmp(p->stageName, name) == 0)
		{
			strlcpy(p->stageName, "@none@", sizeof p->stageName);
			p->clear();
			return;
		}
	}
}

void Map::setName(const char *name)
{
	strlcpy(this->name, name, sizeof this->name);
	
	if (strstr(name, "BioMech"))
	{
		isBossMission = true;
	}
		
	if (strcmp(name, "Galdov") == 0)
	{
		isBossMission = true;
	}

	if (strcmp(name, "Final Battle") == 0)
	{
		isBossMission = true;
	}
		
	if (strstr(name, "Ice"))
	{
		isIceLevel = true;
	}
		
	if (strstr(name, "Arctic"))
	{
		isIceLevel = true;
		isBlizzardLevel = true;
	}
}

void Map::setClipping(int limitLeft, int limitRight, int limitUp, int limitDown)
{
	if (limitLeft != -1) this->limitLeft = limitLeft;
	if (limitRight != -1) this->limitRight = limitRight;
	if (limitUp != -1) this->limitUp = limitUp;
	if (limitDown != -1) this->limitDown = limitDown;
}

void Map::addTrain(const char *name, int startX, int startY, int endX, int endY, int pause, bool atStart, bool active)
{
	Train *train = new Train();
	train->setName(name);
	train->type = TR_TRAIN;
	train->set(startX, startY, endX, endY, pause, atStart);
	train->active = active;

	train->width = 64;
	train->height = 16;
	
	if (pause == 0)
	{
		debug(("WARNING: TRAIN WITH 0 WAIT TIME ADDED '%s' (%d:%d)\n", name, startX, startY));
	}

	trainList.add(train);
}

void Map::addDoor(const char *name, int type, int startX, int startY, int endX, int endY, bool active)
{
	Train *train = new Train();
	train->setName(name);

	train->type = type;

	train->set(startX, startY, endX, endY, 0, false);
	train->active = active;

	if (type < TR_SLIDEDOOR)
	{
		train->width = 16;
		train->height = 64;
	}
	else
	{
		train->width = 64;
		train->height = 16;
	}
	
	trainList.add(train);
}

void Map::addSwitch(const char *name, const char *linkName, const char *requiredObjectName, const char *activateMessage, int type, int x, int y, bool activated)
{
	Switch *swt = new Switch();
	swt->set(name, linkName, requiredObjectName, activateMessage, type, x, y, activated);

	switchList.add(swt);
}

void Map::addItem(Entity *item)
{
	itemList.add(item);
}

void Map::addBullet(Entity *bullet)
{
	bulletList.add(bullet);
}

void Map::addParticle(float x, float y, float dx, float dy, int health, int color, Sprite *sprite, int flags)
{
	Particle *particle = new Particle();
	particle->set(x, y, dx, dy, color, health, flags);
	particle->setSprite(sprite);
	
	particleList.add(particle);
}

void Map::addEnemy(Entity *enemy)
{
	enemyList.add(enemy);
}

void Map::addMIA(Entity *mia)
{
	miaList.add(mia);
}

void Map::addObstacle(Entity *obstacle)
{
	obstacleList.add(obstacle);
}

void Map::addSpawnPoint(const char *name, int x, int y, int type, int subtype, int min, int max, bool active)
{
	SpawnPoint *spawnPoint = new SpawnPoint();
	spawnPoint->create(name, x, y, type, subtype, min, max, active);

	spawnList.add(spawnPoint);
}

void Map::addEffect(Effect *effect)
{
	effectList.add(effect);
}

void Map::addObjective(const char *description, const char *target, int targetValue, bool required)
{
	Objective *objective = new Objective(description, target, targetValue, required);

	objectiveList.add(objective);
}

void Map::addTeleporter(Teleporter *teleporter)
{
	teleportList.add(teleporter);
}

void Map::addLineDef(LineDef *lineDef)
{
	lineList.add(lineDef);
}

void Map::addTrap(Trap *trap)
{
	trapList.add(trap);
}

void Map::evalTileset(const char *baseDir)
{
	if (strstr(baseDir, "caves"))
	{
		isCavesTileset = true;
	}
	else if (strstr(baseDir, "grasslands"))
	{
		isGrasslandsTileset = true;
	}
}

void Map::killAllEnemies()
{
	Entity *enemy = (Entity*)enemyList.getHead()->next;

	while (enemy != NULL)
	{
		enemy->health = -1;

		enemy = (Entity*)enemy->next;
	}
}

void Map::setAllowableEnemy(Entity *enemy)
{
	for (int i = 0 ; i < 10 ; i++)
	{
		if (allowableEnemy[i] == NULL)
		{
			allowableEnemy[i] = enemy;
			maxAllowableEnemies = i + 1;
			return;
		}
	}

	debug(("WARNING: Can't add anymore spawnable enemies to list!!!\n"));
}

char *Map::getSpawnableEnemy(int i)
{
	if (allowableEnemy[i] == NULL)
		return NULL;
	
	return allowableEnemy[i]->name;
}

char *Map::getSpawnableEnemy()
{
	if (maxAllowableEnemies == 0)
	{
		printf("ERROR: No enemy spawn list defined for map '%s'!! Please report this Error!\n", name);
		exit(1);
	}

	return allowableEnemy[Math::prand() % maxAllowableEnemies]->name;
}

void Map::getRandomEntityPosition(int *x, int *y)
{
	Entity *ent = (Entity*)miaList.getHead();

	while (ent->next != NULL)
	{
		ent = (Entity*)ent->next;

		if ((Math::prand() % 5) == 0)
		{
			*x = (int)ent->x;
			*y = (int)ent->y;
			return;
		}
	}

	ent = (Entity*)enemyList.getHead();

	while (ent->next != NULL)
	{
		ent = (Entity*)ent->next;

		if ((Math::prand() % 5) == 0)
		{
			*x = (int)ent->x;
			*y = (int)ent->y;
			return;
		}
	}

	ent = (Entity*)itemList.getHead();

	while (ent->next != NULL)
	{
		ent = (Entity*)ent->next;

		if ((Math::prand() % 5) == 0)
		{
			*x = (int)ent->x;
			*y = (int)ent->y;
			return;
		}
	}
}

void Map::setMainBossPart(Boss *boss)
{
	mainBossPart = boss;
	
	if (mainBossPart != NULL)
	{
		bossEnergyMeterBit = 200;
		bossEnergyMeterBit /= boss->maxHealth;
	}
}

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

	for (int i = 0; i < 10; i++)
	{
		boss[i] = nullptr;
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

	name.clear();

	for (int x = 0; x < MAPWIDTH; x++)
		for (int y = 0; y < MAPHEIGHT; y++)
			data[x][y] = 0;

	for (int i = 0; i < 10; i++)
		allowableEnemy[i] = nullptr;

	maxAllowableEnemies = 0;

	trains.clear();
	items.clear();
	bullets.clear();
	enemies.clear();
	mias.clear();
	obstacles.clear();
	particles.clear();
	switches.clear();
	spawns.clear();
	effects.clear();
	objectives.clear();
	teleports.clear();
	lines.clear();
	traps.clear();

	remainingMinutes = 0;
	remainingSeconds = 0;

	waterLevel = requiredWaterLevel = -1;
	isBossMission = false;
	isIceLevel = false;
	isBlizzardLevel = false;
	isCavesTileset = false;
	isGrasslandsTileset = false;
	bossNextThink = 0;
	mainBossPart = nullptr;
	doBossLevelAction = nullptr;
	bossEnergyMeterBit = 0;

	for (int i = 0; i < 10; i++)
	{
		if (boss[i] != nullptr)
		{
			delete boss[i];
		}
		boss[i] = nullptr;
	}

	windPower = windChangeTime = 0;
}

bool Map::isPracticeMission()
{
	return name.find("Practice") != name.npos;
}

bool Map::isValid(int x, int y)
{
	return x >= 0 && y >= 0 && x < MAPWIDTH && y < MAPHEIGHT;
}

bool Map::isSolid(int x, int y)
{
	return isValid(x, y) && data[x][y] >= MAP_BREAKABLE && data[x][y] < MAP_DECORATION;
}

bool Map::isBreakable(int x, int y)
{
	return isValid(x, y) && data[x][y] >= MAP_BREAKABLE && data[x][y] <= MAP_BREAKABLE2;
}

bool Map::isNoReset(int x, int y)
{
	return isValid(x, y) && data[x][y] >= MAP_NORESET && data[x][y] < MAP_DECORATION;
}

bool Map::isLiquid(int x, int y)
{
	return isValid(x, y) && ((data[x][y] >= MAP_WATER && data[x][y] <= MAP_LAVA) || (data[x][y] >= MAP_WATERANIM && data[x][y] <= MAP_LAVAANIM));
}

bool Map::isTopLayer(int x, int y)
{
	return isValid(x, y) && data[x][y] >= MAP_TOPLAYER;
}

std::vector<std::string> &Map::getPersistent(const std::string &name)
{
	return persistents[name];
}

std::vector<std::string> &Map::createPersistent(const std::string &name)
{
	return persistents[name];
}

void Map::destroyPersistent(const std::string &name)
{
	persistents.erase(name);
}

void Map::setName(const std::string &name)
{
	this->name = name;

	if (name.find("BioMech") != name.npos)
	{
		isBossMission = true;
	}

	if (name == "Galdov")
	{
		isBossMission = true;
	}

	if (name == "Final Battle")
	{
		isBossMission = true;
	}

	if (name.find("Ice") != name.npos)
	{
		isIceLevel = true;
	}

	if (name.find("Arctic") != name.npos)
	{
		isIceLevel = true;
		isBlizzardLevel = true;
	}
}

void Map::setClipping(int limitLeft, int limitRight, int limitUp, int limitDown)
{
	if (limitLeft != -1)
		this->limitLeft = limitLeft;
	if (limitRight != -1)
		this->limitRight = limitRight;
	if (limitUp != -1)
		this->limitUp = limitUp;
	if (limitDown != -1)
		this->limitDown = limitDown;
}

void Map::addTrain(const std::string &name, int startX, int startY, int endX, int endY, int pause, bool atStart, bool active)
{
	auto &train = trains.emplace_back(name, TR_TRAIN, startX, startY, endX, endY, pause, atStart);

	train.active = active;
	train.width = 64;
	train.height = 16;

	if (pause == 0)
	{
		debug(("WARNING: TRAIN WITH 0 WAIT TIME ADDED '%s' (%d:%d)\n", name, startX, startY));
	}
}

void Map::addDoor(const std::string &name, int type, int startX, int startY, int endX, int endY, bool active)
{
	auto &train = trains.emplace_back(name, type, startX, startY, endX, endY, 0, false);

	train.active = active;

	if (type < TR_SLIDEDOOR)
	{
		train.width = 16;
		train.height = 64;
	}
	else
	{
		train.width = 64;
		train.height = 16;
	}
}

void Map::addSwitch(const std::string &name, const std::string &linkName, const std::string &requiredObjectName, const std::string &activateMessage, int type, int x, int y, bool activated)
{
	switches.emplace_back(name, linkName, requiredObjectName, activateMessage, type, x, y, activated);
}

void Map::addParticle(float x, float y, float dx, float dy, int health, int color, Sprite *sprite, int flags)
{
	particles.emplace_back(x, y, dx, dy, color, health, flags, sprite);
}

void Map::addSpawnPoint(const std::string &name, int x, int y, int type, int subtype, int min, int max, bool active)
{
	spawns.emplace_back(name, x, y, type, subtype, min, max, active);
}

void Map::addObjective(const std::string &description, const std::string &target, int targetValue, bool required)
{
	objectives.emplace_back(description, target, targetValue, required);
}

void Map::evalTileset(const std::string &baseDir)
{
	if (baseDir.find("caves") != baseDir.npos)
	{
		isCavesTileset = true;
	}
	else if (baseDir.find("grasslands") != baseDir.npos)
	{
		isGrasslandsTileset = true;
	}
}

void Map::killAllEnemies()
{
	for (auto &&enemy: enemies)
		enemy.health = -1;
}

void Map::setAllowableEnemy(Entity *enemy)
{
	for (int i = 0; i < 10; i++)
	{
		if (allowableEnemy[i] == nullptr)
		{
			allowableEnemy[i] = enemy;
			maxAllowableEnemies = i + 1;
			return;
		}
	}

	debug(("WARNING: Can't add anymore spawnable enemies to list!!!\n"));
}

std::string Map::getSpawnableEnemy(int i)
{
	if (allowableEnemy[i] == nullptr)
		return {};

	return allowableEnemy[i]->name;
}

std::string Map::getSpawnableEnemy()
{
	if (maxAllowableEnemies == 0)
	{
		fmt::print("ERROR: No enemy spawn list defined for map '{}'!! Please report this Error!\n", name);
		exit(1);
	}

	return allowableEnemy[Math::prand() % maxAllowableEnemies]->name;
}

void Map::getRandomEntityPosition(int *x, int *y)
{
	size_t count = mias.size() + enemies.size() + items.size();
	size_t i = Math::prand() % count;

	Entity *ent;

	if (i < mias.size())
	{
		auto it = mias.begin();
		std::advance(it, i);
		ent = &*it;
	}
	else if (i < mias.size() + enemies.size())
	{
		auto it = enemies.begin();
		std::advance(it, i - mias.size());
		ent = &*it;
	}
	else
	{
		auto it = items.begin();
		std::advance(it, i - mias.size() - enemies.size());
		ent = &*it;
	}

	*x = (int)ent->x;
	*y = (int)ent->y;
}

void Map::setMainBossPart(Boss *boss)
{
	mainBossPart = boss;

	if (mainBossPart != nullptr)
	{
		bossEnergyMeterBit = 200;
		bossEnergyMeterBit /= boss->maxHealth;
	}
}

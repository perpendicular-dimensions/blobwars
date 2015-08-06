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

SpawnPoint::SpawnPoint()
{
	name[0] = 0;
	active = false;
	x = y = 0;

	spawnType = spawnSubType = 0;

	currentInterval = minInterval = maxInterval = 0;

	next = NULL;
}

void SpawnPoint::reset()
{
	currentInterval = 0;

	requiredInterval = Math::rrand(minInterval, maxInterval);
}

void SpawnPoint::create(const char *name, int x, int y, int spawnType, int spawnSubType, int minInterval, int maxInterval, bool active)
{
	strlcpy(this->name, name, sizeof this->name);
	this->x = x;
	this->y = y;
	this->spawnType = spawnType;
	this->spawnSubType = spawnSubType;
	this->minInterval = minInterval * 60;
	this->maxInterval = maxInterval * 60;
	this->active = active;

	reset();
}

void SpawnPoint::think()
{
	if (active)
		Math::limitInt(&(++currentInterval), 0, maxInterval);
}

bool SpawnPoint::readyToSpawn()
{
	if (currentInterval >= requiredInterval)
		return true;

	return false;
}

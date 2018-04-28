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

/**
* Checks against various objects (Train, SpawnPoint, Teleporter, Trap)
* to see if they should be activated. If the
* object name matches the link name, it's active status is changed.
* @param linkName The name of the link to check against
* @param activateMessage The message to display on successful trigger
* @param active The new status of the object
*/
void activateTrigger(const std::string &linkName, const std::string &activateMessage, bool active)
{
	if (linkName == "@none@")
		return;
		
	if (linkName == "WATERLEVEL")
	{
		int newLevel = stoi(activateMessage);
		
		// only if the new level is less than our current level
		// (ie - raising water up!)
		if (newLevel < map.waterLevel)
		{
			map.requiredWaterLevel = newLevel;
			engine.setInfoMessage("Water level is rising", 1, INFO_ACTIVATE);
		}
		return;
	}

	if (linkName == "OBSTACLERESET")
	{
		if (!active)
			return;

		for (auto &&obstacle: map.obstacles)
		{
			if (obstacle->name == activateMessage)
			{
				addTeleportParticles(obstacle->x + (obstacle->width / 2), obstacle->y + (obstacle->height / 2), 50, SND_TELEPORT2);
				obstacle->place(obstacle->tx, obstacle->ty);
				Math::removeBit(&obstacle->flags, ENT_TELEPORTING);
				obstacle->dx = obstacle->dy = 0;
				obstacle->environment = ENV_AIR;
				addTeleportParticles(obstacle->x + (obstacle->width / 2), obstacle->y + (obstacle->height / 2), 50, SND_TELEPORT2);
			}
		}

		engine.setInfoMessage("Obstacles Reset", 0, INFO_ACTIVATE);

		return;
	}

	bool linkOkay = false;

	for (auto &&train: map.trains)
	{
		if (linkName == train->name)
		{
			train->active = active;
			if ((train->active) && (activateMessage != "@none@"))
				engine.setInfoMessage(activateMessage, 1, INFO_ACTIVATE);

			if (train->type != TR_TRAIN)
				audio.playSound(SND_OPENDOOR, CH_TOUCH, train->x);

			linkOkay = true;
		}
	}

	for (auto &&sp: map.spawns)
	{
		if (linkName == sp->name)
		{
			sp->active = !sp->active;
			if ((sp->active) && (activateMessage != "@none@"))
				engine.setInfoMessage(activateMessage, 1, INFO_ACTIVATE);

			linkOkay = true;
		}
	}

	for (auto &&tele: map.teleports)
	{
		if (linkName == tele->name)
		{
			tele->active = active;
			if ((tele->active) && (activateMessage != "@none@"))
				engine.setInfoMessage(activateMessage, 1, INFO_ACTIVATE);

			linkOkay = true;
		}
	}

	for (auto &&trap: map.traps)
	{
		if (linkName == trap->name)
		{
			toggleTrap(trap.get());
			if (activateMessage != "@none@")
				engine.setInfoMessage(activateMessage, 1, INFO_ACTIVATE);
			linkOkay = true;
		}
	}

	if (linkOkay)
	{
		showMessageLineDef(linkName, active);
	}
	else
	{
		debug(("WARNING : No such object '%s'!\n", linkName));
	}
}

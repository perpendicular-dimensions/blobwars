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

#include "triggers.h"

/**
* Checks against various objects (Train, SpawnPoint, Teleporter, Trap)
* to see if they should be activated. If the
* object name matches the link name, it's active status is changed.
* @param linkName The name of the link to check against
* @param activateMessage The message to display on successful trigger
* @param active The new status of the object
*/
void activateTrigger(const char *linkName, const char *activateMessage, bool active)
{
	if (strcmp(linkName, "@none@") == 0)
		return;
		
	if (strcmp(linkName, "WATERLEVEL") == 0)
	{
		int newLevel = atoi(activateMessage);
		
		// only if the new level is less than our current level
		// (ie - raising water up!)
		if (newLevel < map.waterLevel)
		{
			map.requiredWaterLevel = atoi(activateMessage);
			engine.setInfoMessage("Water level is rising", 1, INFO_ACTIVATE);
		}
		return;
	}

	if (strcmp(linkName, "OBSTACLERESET") == 0)
	{
		if (!active)
			return;

		Entity *obstacle = (Entity*)map.obstacleList.getHead();

		while (obstacle->next != NULL)
		{
			obstacle = (Entity*)obstacle->next;

			if (strcmp(obstacle->name, activateMessage) == 0)
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

	Train *train = (Train*)map.trainList.getHead();
	SpawnPoint *sp = (SpawnPoint*)map.spawnList.getHead();
	Teleporter *tele = (Teleporter*)map.teleportList.getHead();
	Trap *trap = (Trap*)map.trapList.getHead();

	while (train->next != NULL)
	{
		train = (Train*)train->next;

		if (strcmp(linkName, train->name) == 0)
		{
			train->active = active;
			if ((train->active) && (strcmp(activateMessage, "@none@")))
				engine.setInfoMessage(activateMessage, 1, INFO_ACTIVATE);

			if (train->type != TR_TRAIN)
				audio.playSound(SND_OPENDOOR, CH_TOUCH, train->x);

			linkOkay = true;
		}
	}

	while (sp->next != NULL)
	{
		sp = (SpawnPoint*)sp->next;

		if (strcmp(linkName, sp->name) == 0)
		{
			sp->active = !sp->active;
			if ((sp->active) && (strcmp(activateMessage, "@none@")))
				engine.setInfoMessage(activateMessage, 1, INFO_ACTIVATE);

			linkOkay = true;
		}
	}

	while (tele->next != NULL)
	{
		tele = (Teleporter*)tele->next;

		if (strcmp(linkName, tele->name) == 0)
		{
			tele->active = active;
			if ((tele->active) && (strcmp(activateMessage, "@none@")))
				engine.setInfoMessage(activateMessage, 1, INFO_ACTIVATE);

			linkOkay = true;
		}
	}

	while (trap->next != NULL)
	{
		trap = (Trap*)trap->next;

		if (strcmp(linkName, trap->name) == 0)
		{
			toggleTrap(trap);
			if (strcmp(activateMessage, "@none@"))
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

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

#include "teleporters.h"

/**
* Adds a teleporter to the level.
* @param name The group name of the teleporter
* @param x The x location of the teleporter
* @param y The y location of the teleporter
* @param destX The x destination of the teleporter
* @param destY The y destination of the teleporter
* @param active The active state of the teleporter
*/
void addTeleporter(const char *name, int x, int y, int destX, int destY, bool active)
{
	Teleporter *teleport = new Teleporter();

	teleport->setName(name);
	teleport->set(x, y, destX, destY);
	teleport->active = active;
	
	map.addTeleporter(teleport);
}

/**
* Teleporters an entity that touches this teleporter to
* the teleporter's destination location
* @param ent The entity to check against
*/
void checkTeleportContact(Entity *ent)
{
	Teleporter *teleport = (Teleporter*)map.teleportList.getHead();

	while (teleport->next != NULL)
	{
		teleport = (Teleporter*)teleport->next;

		if (!teleport->active)
			continue;

		if (Collision::collision(ent->x + ent->dx, ent->y + ent->dy, ent->width, ent->height, teleport->x + 16, teleport->y - 20, 32, 25))
		{
			ent->dx = teleport->destX;
			ent->dy = teleport->destY;
			Math::addBit(&ent->flags, ENT_TELEPORTING);
			addTeleportParticles(ent->x + (ent->width / 2), ent->y + (ent->height / 2), 50, SND_TELEPORT3);
			
			debug(("%s - Teleporting to %f:%f\n", ent->name, ent->dx, ent->dy));
		}
	}
}

/**
* Loops through all the teleporters and makes them do their thing
*/
void doTeleporters()
{
	Sprite *teleportStar = graphics.getSprite("TeleportStar", true);
	Teleporter *teleport = (Teleporter*)map.teleportList.getHead();

	int x, y;
	float dy;

	while (teleport->next != NULL)
	{
		teleport = (Teleporter*)teleport->next;

		x = (int)(teleport->x - engine.playerPosX);
		y = (int)(teleport->y - engine.playerPosY);

		if ((abs(x) <= 800) && (abs(y) <= 600))
		{
			if (teleport->sprite == NULL)
			{
				teleport->sprite = graphics.getSprite("Teleporter", true);
			}

			graphics.blit(teleport->sprite->getCurrentFrame(), x, y, graphics.screen, false);

			if (teleport->active)
			{
				dy = Math::rrand(-100, -10);
				dy /= 100;
				map.addParticle(teleport->x + Math::prand() % 64, teleport->y, 0, dy, Math::rrand(30, 60), graphics.white, teleportStar, PAR_WEIGHTLESS);
			}
		}
	}
}

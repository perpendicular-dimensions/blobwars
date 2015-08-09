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

#include "obstacles.h"

void addObstacle(const char *name, int x, int y, const char *spriteName)
{
	Entity *obstacle = new Entity();
	
	obstacle->setName(name);
	obstacle->place(x, y);
	obstacle->setSprites(graphics.getSprite(spriteName, true), graphics.getSprite(spriteName, true), graphics.getSprite(spriteName, true));
	
	if (map.isIceLevel)
	{
		Math::addBit(&obstacle->flags, ENT_SLIDES);
	}

	map.addObstacle(obstacle);
}

bool checkObstacleContact(Entity *ent, int dir)
{
	Entity *obstacle = (Entity*)map.obstacleList.getHead();
	
	bool collision = false;

	while (obstacle->next != NULL)
	{
		obstacle = (Entity*)obstacle->next;
		
		if (obstacle->flags & ENT_TELEPORTING)
		{
			continue;
		}

		if (ent == obstacle)
		{
			continue;
		}
			
		if (dir == DIR_X)
		{
			collision = Collision::collision(ent->x + ent->dx, ent->y, ent->width, ent->height - 1, obstacle->x, obstacle->y, obstacle->width, obstacle->height);
		}
		else if (dir == DIR_Y)
		{
			collision = Collision::collision(ent->x, ent->y + ent->dy, ent->width, ent->height - 1, obstacle->x, obstacle->y, obstacle->width, obstacle->height);
		}
		else
		{
			collision = Collision::collision(ent->x + ent->dx, ent->y + ent->dy, ent->width, ent->height - 1, obstacle->x, obstacle->y, obstacle->width, obstacle->height);
		}

		if (collision)
		{
			if (dir & DIR_X)
			{
				if ((ent->y + ent->height == obstacle->y + obstacle->height) || ((ent->flags & ENT_BULLET) && (ent->owner == &player)))
				{
					obstacle->dx = (ent->dx / 2);
					
					if (ent->dx < 0) ent->x = obstacle->x + obstacle->width;
					if (ent->dx > 0) ent->x = obstacle->x - ent->width;
				}
			}

			if (dir & DIR_Y)
			{
				ent->falling = false;
				ent->dy = 0;
			}

			return true;
		}
	}

	return false;
}

void doObstacles()
{
	Entity *obstacle = (Entity*)map.obstacleList.getHead();

	int x, y;

	while (obstacle->next != NULL)
	{
		obstacle = (Entity*)obstacle->next;

		if (obstacle->flags & ENT_TELEPORTING)
		{
			moveEntity(obstacle);
		}
		else
		{
			x = (int)(obstacle->x - engine.playerPosX);
			y = (int)(obstacle->y - engine.playerPosY);

			// Gravity
			if (!(obstacle->flags & ENT_WEIGHTLESS))
			{
				obstacle->applyGravity();
			}

			moveEntity(obstacle);

			if (!(obstacle->flags & ENT_TELEPORTING))
			{
				if (!(obstacle->flags & ENT_SLIDES))
				{
					obstacle->dx = 0;
				}
			}

			graphics.blit(obstacle->getFaceImage(), x, y, graphics.screen, false);

			obstacle->animate();
		}
	}
}

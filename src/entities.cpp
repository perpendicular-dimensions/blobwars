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

#include "entities.h"

void throwAndDamageEntity(Entity *ent, int damage, int minDX, int maxDX, int DY)
{
	if ((ent == &player) && (game.missionOver > 0))
	{
		return;
	}

	if (!(ent->flags & ENT_EXPLODES))
	{
		audio.playSound(SND_HIT, CH_ANY, ent->x);
		for (int i = 0 ; i < 4 ; i++)
		{
			addBlood(ent, Math::rrand(-5, 5), Math::rrand(-6, -3), i);
		}
	}
	else
	{
		audio.playSound(SND_CLANG, CH_ANY, ent->x);
		addColorParticles(ent->x, ent->y, Math::rrand(25, 75), -1);
	}

	if ((ent == &player) && (engine.cheatInvulnerable))
	{
		if (!ent->immune)
		{
			ent->health -= damage;
		}
		return;
	}

	if (ent == &player)
	{
		if (!ent->immune)
		{
			ent->health -= damage;
		}

		ent->immune = 180;

		if (player.health <= 0)
		{
			audio.playSound(SND_DEATH1 + Math::prand() % 3, CH_DEATH, player.x);
			player.health = 0;
		}

		Math::removeBit(&player.flags, ENT_FLIES);
		
		player.setSprites(graphics.getSprite("BobRight", true), graphics.getSprite("BobLeft", true), graphics.getSprite("BobSpin", true));
	}
	else
	{
		ent->health -= damage;
	}

	((Math::prand() % 2) == 0) ? ent->dx = -minDX : ent->dx = maxDX;
	
	if (ent->dy >= 0)
	{
		ent->dy = DY;
	}
	else
	{
		ent->dy = -DY;
	}
}

bool checkBrickContactX(Entity *ent)
{
	int new_ent_x = (int)round((ent->x + ent->dx) * 100) / 100;
	int ent_y = (int)round(ent->y * 100) / 100;
	int x1 = new_ent_x >> BRICKSHIFT;
	int x2 = (new_ent_x + ent->width - 1) >> BRICKSHIFT;
	int y1 = ent_y >> BRICKSHIFT;
	int y2 = (ent_y + ent->height - 1) >> BRICKSHIFT;
	
	if ((x1 < 0) || (x2 < 0) || (y1 < 0) || (y2 < 0))
	{
		return true;
	}

	int mapAttribute = map.data[x1][y2];

	evaluateMapAttribute(ent, mapAttribute);

	if ((ent->flags & ENT_SWIMS) && (mapAttribute == MAP_AIR))
		return true;

	if (ent->dx < 0)
	{
		if ((map.isSolid(x1, y1)) || (map.isSolid(x1, y2)))
		{
			ent->x = (x1 + 1) * BRICKSIZE;

//			if (map.isSolid(x1, y2))
//			{
//				ent->falling = false;
//			}

			return true;
		}
	}
	else if (ent->dx > 0)
	{
		if ((map.isSolid(x2, y1)) || (map.isSolid(x2, y2)))
		{
			ent->x = (x2 * BRICKSIZE) - ent->width;

//			if (map.isSolid(x1, y2))
//			{
//				ent->falling = false;
//			}

			return true;
		}
	}

	return false;
}

bool checkBrickContactY(Entity *ent)
{
	int ent_x = (int)round(ent->x * 100) / 100;
	int new_ent_y = (int)round((ent->y + ent->dy) * 100) / 100;
 	int x1 = ent_x >> BRICKSHIFT;
	int x2 = (ent_x + ent->width - 1) >> BRICKSHIFT;
	int y1 = new_ent_y >> BRICKSHIFT;
	int y2 = (new_ent_y + ent->height - 1) >> BRICKSHIFT;
	
	if ((x1 < 0) || (x2 < 0) || (y1 < 0) || (y2 < 0))
	{
		return true;
	}

	int mapAttribute = map.data[x1][y2];
	
	if (ent->dy < 0)
	{
		mapAttribute = map.data[x1][y1];
	}

	evaluateMapAttribute(ent, mapAttribute);
	
	if (ent->flags & ENT_SWIMS)
	{
		switch (mapAttribute)
		{
			case MAP_AIR:
			case MAP_AIR_WALL_1:
			case MAP_AIR_WALL_2:
			case MAP_AIR_WALL_3:
			case MAP_AIR_CEILING_1:
				return true;
			case MAP_AIR_WALL_4:
				if (map.isCavesTileset)
				{
					return true;
				}
				break;
			case MAP_AIR_CEILING_2:
				if (map.isGrasslandsTileset)
				{
					return true;
				}
				break;
		}
	}

	if (ent->dy < 0)
	{
		if ((map.isSolid(x1, y1)) || (map.isSolid(x2, y1)))
		{
			ent->y = (y1 + 1) * BRICKSIZE;
			ent->falling = false;

			return true;
		}
	}
	else if (ent->dy > 0)
	{
		ent->falling = true;
		
		if ((map.isSolid(x1, y2)) || (map.isSolid(x2, y2)))
		{		
			ent->falling = false;
			
			ent->y = (y2 * BRICKSIZE) - ent->height;
			
			if ((map.isSolid(x1, y2)) && (!map.isBreakable(x1, y2)) && (!map.isNoReset(x1, y2)))
			{
				if ((ent == &player) && (player.environment == ENV_AIR))
				{
					game.setCheckPoint(x1 * BRICKSIZE, (y2 * BRICKSIZE) - BRICKSIZE);
					
					if (engine.practice)
					{
						game.setObjectiveCheckPoint();
					}
				}
			}

			if ((map.isSolid(x2, y2)) && (!map.isBreakable(x2, y2)) && (!map.isNoReset(x2, y2)))
			{         
				if ((ent == &player) && (player.environment == ENV_AIR))
				{
					game.setCheckPoint(x2 * BRICKSIZE, (y2 * BRICKSIZE) - BRICKSIZE);
					if (engine.practice)
					{
						game.setObjectiveCheckPoint();	
					}
				}
			}

			return true;
		}
	}

	return false;
}

void moveEntity(Entity *ent)
{
	if (ent->owner->flags & ENT_TELEPORTING)
	{
		int diffX = (abs((int)ent->x - (int)ent->dx) / 20);
		int diffY = (abs((int)ent->y - (int)ent->dy) / 20);

		// add teleport particles so we can see where thing are going (no sound)
		addTeleportParticles(ent->x + Math::prand() % ent->width, ent->y + Math::prand() % ent->height, 3, -1);

		Math::limitInt(&diffX, 3, 30);
		Math::limitInt(&diffY, 3, 30);

		if (ent->x > ent->dx) ent->x -= diffX;
		if (ent->x < ent->dx) ent->x += diffX;
		if (ent->y > ent->dy) ent->y -= diffY;
		if (ent->y < ent->dy) ent->y += diffY;

		if (Collision::collision(ent->x, ent->y, ent->width, ent->height, ent->dx, ent->dy, ent->width, ent->height))
		{
			Math::removeBit(&ent->flags, ENT_TELEPORTING);
			addTeleportParticles(ent->x + (ent->width / 2), ent->y + (ent->height / 2), 25, SND_TELEPORT3);
			ent->dx = ent->dy = 0;
			ent->environment = ENV_AIR;
			
			if (ent == &player)
			{
				if (player.flags & ENT_FLIES)
				{
					player.setSprites(graphics.getSprite("JPBobRight", true), graphics.getSprite("JPBobLeft", true), graphics.getSprite("BobSpin", true));
				}
				else
				{
					player.setSprites(graphics.getSprite("BobRight", true), graphics.getSprite("BobLeft", true), graphics.getSprite("BobSpin", true));
				}
			}
			
			// raise to the floor
 			int x1 = (int)ent->x >> BRICKSHIFT;
			int x2 = ((int)ent->x + ent->width - 1) >> BRICKSHIFT;
			int y2 = ((int)ent->y + ent->height - 1) >> BRICKSHIFT;
			if ((map.isSolid(x1, y2)) || (map.isSolid(x2, y2)))
			{
				ent->y = (y2 * BRICKSIZE) - ent->height;
			}

			debug(("%s reappeared at %f:%f\n", ent->name, ent->x, ent->y));
		}

		return;
	}

	if (ent->owner != ent)
	{
		return;
	}
		
	ent->falling = true;
	
	if (ent != &player)
	{
		if ((!(ent->flags & ENT_WEIGHTLESS)) && (!(ent->flags & ENT_FLIES)) && (!(ent->flags & ENT_SWIMS)))
		{
			ent->applyGravity();
		}
	}
	else if (!(ent->flags & ENT_FLIES))
	{
		if (ent->environment == ENV_AIR)
		{
			ent->applyGravity();
		}
		// This is what makes swimming work:
		else if ((!config.isControl(CONTROL::UP)) && (!config.isControl(CONTROL::JUMP))  && (!config.isControl(CONTROL::DOWN)))
		{
			ent->applyGravity();
		}
	}

	if (ent->dx != 0)
	{		
		if ((checkBrickContactX(ent)) || (checkObstacleContact(ent, DIR_X)) || (checkTrainContact(ent, DIR_X)))
		{
			ent->dx = 0;
		}

		ent->x += ent->dx;

		if (ent->flags & ENT_SLIDES)
		{
			ent->dx *= 0.98;
		}
	}

	if (ent->dy != 0)
	{
		if ((checkBrickContactY(ent)) || (checkObstacleContact(ent, DIR_Y)) || (checkTrainContact(ent, DIR_Y)))
		{
			if ((ent->flags & ENT_BOUNCES) && (ent->dy >= 3))
			{
				ent->dy = (0 - ent->dy / 2);
			}
			else
			{
				ent->dy = 0;
			}
		}
		
		ent->y += ent->dy;
	}
	
	checkSwitchContact(ent);
	checkTeleportContact(ent);

//	Math::limitFloat(&ent->x, 10, (MAPWIDTH * BRICKSIZE) - 20);
	if (ent->x < 10)
	{
		ent->x = 10;
		if (ent->dx < 0)
		{
			ent->dx = 0;
		}
	}
	else if (ent->x > (MAPWIDTH * BRICKSIZE) - 20)
	{
		ent->x = (MAPWIDTH * BRICKSIZE) - 20;
		if (ent->dx > 0)
		{
			ent->dx = 0;
		}
	}
}

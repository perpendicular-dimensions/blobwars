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

#include "explosions.h"

void addExplosion(float x, float y, int radius, Entity *owner)
{
	audio.playSound(SND_GRENADE, CH_EXPLODE, x);

	float dx, dy;
	int distX, distY;
	int distance;

	Sprite *explosion = graphics.getSprite("Explosion", true);

	for (int i = 0 ; i < radius ; i++)
	{
		dx = Math::rrand(-radius, radius); dx /= 10;
		dy = Math::rrand(-radius, radius); dy /= 10;
		map.addParticle(x, y, dx, dy, Math::rrand(5, 30), graphics.white, explosion, PAR_WEIGHTLESS);
	}

	Entity *enemy = (Entity*)map.enemyList.getHead();

	while (enemy->next != NULL)
	{
		enemy = (Entity*)enemy->next;
		
		if ((enemy->flags & ENT_IMMUNE) || (enemy->flags & ENT_IMMUNEEXPLODE))
		{
			continue;
		}
		
		if (enemy->dead == DEAD_DYING)
		{
			continue;
		}

		distX = (int)fabs(enemy->x + (enemy->width / 2) - x);
		distY = (int)fabs(enemy->y + (enemy->height / 2) - y);

		distX *= distX;
		distY *= distY;

		distance = (int)sqrt(distX + distY);

		if (radius - distance > 0)
		{	
			if (enemy->health > 0)
			{
				enemy->health -= radius - distance;
				
				if (enemy->health <= 0)
				{	
					checkObjectives("Enemy", false);
					checkObjectives(enemy->name, false);
				}
				
				if (!(enemy->flags & ENT_STATIC))
				{
					if (enemy->flags & ENT_EXPLODES)
					{
						audio.playSound(SND_ELECDEATH1 + Math::prand() % 3, CH_DEATH, enemy->x);
					}
					else
					{
						if (game.gore)
						{
							audio.playSound(SND_DEATH1 + Math::prand() % 3, CH_DEATH, enemy->x);
						}
					}
				}
				
				if (owner == &player)
				{
					addPlayerScore(enemy->value);
				}
			}
			
			for (int i = 0 ; i < 4 ; i++)
			{
				(enemy->flags & ENT_EXPLODES) ? addSmokeAndFire(enemy, Math::rrand(-15, 5), Math::rrand(-15, 5), 2) : addBlood(enemy, Math::rrand(-5, 5), Math::rrand(-5, 5), 1);
			}
				
			if (!(enemy->flags & ENT_STATIC))
			{
				enemy->dx = Math::rrand(-5, 5);
				enemy->dy = Math::rrand(-5, 0);
			}
		}
	}

	if ((player.immune) || (player.health <= -60) || (owner == &player) || (game.missionOver > 0))
	{
		return;
	}

	distX = (int)fabs(player.x + (player.width / 2) - x);
	distY = (int)fabs(player.y + (player.height / 2) - y);

	distX *= distX;
	distY *= distY;

	distance = (int)sqrt(distX + distY);

	if (radius - distance >= 0)
		throwAndDamageEntity(&player, 2, -3, 3, -8);
}

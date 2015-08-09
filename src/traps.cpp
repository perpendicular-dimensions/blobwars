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

#include "traps.h"

/**
* Adds a trap to the map
* @param name The name of the trap group
* @param trapType The type of trap this is
* @param damage The damage the trap does
* @param speed The speed at which the trap moves
* @param startY The starting X location of the trap
* @param startY The starting Y location of the trap
* @param startY The ending X location of the trap
* @param startY The ending Y location of the trap
* @param wait1 The first state wait time
* @param wait2 The second state wait time
* @param sprite The Sprite to use with this trap
* @param active The active state of this trap
*/
void addTrap(const char *name, int trapType, int damage, int speed, int startX, int startY, int endX, int endY, int wait1, int wait2, const char *sprite, bool active)
{
	Trap *trap = new Trap();

	trap->setName(name);
	trap->setTrapType(trapType);
	trap->setDamage(damage);
	trap->setSpeed(speed);
	trap->setDestinations(startX, startY, endX, endY);
	
	if (wait1 == -1) wait1 = Math::rrand(10, 60);
	if (wait2 == -1) wait2 = Math::rrand(10, 60);

	trap->setWaitTimes(wait1, wait2);
	trap->setSprite(graphics.getSprite(sprite, true));
	trap->active = active;

	if (trap->type == TRAP_TYPE_BARRIER)
	{
		trap->currentAction = TRAP_FIRSTACTION;
		trap->thinktime = wait1;
		if (trap->thinktime == 0)
			trap->thinktime = 1;
	}

	map.addTrap(trap);
}

/**
* Toggles the active state of a trap
* @param The trap to toggle
*/
void toggleTrap(Trap *trap)
{
	trap->active = !trap->active;

	if (trap->type == TRAP_TYPE_MINE)
	{
		if (trap->active)
		{
			trap->setSprite(graphics.getSprite("ActiveMine", true));
		}
		else
		{
			trap->setSprite(graphics.getSprite("InActiveMine", true));
		}
	}
}

/**
* Draws the chain link for the spiked ball traps.
* @param trap The trap that the chain link should be drawn from
*/
void drawTrapChain(Trap *trap)
{
	float x = trap->startX;
	float y = trap->startY;
	int tx = (int)trap->x + (trap->sprite->image[0]->w / 2);
	int ty = (int)trap->y + (trap->sprite->image[0]->h / 2);
	float dx, dy;

	Math::calculateSlope(tx, ty, trap->startX, trap->startY, &dx, &dy);

	dx *= 7;
	dy *= 7;

	SDL_Surface *chainLink = graphics.getSprite("ChainLink", true)->getCurrentFrame();

	if ((dx == 0) && (dy == 0))
		return;

	while (true)
	{
		graphics.blit(chainLink, (int)(x - engine.playerPosX), (int)(y - engine.playerPosY), graphics.screen, true);
		
		x += dx;
		y += dy;

		if (Collision::collision(x, y, 3, 3, tx, ty, 4, 4))
			break;
	}
}

/**
* Checks for a trap colliding with enemies or the Player
* @param trap The trap to perform the check
* @return Whether the trap made contact with an enemy or the Player
*/
bool doTrapCollisions(Trap *trap)
{
	bool hit = false;

	if ((trap->type == TRAP_TYPE_SPIKE) && (trap->currentAction != TRAP_FIRSTACTION))
	{
		return false;
	}

	Entity *enemy = (Entity*)map.enemyList.getHead();

	while (enemy->next != NULL)
	{
		enemy = (Entity*)enemy->next;

		if (Collision::collision(trap->x, trap->y, trap->width, trap->height, enemy->x, enemy->y, enemy->width, enemy->height))
		{
			throwAndDamageEntity(enemy, trap->damage, -5, 5, -8);
			hit = true;
		}
	}

	if ((player.immune > 0) && (player.immune <= 120))
	{
		return hit;
	}

	if (Collision::collision(trap->x, trap->y, trap->width, trap->height, player.x, player.y, player.width, player.height))
	{
		throwAndDamageEntity(&player, trap->damage, -5, 5, -8);
		
		if (trap->damage == 10)
		{
			player.health = 0;
		}
		
		if (trap->type == TRAP_TYPE_MINE)
		{
			for (int i = 0 ; i < 10 ; i++)
			{
				addExplosion(trap->x + Math::rrand(-15, 15), trap->y + Math::rrand(-15, 15), 50, &engine.world);
			}
		}
		hit = true;
	}

	return hit;
}

/**
* Loops through all the traps and makes them do their thing according
* to the type of trap that they are
*/
void doTraps()
{
	Trap *trap = (Trap*)map.trapList.getHead();
	Trap *previous = trap;

	bool remove = false;
	int x, y, mx, my;
	unsigned int absX, absY;

	while (trap->next != NULL)
	{
		trap = (Trap*)trap->next;
		
		x = (int)(trap->x - engine.playerPosX);
		y = (int)(trap->y - engine.playerPosY);

		if (trap->type == TRAP_TYPE_SWING)
		{
			absX = abs(x - trap->endX);
			absY = abs(y - trap->endY);
		}
		else
		{
			absX = abs(x);
			absY = abs(y);
		}
		
		/*
			Barrier traps must be do at all times. Otherwise there is an "break" in the
			line if more than one barrier is used over a large space and the barrier becomes
			inconsistant.
		*/

		if (((absX < 800) && (absY < 600)) || (trap->type == TRAP_TYPE_BARRIER) || (trap->type == TRAP_TYPE_FLAME))
		{
			remove = false;

			if ((trap->think()) && (trap->active))
			{
				switch (trap->type)
				{
					case TRAP_TYPE_MINE:
						audio.playSound(SND_ROCKET, CH_SPAWN, trap->x);
						break;
					case TRAP_TYPE_SPIKE:
						audio.playSound(SND_ROCKET, CH_SPAWN, trap->x);
						break;
					case TRAP_TYPE_SWING:
						audio.playSound(SND_THROW, CH_SPAWN, trap->x);
						break;
					case TRAP_TYPE_BARRIER:
						if ((absX <= 640) && (absY <= 480))
						{
							audio.playSound(SND_ELECTRICITY1 + Math::prand() % 3, CH_SPAWN, trap->x);
						}
						break;
					case TRAP_TYPE_FLAME:
						if ((absX <= 640) && (absY <= 480))
						{
							audio.playSound(SND_FIRECRACKLE, CH_SPAWN, trap->x);
						}
						break;
				}
			}

			if (trap->type == TRAP_TYPE_MINE)
			{
				mx = (int)trap->x >> BRICKSHIFT;
				my = (int)(trap->y + trap->dy + trap->sprite->image[0]->h) >> BRICKSHIFT;

				if (map.isSolid(mx, my))
				{
					trap->y = (my * BRICKSIZE) - trap->sprite->image[0]->h;
					trap->dy = 0;
				}
			}

			if (trap->type == TRAP_TYPE_SWING)
			{
				drawTrapChain(trap);
			}

			switch (trap->type)
			{
				case TRAP_TYPE_FLAME:
				
					graphics.blit(trap->sprite->getCurrentFrame(), x, y, graphics.screen, true);

					if ((absX <= 640) && (absY <= 480))
					{
						if (trap->currentAction == TRAP_FIRSTACTION)
						{
							if (trap->active)
							{
								engine.world.place((int)trap->x + (trap->startX * 8), (int)trap->y + (trap->startY * 8));
								engine.world.currentWeapon = &weapon[WP_FLAMETHROWER];
	
								if (trap->startX == -1)
								{
									addBullet(&engine.world, Math::rrand(-500, -100) / 100.00, Math::rrand(-20, 20) / 100.00);
								}
								else if (trap->startX == 1)
								{
									addBullet(&engine.world, Math::rrand(100, 500) / 100.00, Math::rrand(-2, 20) / 100.00);
								}
								else if (trap->startY == -1)
								{
									addBullet(&engine.world, Math::rrand(-20, 20) / 100.00, Math::rrand(-500, -100) / 100.00);
								}
								else if (trap->startY == 1)
								{
									addBullet(&engine.world, Math::rrand(-20, 20) / 100.00, Math::rrand(100, 500) / 100.00);
								}
							}
						}
					}
					break;

				case TRAP_TYPE_BARRIER:
					if (trap->currentAction == TRAP_FIRSTACTION)
					{
						if (trap->active)
						{
							graphics.blit(trap->sprite->getCurrentFrame(), x, y, graphics.screen, false);
							doTrapCollisions(trap);
						}
					}
					break;

				default:
					graphics.blit(trap->sprite->getCurrentFrame(), x, y, graphics.screen, false);
    				if (trap->active)
					{
						remove = doTrapCollisions(trap);
					}
					break;
			}
		}


		if (trap->type != TRAP_TYPE_MINE)
			remove = false;

		if (!remove)
		{
			previous = trap;
		}
		else
		{
			map.trapList.remove(previous, trap);
			trap = previous;
		}
	}
}

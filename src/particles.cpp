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

#include "particles.h"

void addWindParticles()
{
	int c;
	float x, y, dx, dy;
	

	for (int i = 0 ; i < 50 ; i++)
	{
		if (player.x < 320)
		{
			x = Math::rrand(-100, 700);
		}
		else
		{
			x = player.x + Math::rrand(-450, 450);
		}
			
		c = Math::prand() % 4;
		switch (c)
		{
			case 0: c = graphics.white; break;
			case 1: c = graphics.lightGrey; break;
			case 2: c = graphics.grey; break;
			case 3: c = graphics.darkGrey; break;
		}
		
		y = player.y + Math::rrand(-450, 450);
		dx = Math::rrand(1, 100) * map.windPower;
		dx /= 100;
		dy = Math::rrand(1, 10); dy /= 10;
		map.addParticle(x, y, dx, dy, 120, c, NULL, PAR_WEIGHTLESS);
	}
}

void addColorParticles(float x, float y, int amount, int color)
{
	int c = color;
	float dx, dy;

	for (int i = 0 ; i < amount ; i++)
	{
		if (color == -1)
			c = Math::prand() % 5;

		switch (c)
		{
			case 0: c = graphics.white; break;
			case 1: c = graphics.grey; break;
			case 2: c = graphics.blue; break;
			case 3: c = graphics.cyan; break;
			case 4: c = graphics.red; break;
		}

		dx = Math::rrand(-30, 30); dx /= 30;
		dy = Math::rrand(-30, 30); dy /= 30;
		map.addParticle(x, y, dx, dy, Math::rrand(5, 30), c, NULL, 0);
	}
}

void addFireTrailParticle(float x, float y)
{
	map.addParticle(x, y, 0, 0, 12, graphics.red, graphics.getSprite("SmallExplosion", true), PAR_WEIGHTLESS);
}

void addFireParticles(float x, float y, int amount)
{
	for (int i = 0 ; i < amount ; i++)
	{
		map.addParticle(x + Math::rrand(-2, 2), y + Math::rrand(-2, 2), 0, 1, Math::rrand(5, 30), graphics.red, graphics.getSprite("Explosion", true), PAR_COLLIDES);
	}
}

void addBubble(float x, float y)
{
	if ((Math::prand() % 50) == 0)
	{
		map.addParticle(x + Math::prand() % BRICKSIZE, y + 19, 0, Math::rrand(-3, -1), Math::rrand(30, 90), graphics.red, graphics.getSprite("Bubble", true), PAR_COLLIDES | PAR_WEIGHTLESS | PAR_LIQUID);
	}
}

void throwStalagParticles(float x, float y)
{
	Sprite *stalagPiece = graphics.getSprite("StalagPiece", true);
	
	int amount = Math::rrand(3, 6);
	
	for (int i = 0 ; i < amount ; i++)
	{
		map.addParticle(x, y, Math::rrand(-2, 2), Math::rrand(-3, -1), Math::rrand(5, 30), graphics.red, stalagPiece, 0);
	}
}

void throwBrickParticles(float x, float y)
{
	int amount = Math::rrand(4, 8);
	
	Sprite *wallPiece = graphics.getSprite("WallPiece", true);
	
	for (int i = 0 ; i < amount ; i++)
	{
		map.addParticle(x, y, Math::rrand(-2, 2), Math::rrand(-4, -1), Math::rrand(5, 30), graphics.red, wallPiece, 0);
	}
}

void addTeleportParticles(float x, float y, int amount, int soundToPlay)
{
	Sprite *teleportStar = graphics.getSprite("TeleportStar", true);
	float dx, dy;

	for (int i = 0 ; i < amount ; i++)
	{
		dx = Math::rrand(-30, 30); dx /= 20;
		dy = Math::rrand(-30, 30); dy /= 20;
		map.addParticle(x, y, dx, dy, Math::rrand(30, 60), graphics.red, teleportStar, PAR_WEIGHTLESS);
	}

	if (soundToPlay != -1)
	{
		audio.playSound(soundToPlay, CH_SPAWN, x);
	}
}

void doParticles()
{
	Particle *particle = (Particle*)map.particleList.getHead();
	Particle *previous = particle;

	int x, y;

	while (particle->next != NULL)
	{
		particle = (Particle*)particle->next;

		x = (int)(particle->x - engine.playerPosX);
		y = (int)(particle->y - engine.playerPosY);

		if (particle->sprite == NULL)
		{
			graphics.lock(graphics.screen);
			
			graphics.putPixel(x, y, particle->color, graphics.screen);
			
			graphics.unlock(graphics.screen);
		}
		else
		{
			graphics.blit(particle->getFrame(), x, y, graphics.screen, false);
		}

		particle->health--;

		particle->move();

		if (!(particle->flags & PAR_WEIGHTLESS))
		{
			particle->dy += 0.1;
		}

		x = (int)particle->x >> BRICKSHIFT;
		y = (int)particle->y >> BRICKSHIFT;

		if (particle->flags & PAR_COLLIDES)
		{
			if (map.isSolid(x, y))
			{
				particle->health = 0;
			}
		}

		if (particle->flags & PAR_LIQUID)
		{
			if (!map.isLiquid(x, y))
			{
				particle->health = 0;
			}
		}

		if (particle->health > 0)
		{
			previous = particle;
		}
		else
		{
			map.particleList.remove(previous, particle);
			particle = previous;
		}
	}
}

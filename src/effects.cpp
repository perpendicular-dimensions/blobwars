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

#include "effects.h"

void addEffect(float x, float y, float dx, float dy, int flags)
{
	Effect *effect = new Effect();

	effect->create(x, y, dx, dy, flags);

	map.addEffect(effect);
}

void addColoredEffect(float x, float y, float dx, float dy, int color, int flags)
{
	Effect *effect = new Effect();

	effect->create(x, y, dx, dy, flags);
	effect->health = Math::rrand(60, 90);
	effect->color = color;

	map.addEffect(effect);
}

void addSmokeAndFire(Entity *ent, float dx, float dy, int amount)
{
	int x, y;

	for (int i = 0 ; i < amount ; i++)
	{
		x = (int)(ent->x + Math::prand() % ent->width);
		y = (int)(ent->y + Math::prand() % ent->height);
		if ((Math::prand() % 4) > 0)
		{
			addEffect(x, y, dx, dy, EFF_TRAILSFIRE);
		}
		else
		{
			addEffect(x, y, dx, dy, EFF_SMOKES);
		}
	}
}

void addBlood(Entity *ent, float dx, float dy, int amount)
{
	int x, y;
	
	if (engine.cheatBlood)
	{
		amount *= 3;
	}

	if (game.gore)
	{
		for (int i = 0 ; i < amount ; i++)
		{
			x = (int)(ent->x + Math::prand() % ent->width);
			y = (int)(ent->y + Math::prand() % ent->height);
			addEffect(x, y, dx, dy, EFF_BLEEDS);
		}
	}
}

void doEffects()
{
	Effect *effect = (Effect*)map.effectList.getHead();
	Effect *previous = effect;

	Sprite *blood = graphics.getSprite("RedBloodParticle", true);
	Sprite *explosion = graphics.getSprite("SmallExplosion", true);
	Sprite *smoke = graphics.getSprite("Smoke", true);

	int x, y;

	while (effect->next != NULL)
	{
		effect = (Effect*)effect->next;

		effect->update();

		if (effect->flags & EFF_BLEEDS)
		{
			map.addParticle(effect->x, effect->y, 0, 1, Math::rrand(5, 30), graphics.red, blood, PAR_COLLIDES);
		}
		else if (effect->flags & EFF_TRAILSFIRE)
		{
			map.addParticle(effect->x, effect->y, 0, 1, Math::rrand(5, 30), graphics.red, explosion, PAR_COLLIDES);
		}
		else if (effect->flags & EFF_SMOKES)
		{
			map.addParticle(effect->x, effect->y, 0, 1, Math::rrand(5, 30), graphics.red, smoke, PAR_COLLIDES);
		}
		else if (effect->flags & EFF_COLORED)
		{
			map.addParticle(effect->x, effect->y, 0, 1, Math::rrand(5, 30), effect->color, NULL, PAR_COLLIDES);
		}

		x = (int)(effect->x - engine.playerPosX);
		y = (int)(effect->y - engine.playerPosY);

		if ((x < 0) || (y < 0))
		{
			effect->health = 0;
		}
		else
		{
			x = (int)effect->x >> BRICKSHIFT;
			y = (int)effect->y >> BRICKSHIFT;

			if (map.isSolid(x, y))
			{
				effect->health = 0;
			}
		}

		if (effect->health > 0)
		{
			previous = effect;
		}
		else
		{
			map.effectList.remove(previous, effect);
			effect = previous;
		}
	}
}

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

Entity::Entity()
{
	name[0] = 0;
	x = y = dx = dy = tx = ty = width = height = 0;
	id = 0;
	health = 0;
	immune = 120;
	environment = ENV_AIR;
	value = 0;
	oxygen = 7;
	falling = false;
	fuel = 7;
	thinktime = baseThink = 0;
	next = NULL;
	owner = this;
	face = 0;
	reload = 0;
	deathSound = -1;
	flags = 0;
	falling = false;
	owner = this;
	dead = DEAD_ALIVE;
	referenced = false;

	for (int i = 0 ; i < 3 ; i++)
	{
		sprite[i] = NULL;
	}
}

void Entity::setName(const char *name)
{
	strlcpy(this->name, name, sizeof this->name);
}

void Entity::setSprites(Sprite *sprite1, Sprite *sprite2, Sprite *sprite3)
{
	sprite[0] = sprite1;
	sprite[1] = sprite2;
	sprite[2] = sprite3;

	currentFrame = 0;

	if (sprite[0]->maxFrames > 0)
	{
		currentFrame = Math::prand() % (sprite[0]->maxFrames + 1);
	}

	width = sprite[0]->image[0]->w;
	height = sprite[0]->image[0]->h;

	currentTime = 1;
}

void Entity::animate()
{
	currentTime--;

	if (currentTime == 0)
	{
		currentFrame++;
		sprite[face]->getNextFrame(&currentFrame, &currentTime);
	}
}

SDL_Surface *Entity::getFaceImage()
{
	if ((health > 0) && (immune <= 120))
	{
		return sprite[face]->image[currentFrame];
	}

	return sprite[2]->getCurrentFrame();
}

void Entity::place(int x, int y)
{
	this->x = this->tx = x;
	this->y = this->ty = y;
}

void Entity::setVelocity(float dx, float dy)
{
	this->dx = dx;
	this->dy = dy;
}

void Entity::move()
{
	x += dx;
	y += dy;
}

void Entity::setRandomVelocity()
{
	dx = Math::rrand(-100, 100);
	dx /= 50;

	dy = Math::rrand(-15, 0);
}

void Entity::applyGravity()
{
	if ((environment == ENV_AIR) && (!(flags & ENT_WEIGHTLESS)))
		dy += 0.5;
	else
		dy = 1;

	Math::limitFloat(&dy, -12, 12);
}

void Entity::checkEnvironment()
{
	switch (environment)
	{
		case ENV_AIR:
			Math::limitChar(&(oxygen += 2), 0, 7);
			break;
		case ENV_WATER:
			if (!(flags & ENT_SWIMS))
			{
				Math::limitChar(&(oxygen -= 1), 0, 7);
				if (oxygen == 0)
				{
					health--;
					thinktime = 30;
				}
			}
			break;
		case ENV_SLIME:
			if (immune == 0)
				health--;
			break;
		case ENV_LAVA:
			if (immune == 0)
				health -= 2;
			break;
	}
}

void Entity::think()
{
	if (immune > 0)
	{
		immune--;

		if ((falling) && (immune == 121) && (environment == ENV_AIR))
		{
			immune = 122;
		}
	}

	Math::wrapInt(&(--thinktime), 0, baseThink);
	Math::limitInt(&(--reload), 0, 255);

	if (flags & ENT_DYING)
	{
		health--;
	}
		
	if (flags & ENT_FLIES)
	{
		if (thinktime == 0)
		{
			fuel--;
			if (fuel == 0)
			{
				Math::removeBit(&flags, ENT_FLIES);
			}
		}
	}
	else
	{
		if (thinktime == 0)
		{
			fuel++;
		}
	}

	Math::limitChar(&fuel, 0, 7);

	if ((thinktime == 0) && (!(flags & ENT_INANIMATE)))
	{
		checkEnvironment();
	}
}

/*
Copyright (C) 2004-2010 Parallel Realities

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

Particle::Particle()
{
	x = y = 0;
	dx = dy = 0;
	sprite = NULL;
	color = 0;
	next = NULL;
}

void Particle::set(float x, float y, float dx, float dy, int color, int health, int flags)
{
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	this->color = color;
	this->health = health;
	this->flags = flags;
}

void Particle::setSprite(Sprite *sprite)
{
	if (sprite == NULL)
		return;

	this->sprite = sprite;

	currentFrame = Math::prand() % (sprite->maxFrames + 1);
	currentTime = 1;
}

SDL_Surface *Particle::getFrame()
{
	currentTime--;

	if (currentTime == 0)
	{
		currentFrame++;
		sprite->getNextFrame(&currentFrame, &currentTime);
	}

	return sprite->image[currentFrame];
}

void Particle::move()
{
	x += dx;
	y += dy;
}

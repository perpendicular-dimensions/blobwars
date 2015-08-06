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

Effect::Effect()
{
	x = y = dx = dy = 0;
	health = 0;
	//frames = frameTime = currentFrameTime = 0;
	flags = 0;
	color = 0;

	/*
	for (int i = 0 ; i < 4 ; i++)
	{
		frame[i] = NULL;
	}
	*/

	next = NULL;
}

void Effect::create(float x, float y, float dx, float dy, int flags)
{
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	this->flags = flags;

	this->health = 120;
}

void Effect::update()
{
	x += dx;
	y += dy;

	if (!(flags & EFF_WEIGHTLESS))
	{
		dy += 0.5;
	}

	health--;
}

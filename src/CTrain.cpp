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

Train::Train()
{
	active = true;
	type = TR_TRAIN;
	next = NULL;
	sprite = NULL;
	waitAtStart = true;
}

float Train::getDX()
{
	if ((think > 0) || (!active))
		return 0;

	return dx;
}

float Train::getDY()
{
	if ((think > 0) || (!active))
		return 0;

	return dy;
}

bool Train::isReady()
{
	if (think == 0)
	{
		think = 2;
		return true;
	}

	think = 2;

	return false;
}

bool Train::isMoving()
{
	if (think == 0)
		return true;
		
	return false;
}

bool Train::waitsForPlayer()
{
	if ((active) || (pause != 0))
		return false;

	return true;
}

void Train::setName(const char *name)
{
	strlcpy(this->name, name, sizeof this->name);
}

void Train::set(int startX, int startY, int endX, int endY, int pause, bool fromStart)
{
	this->startX = startX;
	this->startY = startY;
	this->endX = endX;
	this->endY = endY;

	this->pause = pause;
	this->think = 0;

	Math::calculateSlope(startX, startY, endX, endY, &dx, &dy);
	
	if ((dx != 0) && (dy != 0))
		debug(("WARNING: TRAIN '%s' is not straight! - %d %d %d %d\n", name, startX, startY, endX, endY));

	if (fromStart)
	{
		this->x = this->startX;
		this->y = this->startY;
		waitAtStart = true;
	}
	else
	{
		this->x = this->endX;
		this->y = this->endY;
		if (type == TR_TRAIN)
		{
			dx = -dx;
			dy = -dy;
		}
		waitAtStart = false;
	}
}

bool Train::openClose()
{
	Math::limitInt(&(--think), 0, 9999);

	if (active)
	{
		if ((x > startX) || (y > startY))
		{
			x += dx;
			y += dy;
			
			if ((x == startX) && (y == startY))
				return true;
		}

		return false;
	}
	else
	{
		if ((x < endX) || (y < endY))
		{
			x -= dx;
			y -= dy;

			if ((x == endX) && (y == endY))
				return true;
		}
		
		return false;
	}
}

void Train::move()
{
	if (think > 0)
	{
		think--;
		return;
	}

	if (!active)
		return;

	x -= dx;
	y -= dy;

	if (((x >= endX) && (y >= endY)) || ((x <= startX) && (y <= startY)))
	{
		dx = -dx;
		dy = -dy;
		think = pause;
		
		if (think == 0)
		{
			think = 60;

			if ((x <= startX) && (y <= startY) && (waitAtStart))
			{
				active = false;
				think = 0;
			}

			if ((x >= endX) && (y >= endY) && (!waitAtStart))
			{
				active = false;
				think = 0;
			}
		}
	}
}

int Train::getPause()
{
	return pause;
}

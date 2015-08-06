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

Trap::Trap()
{
	currentAction = TRAP_WAIT2;
	active = true;
	thinktime = 60;
	x = y = dx = dy = 0;
}

void Trap::setName(const char *name)
{
	strlcpy(this->name, name, sizeof this->name);
}

void Trap::setTrapType(int type)
{
	this->type = type;
}

void Trap::setSpeed(int speed)
{
	this->speed = speed;
}

void Trap::setWaitTimes(int time1, int time2)
{
	thinktime = time1;
	this->waitTime[0] = time1;
	this->waitTime[1] = time2;
}

void Trap::setSprite(Sprite *sprite)
{
	this->sprite = sprite;

	this->width = sprite->image[0]->w;
	this->height = sprite->image[0]->h;
}

void Trap::setDestinations(int startX, int startY, int endX, int endY)
{
	this->startX = startX;
	this->startY = startY;
	this->endX = endX;
	this->endY = endY;
	
	x = endX;
	y = endY;
	
	if (type == TRAP_TYPE_SWING)
	{
		x = startX;
		y = startY;
		speed = Math::rrand(-2, 2);
		dx = 0.05;
		dy = Math::rrand(0, 45);
	}
}

void Trap::setDamage(int damage)
{
	this->damage = damage;
}

bool Trap::performSwingAction()
{
	if (!active)
	{
		x = startX;
		y = startY + endY;
		return false;
	}

	x = (0 * cos((dy * PI) / 180)) - (1 * sin((dy * PI) / 180));
	y = (1 * cos((dy * PI) / 180)) + (0 * sin((dy * PI) / 180));

	x *= endX;
	y *= endY;

	x += startX;
	y += startY;

	speed += dx;

	Math::limitFloat(&speed, -2.5, 2.5);

	dy += speed;

	if (dy <= -10)
	{
		dx = 0.05;
	}
	else if (dy >= 10)
	{
		dx = -0.05;
	}

	if (((int)dy >= -2) && ((int)dy <= 2))
		return true;

	return false;
}

bool Trap::performSpikeAction()
{
	if (currentAction == TRAP_FIRSTACTION)
	{
		if (!active)
			return false;

		Math::limitFloat(&(x -= speed), startX, endX);
		Math::limitFloat(&(y -= speed), startY, endY);

		if ((x == startX) && (y == startY))
		{
			Math::wrapChar(&(++currentAction), 0, 3);
			thinktime = waitTime[0];
		}
	}

	if (currentAction == TRAP_SECONDACTION)
	{
		Math::limitFloat(&(++x), startX, endX);
		Math::limitFloat(&(++y), startY, endY);

		if ((x == endX) && (y == endY))
		{
			Math::wrapChar(&(++currentAction), 0, 3);
			thinktime = waitTime[1];
		}
	}
	
	return false;
}

bool Trap::performBarrierAction()
{
	thinktime--;

	if (thinktime == 0)
	{
		currentAction = TRAP_WAIT2;
		thinktime = waitTime[1];
		if (thinktime == 0)
		{
			currentAction = TRAP_FIRSTACTION;
			thinktime = waitTime[0];
		}
	}

	if (thinktime >= waitTime[0] - 3)
		return true;

	return false;
}

bool Trap::performFlameAction()
{
	thinktime--;

	if (thinktime == 0)
	{
		currentAction = TRAP_WAIT2;
		thinktime = waitTime[1];
		if (thinktime == 0)
		{
			currentAction = TRAP_FIRSTACTION;
			thinktime = waitTime[0];
		}
	}

	if (thinktime >= waitTime[0] - 3)
		return true;

	return false;
}

bool Trap::think()
{
	if (type == TRAP_TYPE_MINE)
	{
		dy += 0.5;
		y += dy;
		return false;
	}

	if ((currentAction == TRAP_WAIT1) || (currentAction == TRAP_WAIT2))
	{
		thinktime--;

		if (thinktime > 0)
			return false;

		Math::wrapChar(&(++currentAction), 0, 3);
		thinktime = 60;
		
		if ((type == TRAP_TYPE_BARRIER) || (type == TRAP_TYPE_FLAME))
			thinktime = waitTime[0];
	}
	else
	{
		switch (type)
		{
			case TRAP_TYPE_SPIKE:
				return performSpikeAction();
				break;
			case TRAP_TYPE_SWING:
				return performSwingAction();
				break;
			case TRAP_TYPE_BARRIER:
				return performBarrierAction();
				break;
			case TRAP_TYPE_FLAME:
				return performBarrierAction();
				break;
		}
	}
	
	return false;
}

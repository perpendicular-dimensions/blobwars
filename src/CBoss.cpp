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

Boss::Boss()
{
	active = true;
		   
	thinktime = 0;
	actiontime = 0;
	custom = 0;
	
	maxHealth = 0;
	
	shieldFrequency = 999;
	
	react = NULL;
	think = NULL;
	die = NULL;
	
	Math::addBit(&flags, ENT_BOSS);
}

Boss::~Boss()
{
}

void Boss::setThinkTime(int time)
{
	this->thinktime = time;
}
void Boss::setActionFinished(int time)
{
	this->actiontime = time;
}

bool Boss::readyForThink()
{
	Math::limitInt(&(--thinktime), 0, 99999);
	
	return thinktime == 0;
}

bool Boss::actionFinished()
{
	Math::limitInt(&(--actiontime), 0, 99999);
	
	return actiontime == 0;
}

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

Weapon::Weapon()
{
	name[0] = 0;
	id = 0;
	damage = 0;
	reload = 0;
	dx = dy = 0;
	flags = 0;
	fireSound = 1;
	health = 0;
}

void Weapon::setName(const char *name)
{
	strlcpy(this->name, name, sizeof this->name);
}

int Weapon::getSpeed(int face) const
{
	return (dx - ((dx * 2) * face));
}

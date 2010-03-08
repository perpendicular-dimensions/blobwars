/*
Copyright (C) 2004 Parallel Realities

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

Switch::Switch()
{
	strcpy(this->name, "");
	strcpy(this->linkName, "");
	strcpy(this->requiredObjectName, "");
	strcpy(this->activateMessage, "");

	type = 0;
	x = y = 0;
	activated = false;
	health = 0;

	next = NULL;
}

void Switch::set(const char *name, const char *linkName, const char *requiredObjectName, const char *activateMessage, int type, int x, int y, bool activated)
{
	strcpy(this->name, name);
	strcpy(this->linkName, linkName);
	strcpy(this->requiredObjectName, requiredObjectName);
	strcpy(this->activateMessage, activateMessage);
	this->type = type;
	this->x = x;
	this->y = y;
	this->activated = activated;

	health = 0;

	next = NULL;
}

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

Switch::Switch(const std::string &name, const std::string &linkName, const std::string &requiredObjectName, const std::string &activateMessage, int type, int x, int y, bool activated)
{
	this->name = name;
	this->linkName = linkName;
	this->requiredObjectName = requiredObjectName;
	this->activateMessage = activateMessage;
	this->type = type;
	this->x = x;
	this->y = y;
	this->activated = activated;
}

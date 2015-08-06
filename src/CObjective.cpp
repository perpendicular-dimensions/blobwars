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

Objective::Objective()
{
	this->description[0] = 0;
	this->target[0] = 0;
	this->currentValue = 0;
	this->targetValue = 0;
	this->required = false;
	this->completed = false;

	next = NULL;
}

Objective::Objective(const char *description, const char *target, int targetValue, bool required)
{
	strlcpy(this->description, description, sizeof this->description);
	strlcpy(this->target, target, sizeof this->target);
	this->targetValue = targetValue;
	this->required = required;

	this->currentValue = 0;

	this->completed = false;

	next = NULL;
}

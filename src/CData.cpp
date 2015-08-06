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

Data::Data()
{
	key[0] = 0;
	value[0] = 0;
	current = 0;
	target = 1;
}

void Data::set(const char *key, const char *value, int current, int target)
{
	if ((strlen(key) > 100) || (strlen(value) > 100))
	{
		debug(("Key-Value Pair too Big!!! (%s:%s)\n", key, value));
		return;
	}

	strlcpy(this->key, key, sizeof this->key);
	strlcpy(this->value, value, sizeof this->value);
	this->current = current;
	this->target = target;
}

bool Data::isComplete()
{
	return (current == target);
}

void Data::getCurrentTarget(int *current, int *target)
{
	*current = this->current;
	*target = this->target;
}

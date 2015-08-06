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

Widget::Widget()
{
	name[0] = 0;
	groupName[0] = 0;
	label[0] = 0;
	options[0] = 0;
	x = y = type = min = max = 0;
	enabled = visible = true;
	changed = false;

	value = NULL;
	next = previous = NULL;
	image = NULL;
}

void Widget::setProperties(const char *name, const char *groupName, const char *label, const char *options, int x, int y, int min, int max)
{
	if ((strlen(name) > 50) || (strlen(groupName) > 50) || (strlen(label) > 80) || (strlen(options) > 100))
	{
		debug(("WIDGET NAME OVERFLOW!\n"));
		exit(1);
	}

	strlcpy(this->name, name, sizeof this->name);
	strlcpy(this->groupName, groupName, sizeof this->groupName);
	strlcpy(this->label, label, sizeof this->label);
	strlcpy(this->options, options, sizeof this->options);
	this->x = x;
	this->y = y;
	this->min = min;
	this->max = max;
}

void Widget::setValue(int *value)
{
	this->value = value;
}

void Widget::redraw()
{
	if (image != NULL)
		SDL_FreeSurface(image);

	image = NULL;
}

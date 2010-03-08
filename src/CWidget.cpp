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

Widget::Widget()
{
	strcpy(name, "");
	strcpy(groupName, "");
	strcpy(label, "");
	strcpy(options, "");
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

	strcpy(this->name, name);
	strcpy(this->groupName, groupName);
	strcpy(this->label, label);
	strcpy(this->options, options);
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

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

HubLevel::HubLevel()
{
	stageName[0] = 0;
	filename[0] = 0;
	x = y = 0;

	levelNameImage = NULL;
	target = NULL;
	next = NULL;
}

void HubLevel::set(const char *stageName, const char *filename, int x, int y)
{
	strlcpy(this->stageName, stageName, sizeof this->stageName);
	strlcpy(this->filename, filename, sizeof this->filename);
	this->x = x;
	this->y = y;
}

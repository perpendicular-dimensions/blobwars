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

FileData::FileData()
{
	filename[0] = 0;
	fSize = 0;
	cSize = 0;
	location = 0;
}

void FileData::set(const char *filename, Uint32 fSize, Uint32 cSize, Uint32 location)
{
	if (strlen(filename) > 59)
	{
		printf("WARNING: FileData::set - FILENAME EXCEEDS 60 CHARACTERS!!!\n");
		return;
	}
	
	strlcpy(this->filename, filename, sizeof this->filename);
	
	this->fSize = fSize;
	this->cSize = cSize;
	this->location = location;
}

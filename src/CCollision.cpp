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

bool Collision::collision(float x0, float y0, int w0, int h0, float x2, float y2, int w1, int h1)
{
	float x1 = x0 + w0;
	float y1 = y0 + h0;

	float x3 = x2 + w1;
	float y3 = y2 + h1;

	return !(x1<x2 || x3<x0 || y1<y2 || y3<y0);
}

bool Collision::collision(Entity *ent1, Entity *ent2)
{
	if ((ent1->flags & ENT_NOCOLLISIONS) || (ent2->flags & ENT_NOCOLLISIONS))
		return false;

	return collision(ent1->x, ent1->y, ent1->width, ent1->height, ent2->x, ent2->y, ent2->width, ent2->height);
}

bool Collision::collision(Entity *ent, Switch *swt)
{
	return collision(ent->x, ent->y, ent->width, ent->height, swt->x, swt->y, 64, 16);
}

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

extern void addBullet(Entity *owner, float dx, float dy);
extern void addEnemy(const char *name, int x, int y, int flags);
extern Entity *getDefinedEnemy(const char *name);
extern void addTeleportParticles(float x, float y, int amount, int soundToPlay);
extern void addExplosion(float x, float y, int radius, Entity *owner);
extern void dropHelperItems(int x, int y);

extern Audio audio;
extern Engine engine;
extern Game game;
extern Graphics graphics;
extern Map map;

extern Entity defEnemy[MAX_ENEMIES];
extern Entity player;
extern Weapon weapon[MAX_WEAPONS];

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

extern void checkObjectives(const char *name, bool alwaysInform);
extern void throwAndDamageEntity(Entity *ent, int damage, int minDX, int maxDX, int DY);
extern void addSmokeAndFire(Entity *ent, float dx, float dy, int amount);
extern void addBlood(Entity *ent, float dx, float dy, int amount);
extern void gibPlayer();
extern void addPlayerScore(int score);

extern Audio audio;
extern Engine engine;
extern Game game;
extern Graphics graphics;
extern Map map;

extern Entity player;

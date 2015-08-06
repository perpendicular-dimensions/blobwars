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

extern void throwAndDamageEntity(Entity *ent, int damage, int minDX, int maxDX, int DY);
extern void moveEntity(Entity *ent);
extern void addBullet(Entity *owner, float dx, float dy);
extern void addEffect(float x, float y, float dx, float dy, int flags);
extern void addBlood(Entity *ent, float dx, float dy, int amount);
extern void addTeleportParticles(float x, float y, int amount, int soundToPlay);
extern void addFireTrailParticle(float x, float y);
extern void addBubble(float x, float y);
extern void addColoredEffect(float x, float y, float dx, float dy, int color, int flags);

extern Audio audio;
extern Config config;
extern Engine engine;
extern Game game;
extern Graphics graphics;
extern Map map;
extern MedalServer medalServer;

extern Entity player;
extern Weapon weapon[MAX_WEAPONS];

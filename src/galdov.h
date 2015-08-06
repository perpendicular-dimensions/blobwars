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

extern void addTeleportParticles(float x, float y, int amount, int soundToPlay);
extern void addBlood(Entity *ent, float dx, float dy, int amount);
extern void checkObjectives(const char *name, bool alwaysInform);
extern void addColorParticles(float x, float y, int amount, int color);
extern void addBullet(Entity *owner, float dx, float dy);
extern void addExplosion(float x, float y, int radius, Entity *owner);
extern void addSmokeAndFire(Entity *ent, float dx, float dy, int amount);
extern void throwAndDamageEntity(Entity *ent, int damage, int minDX, int maxDX, int DY);
extern bool hasClearShot(Entity *enemy);
extern Weapon *getRandomAimedWeapon();
extern Weapon *getRandomStraightWeapon();
extern Weapon *getRandomGaldovWeapon();

extern void addItem(int itemType, const char *name, int x, int y, const char *spriteName, int health, int value, int flags, bool randomMovement);

extern Audio audio;
extern Engine engine;
extern Graphics graphics;
extern Game game;
extern Map map;

extern Entity player;
extern Entity defItem[MAX_ITEMS];
extern Weapon weapon[MAX_WEAPONS];

// this is used exclusively by the bosses
extern Boss *self;

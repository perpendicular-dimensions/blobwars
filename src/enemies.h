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

extern bool checkBrickContact(Entity *object);
extern void moveEntity(Entity *ent);
extern void addColorParticles(float x, float y, int amount, int color);
extern void addBullet(Entity *owner, float dx, float dy);
extern void addItem(int itemType, char *name, int x, int y, int imageIndex, int health, int value, int flags, bool randomMovement);
extern void dropRandomItems(int x, int y);
extern void addEffect(float x, float y, float dx, float dy, int flags);
extern void addColoredEffect(float x, float y, float dx, float dy, int color, int flags);
extern void addExplosion(float x, float y, int radius, Entity *owner);
extern void addBlood(Entity *ent, float dx, float dy, int amount);
extern void addSmokeAndFire(Entity *ent, float dx, float dy, int amount);
extern void checkObjectives(const char *name, bool alwaysInform);
extern Weapon *getWeaponByName(const char *name);
extern void addFireTrailParticle(float x, float y);
extern void addPlayerScore(int score);
extern void presentPlayerMedal(const char *tname);

extern void doGaldovAI(Entity *galdov);
extern void addTeleportParticles(float x, float y, int amount, int soundToPlay);

extern Audio audio;
extern Engine engine;
extern Game game;
extern Graphics graphics;
extern Map map;

extern Entity defEnemy[MAX_ENEMIES];
extern Entity player;
extern Weapon weapon[MAX_WEAPONS];

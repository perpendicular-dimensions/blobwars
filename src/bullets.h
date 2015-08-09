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

extern bool checkTrainContact(Entity *ent, int dir);
extern void getTrainMotion(Entity *ent, int &dx, int &dy);
extern bool checkObstacleContact(Entity *ent, int dir);
extern void enemyBulletCollisions(Entity *bullet);
extern void checkPlayerBulletCollisions(Entity *bullet);
extern void checkBossBulletCollisions(Entity *bullet);
extern void addFireTrailParticle(float x, float y);
extern void addColorParticles(float x, float y, int amount, int color);
extern void addExplosion(float x, float y, int radius, Entity *owner);
extern void addFireParticles(float x, float y, int amount);
extern void checkSwitchContact(Entity *ent);
extern void throwBrickParticles(float x, float y);
extern void throwStalagParticles(float x, float y);
extern void presentPlayerMedal(const char *tname);

extern Audio audio;
extern Engine engine;
extern Game game;
extern Graphics graphics;
extern Map map;

extern Entity player;
extern Weapon weapon[MAX_WEAPONS];

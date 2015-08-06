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

extern void addObstacle(const char *name, int x, int y, const char *spriteName);
extern void addItem(int itemType, const char *name, int x, int y, const char *shapeName, int health, int value, int flags, bool randomMovement);
extern void addMIA(const char *name, int x, int y, int type);
extern void addTeleporter(const char *name, int x, int y, int destX, int destY, bool active);
extern void addLineDef(const char *name, const char *linkName, const char *message, int x, int y, int width, int height, bool active);
extern void addTrap(const char *name, int trapType, int damage, int speed, int startX, int startY, int endX, int endY, int wait1, int wait2, const char *sprite, bool active);
extern void loadSprite(const char *token);
extern void loadEnemy(const char *token);
extern Entity *getDefinedEnemy(const char *name);
extern void addEnemy(const char *name, int x, int y, int flags);
extern bool perfectlyCompleted();

extern Audio audio;
extern Engine engine;
extern Game game;
extern GameData gameData;
extern Graphics graphics;
extern Map map;
extern ReplayData replayData;

extern Entity player;

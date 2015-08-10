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

extern void initSystem();
extern void cleanup();
extern void loadResources();

extern void doItems();
extern void doPlayer();
extern void doTrains();
extern void doSwitches();
extern void doBullets();
extern void doEffects();
extern void doParticles();
extern void doMIAs();
extern void doObstacles();
extern void doTeleporters();
extern void doLineDefs();
extern void doTraps();

extern void addEnemy(const char *name, int x, int y, int flags);
extern void addItem(int itemType, const char *name, int x, int y, const char *spriteName, int health, int value, int flags, bool randomMovement);

class String {

	public:

		char string[1024];
		String *next;

		String()
		{
			strcpy(string, "");
			next = NULL;
		}

};

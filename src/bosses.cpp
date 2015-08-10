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

#include "bosses.h"

// this is used exclusively by the bosses
Boss *self;

void tankBossLevel()
{
	if (player.health < 1)
		return;
	
	if (map.bossNextThink > 0)
	{
		map.bossNextThink--;
		return;
	}
	
	int r = 0;
	
	switch (Math::prand() % 4)
	{
		case 0:
			r = ITEM_MACHINEGUN;
			break;
		case 1:
			r = ITEM_CHERRY;
			break;
		case 2:
			r = ITEM_DOUBLECHERRY;
			break;
		case 3:
			r = ITEM_TRIPLECHERRY;
			break;
	}
	
	int x = 0;
	int y = 8448;
	
	switch (Math::prand() % 4)
	{
		case 0:
			x = 905;
			break;
		case 1:
			x = 840;
			break;
		case 2:
			x = 495;
			break;
		case 3:
			x = 425;
			break;
	}
	
	addItem(defItem[r].id, defItem[r].name, x, y, defItem[r].sprite[0]->name, 240, defItem[r].value, ENT_DYING, true);
	
	Entity *enemy = (Entity*)map.enemyList.getHead();
	
	bool blob1 = false;
	bool blob2 = false;
	
	if (game.skill >= 2)
	{
		if ((Math::prand() % 3) == 0)
		{
			addEnemy("Red Blob", Math::rrand(525, 800), 8100, 0);
		}
		
		if (game.skill == 3)
		{
			addEnemy("Red Blob", Math::rrand(525, 800), 8100, 0);
		} 
	}

	while (enemy->next != NULL)
	{
		enemy = (Entity*)enemy->next;
		
		if (strcmp(enemy->name, "Red Blob 1") == 0)
			blob1 = true;
		
		if (strcmp(enemy->name, "Red Blob 2") == 0)
			blob2 = true;	
	}
	
	if (!blob1)
	{
		addEnemy("Red Blob 1", 1100, 8352, 0);
		debug(("Added Red Blob #1\n"));
	}
		
	if (!blob2)
	{
		addEnemy("Red Blob 2", 120, 8352, 0);
		debug(("Added Red Blob #2\n"));
	}
	
	map.bossNextThink = Math::rrand(10, 15) * 60;
}

void aquaBossLevel()
{
	if (player.health < 1)
		return;
	
	if (map.bossNextThink > 0)
	{
		map.bossNextThink--;
		return;
	}
	
	switch (Math::prand() % 4)
	{
		case 0:
			addEnemy("Aqua Blob", 1600, 530, 0);
			break;
		case 1:
			addEnemy("Aqua Blob", 1600, 690, 0);
			break;
		case 2:
			addEnemy("Aqua Blob", 1600, 820, 0);
			break;
		case 3:
			addEnemy("Aqua Blob", 1600, 940, 0);
			break;
	}
	
	map.bossNextThink = Math::rrand(5, 10) * 60;
	if (game.skill >= 2)
		map.bossNextThink = Math::rrand(2, 6) * 60;
}

void droidBossLevel()
{
	if (player.health < 1)
		return;
	
	if (map.bossNextThink > 0)
	{
		map.bossNextThink--;
		return;
	}
	
	Entity *item = (Entity*)map.itemList.getHead();
	
	bool addPistol, addMachineGun, addLaser, addSpread;
	addPistol = addMachineGun = addLaser = addSpread = true;

	while (item->next != NULL)
	{
		item = (Entity*)item->next;
		
		if (item->flags & ENT_DYING)
			continue;
		
		switch (item->id)
		{
			case ITEM_PISTOL:
				addPistol = false;
				break;
			case ITEM_MACHINEGUN:
				addMachineGun = false;
				break;
			case ITEM_LASER:
				addLaser = false;
				break;
			case ITEM_SPREAD:
				addSpread = false;
				break;
		}
	}
	
	if (addPistol)
		addItem(defItem[ITEM_PISTOL].id, defItem[ITEM_PISTOL].name, 2432, 4576, defItem[ITEM_PISTOL].sprite[0]->name, 240, defItem[ITEM_PISTOL].value, 0, false);
		
	if (addMachineGun)
		addItem(defItem[ITEM_MACHINEGUN].id, defItem[ITEM_MACHINEGUN].name, 2432, 4224, defItem[ITEM_MACHINEGUN].sprite[0]->name, 240, defItem[ITEM_MACHINEGUN].value, 0, false);
	
	if (addLaser)
		addItem(defItem[ITEM_LASER].id, defItem[ITEM_LASER].name, 3552, 4384, defItem[ITEM_LASER].sprite[0]->name, 240, defItem[ITEM_LASER].value, 0, false);
	
	if (addSpread)
		addItem(defItem[ITEM_SPREAD].id, defItem[ITEM_SPREAD].name, 3552, 4800, defItem[ITEM_SPREAD].sprite[0]->name, 240, defItem[ITEM_SPREAD].value, 0, false);
	
	int r = Math::prand() % 6;
	if (game.skill == 2)
		r = Math::prand() % 4;
	
	int x, y;
	
	switch (r)
	{
		case 0:
			addEnemy("Eye Droid V4.0", 3648, 4704, 0);
			break;
		case 1:
			addEnemy("Eye Droid V4.0", 3648, 4224, 0);
			break;
		case 2:
			addEnemy("Eye Droid V4.0", 2336, 4128, 0);
			break;
		case 3:
			addEnemy("Eye Droid V4.0", 2336, 4736, 0);
			break;
		default:
			break;
	}
	
	for (int i = 0 ; i < 2 ; i++)
	{
		r = Math::rrand(ITEM_CHERRY, ITEM_DOUBLECHERRY);
		x = (int)player.x >> BRICKSHIFT;
		y = (int)player.y >> BRICKSHIFT;
		x += Math::rrand(-10, 10);
		y += Math::rrand(-10, 10);
		if (map.data[x][y] == MAP_AIR)
		{
			x = x << BRICKSHIFT;
			y = y << BRICKSHIFT;
			addItem(defItem[r].id, defItem[r].name, x, y, defItem[r].sprite[0]->name, 240, defItem[r].value, ENT_DYING, true);
		}
	}
	
	map.bossNextThink = Math::rrand(10, 15) * 60;
}

void galdovLevel()
{
}

void galdovFinalLevel()
{
	if (player.health < 1)
		return;
	
	if (map.bossNextThink > 0)
	{
		map.bossNextThink--;
		return;
	}
	
	map.bossNextThink = Math::rrand(10, 15) * 60;
	
	if ((map.boss[0]->health > 30 * game.skill) || (map.boss[0]->health <= 15 * game.skill))
	{
		return;
	}
	
	int bossIndex = -1;
	
	for (int i = 6 ; i < 10 ; i++)
	{
		if (map.boss[i] == NULL)
			continue;
		
		if (map.boss[i]->health <= -60)
		{
			bossIndex = i;
			break;
		}
	}
	
	if (bossIndex == -1)
		return;
	
	int x = Math::rrand(23, 62);
	int y = Math::rrand(42, 61);
	
	if (map.data[x][y] == MAP_AIR)
	{
		x = x << BRICKSHIFT;
		y = y << BRICKSHIFT;
		map.boss[bossIndex]->active = true;
		map.boss[bossIndex]->place(x, y);
		map.boss[bossIndex]->dx = 0;
		map.boss[bossIndex]->dy = 0;
		map.boss[bossIndex]->health = 3 * game.skill;
		map.boss[bossIndex]->maxHealth = 3 * game.skill;
		Math::addBit(&map.boss[bossIndex]->flags, ENT_FLIES);
		addTeleportParticles(map.boss[bossIndex]->x + (map.boss[bossIndex]->width / 2), map.boss[bossIndex]->y + (map.boss[bossIndex]->height / 2), 50, SND_TELEPORT2);
	}
}

void createBoss(const char *stageName)
{
	map.mainBossPart = NULL;
	
	if (!map.isBossMission)
	{
		return;
	}
	
	debug(("createBoss()\n"));
	
	if (strcmp(stageName, "BioMech Supply Depot") == 0)
	{
		tankBossMGInit();
		tankBossGLInit();
		map.bossNextThink = 0;
		map.doBossLevelAction = &tankBossLevel;
	}
	else if (strcmp(stageName, "BioMech Communications") == 0)
	{
		aquaBossInit();
		map.bossNextThink = 0;
		map.doBossLevelAction = &aquaBossLevel;
	}
	else if (strcmp(stageName, "BioMech Assimilator") == 0)
	{
		droidBossInit();
		map.bossNextThink = 0;
		map.doBossLevelAction = &droidBossLevel;
	}
	else if (strcmp(stageName, "BioMech HQ") == 0)
	{
		galdovInit();
		map.bossNextThink = 0;
		map.doBossLevelAction = &galdovLevel;
	}
	else if (strcmp(stageName, "Final Battle") == 0)
	{
		galdovFinalInit();
		map.bossNextThink = 0;
		map.doBossLevelAction = &galdovFinalLevel;
	}
	
	debug(("createBoss(): Done\n"));
}

void doBosses()
{
	int x, y, absX, absY;
	
	self = NULL;
	
	for (int i = 0 ; i < 10 ; i++)
	{
		if (map.boss[i] == NULL)
		{
			continue;
		}
		
		self = map.boss[i];
			
		moveEntity((Entity*)self);
		
		if (self->flags & ENT_TELEPORTING)
			continue;
		
		x = (int)(self->x - engine.playerPosX);
		y = (int)(self->y - engine.playerPosY);
		
		absX = abs(x);
		absY = abs(y);
		
		if ((absX < 700) && (absY < 500))
		{
			if (self->flags & ENT_FIRETRAIL)
			{
				addFireTrailParticle(self->x + (self->face * 16) + Math::rrand(-1, 1), self->y + Math::rrand(-1, 1));
			}
			graphics.blit(self->getFaceImage(), x, y, graphics.screen, false);
			self->animate();
		}
		
		if (!self->active)
		{
			continue;
		}
		
		if (self->health <= 0)
		{
			if (self->health > -100)
			{
				self->die();
			}
			else
			{
				checkObjectives(self->name, false);	

				if (!self->referenced)
				{
					debug(("Deleting unreferenced Boss...\n"));
					
					if(map.mainBossPart == map.boss[i])
						map.mainBossPart = NULL;
					delete map.boss[i];
					map.boss[i] = NULL;
				} else
					self->referenced = false;
			}
		}
		else if (self->actionFinished())
		{	
			if (self->readyForThink())
			{
				self->think();
			}
		}
	}
	
	map.doBossLevelAction();
}

bool isCorrectShieldFrequency(Entity *bullet)
{	
	if (bullet->id != self->shieldFrequency)
	{
		bullet->owner = self;
		bullet->dx = -bullet->dx;
		bullet->dx *= 0.25;
		bullet->dy = -2;
		
		bullet->x += bullet->dx;
		bullet->y += bullet->dy;
		
		bullet->health += 60;
		
		bullet->owner = self;
		
		Math::removeBit(&bullet->flags, ENT_WEIGHTLESS);
		Math::removeBit(&bullet->flags, ENT_BOUNCES);
		
		audio.playSound(SND_RICO1, CH_ANY, bullet->x);
		
		return false;
	}
	
	return true;
}

void checkBossBulletCollisions(Entity *bullet)
{
	if ((bullet->health < 1) || (player.health <= -60))
	{
		return;
	}

	if (game.missionOver > 0)
	{
		return;
	}
	
	self = NULL;

	if (bullet->owner == &player)
	{
		for (int i = 0 ; i < 10 ; i++)
		{
			if (map.boss[i] == NULL)
			{
				continue;
			}
			
			self = map.boss[i];
		
			if (Collision::collision((Entity*)self, bullet))
			{
				
				if (map.boss[i]->shieldFrequency != 999)
				{
					if (!isCorrectShieldFrequency(bullet))
					{
						return;
					}
				}
				
				if (!(self->flags & ENT_IMMUNE))
				{
					self->health -= bullet->damage;
					audio.playSound(SND_CLANG, CH_ANY, bullet->x);
					addColorParticles(bullet->x, bullet->y, Math::rrand(25, 75), -1);
					Math::removeBit(&bullet->flags, ENT_SPARKS);
					Math::removeBit(&bullet->flags, ENT_PUFFS);
				}
				
				if (self->react != NULL && self->health > 0)
				{
					self->react();
				}
	
				if (bullet->id != WP_LASER)
				{
					bullet->health = 0;
				}
			}
		}
	}
}

void doGaldovAI(Entity *galdov)
{
	map.fightingGaldov = true;

	if (SDL_GetTicks() < map.bossNextThink)
		return;

	map.bossNextThink = SDL_GetTicks() + Math::rrand(250, 1500);

	switch (Math::prand() % 4)
	{
		case 0:
			if ((Math::prand() % 5) == 0)
			{
				if (galdov->flags & ENT_FLIES)
				{
					Math::removeBit(&galdov->flags, ENT_FLIES);
					Math::removeBit(&galdov->flags, ENT_FIRETRAIL);
				}
				else
				{
					Math::addBit(&galdov->flags, ENT_FLIES);
					Math::addBit(&galdov->flags, ENT_FIRETRAIL);
				}
			}
			break;
		case 1:
			if (galdov->flags & ENT_JUMPS)
			{
				Math::removeBit(&galdov->flags, ENT_JUMPS);
			}
			else
			{
				Math::addBit(&galdov->flags, ENT_JUMPS);
			}
			break;
		case 2:
			galdov->currentWeapon = getRandomGaldovWeapon();
			break;
		case 3:
			if (galdov->flags & ENT_RAPIDFIRE)
			{
				Math::removeBit(&galdov->flags, ENT_RAPIDFIRE);
			}
			else
			{
				Math::addBit(&galdov->flags, ENT_RAPIDFIRE);
			}
			break;
	}
	
	if ((Math::prand() % 25) == 0)
	{
		switch (Math::prand() % 5)
		{
			case 0:
				engine.setInfoMessage("Galdov: Stupid creature!! Give up and join us!", 0, INFO_HINT);
				break;
			case 1:
				engine.setInfoMessage("Galdov: Why do you persist in fighting us?!", 0, INFO_HINT);
				break;
			case 2:
				engine.setInfoMessage("Galdov: And this is the best the Blob Army can offer?", 0, INFO_HINT);
				break;
			case 3:
				engine.setInfoMessage("Galdov: We WILL have the crystals! NOTHING will stop us!!", 0, INFO_HINT);
				break;
			case 4:
				engine.setInfoMessage("Galdov: You're mine now!!!", 0, INFO_HINT);
				break;
		}
	}
}

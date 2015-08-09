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

#include "finalBattle.h"

void galdovMiniAttack();
void galdovFinalFlee1();
void blackDroidAttack();

// ************************ Black Droids for Part 2 *************************************

void blackDroidFire()
{	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	if (!hasClearShot(self))
		return;
	
	self->setActionFinished(9);
	self->setThinkTime(2);
	self->think = &blackDroidFire;
	
	addBullet((Entity*)self, 0, 0);
	
	self->custom--;
	if (self->custom == 0)
	{
		self->think = &blackDroidAttack;
	}
}

void blackDroidDie()
{
	self->health--;
	Math::removeBit(&self->flags, ENT_FLIES);
	
	if (self->health < -10)
	{
		if ((self->health % 3) == 0)
		{
			addExplosion(self->x, self->y, 35, self);
			addSmokeAndFire(self, Math::rrand(-15, 15), Math::rrand(-15, 15), 2);
		}
	}
	
	if (self->health <= -60)
	{
		if ((Math::prand() % 2) == 0)
		{
			dropItems((int)self->x, (int)self->y);
			dropItems((int)self->x, (int)self->y);
		}
		else
		{
			addItem(100, "Red Keycard", (int)self->x, (int)self->y, "RedKeyCard", 240, 1, ENT_DYING, true);
		}
		self->active = false;
		self->place(9999, 9999);
	}
}

void blackDroidChasePlayer()
{
	self->think = &blackDroidAttack;
	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	float dx, dy;
	
	Math::calculateSlope(player.x, player.y, self->x, self->y, &dx, &dy);
	
	self->dx = (dx * 2);
	self->dy = (dy * 2);
	
	self->setThinkTime(10);
	self->setActionFinished(10);
}

void blackDroidMoveRandom()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->think = &blackDroidAttack;
	
	self->dx = Math::rrand(-200, 200);
	self->dy = Math::rrand(-200, 200);

	self->dx *= 0.01;
	self->dy *= 0.01;

	self->setThinkTime(2);
	self->setActionFinished(Math::rrand(60, 90));
}

void blackDroidAttack()
{
	self->think = blackDroidAttack;
	self->setThinkTime(2);
	self->setActionFinished(2);
	
	int r = Math::prand() % 15;
	
	if (r <= 2)
	{
		self->think = &blackDroidFire;
		self->setThinkTime(2);
		self->setActionFinished(2);
		self->custom = Math::rrand(2, 4) * game.skill;
	}
	else if (r < 5)
	{
		self->think = &blackDroidChasePlayer;
		self->setThinkTime(2);
		self->setActionFinished(2);
	}
	else
	{
		self->think = &blackDroidMoveRandom;
		self->setThinkTime(2);
		self->setActionFinished(2);
	}
}

void galdovInitBlackDroids()
{
	for (int i = 6 ; i < 10 ; i++)
	{
		if (map.boss[i] == NULL)
		{
			map.boss[i] = new Boss();
		}
		debug(("BlackDroid %d init\n", i));
		strlcpy(map.boss[i]->name, "BlackDrod", sizeof map.boss[i]->name);
		map.boss[i]->health = -90;
		map.boss[i]->maxHealth = -90;
		map.boss[i]->setSprites(graphics.getSprite("BlackDroidRight", true), graphics.getSprite("BlackDroidLeft", true), graphics.getSprite("BlackDroidDie", true));
		map.boss[i]->currentWeapon = &weapon[WP_PLASMARIFLE];
		map.boss[i]->face = 0;
		map.boss[i]->active = false;
		map.boss[i]->place(9999, 9999);
		map.boss[i]->immune = 0;
		map.boss[i]->setThinkTime(2);
		map.boss[i]->think = &blackDroidAttack;
		map.boss[i]->react = NULL;
		map.boss[i]->die = &blackDroidDie;
		map.boss[i]->setActionFinished(120);
		Math::addBit(&map.boss[i]->flags, ENT_FLIES);
		Math::addBit(&map.boss[i]->flags, ENT_AIMS);
		Math::removeBit(&map.boss[i]->flags, ENT_FIRETRAIL);
	}
}

// ************************ First part of final battle **********************************

void galdovFinalKeepAlive1()
{
	map.boss[0]->health = 30 * game.skill;
}

void galdovFinalPainThrow()
{
	self->dx = -3;
	self->dy = -12;
	
	self->think = &galdovFinalFlee1;
	self->setThinkTime(2);
	self->setActionFinished(140);
}

void galdovFinalDropCrystal()
{
	self->setThinkTime(2);
	self->setActionFinished(2);
	
	int x = (int)player.x >> BRICKSHIFT;
	int y = (int)player.y >> BRICKSHIFT;
	x += Math::rrand(-2, 2);
	y += Math::rrand(-2, 2);
	
	Math::limitInt(&x, 2, 62);
	Math::limitInt(&y, 2, 26);
	
	if (map.data[x][y] == MAP_AIR)
	{
		x = x << BRICKSHIFT;
		y = y << BRICKSHIFT;
		self->x = x;
		self->y = y;
		addTeleportParticles(self->x + 10, self->y + 10, 50, SND_TELEPORT2);
		addItem(101, "Reality Crystal", (int)self->x, (int)self->y, "RealityCrystal", 100, 1, 0, true);
		self->think = &galdovFinalPainThrow;
		self->react = &galdovFinalKeepAlive1;
		Math::removeBit(&self->flags, ENT_FLIES);
		Math::removeBit(&self->flags, ENT_FIRETRAIL);
		map.setMainBossPart(NULL);
		map.boss[0]->health = 30 * game.skill;
		map.boss[0]->setSprites(graphics.getSprite("GaldovPain", true), graphics.getSprite("GaldovPain", true), graphics.getSprite("GaldovPain", true));
		engine.setInfoMessage("Galdov has dropped the crystal! Quick! Get it!!", 99, INFO_HINT);
		audio.playSound(SND_BOSSCUSTOM2, CH_AMBIANCE, x);
	}
}

void galdovFinalDie()
{
	if (game.missionOverReason == MIS_INPROGRESS)
	{
		game.missionOverReason = MIS_GAMECOMPLETE;
		audio.stopMusic();
		audio.playSound(SND_BOSSCUSTOM3, CH_AMBIANCE, self->x);
		player.health = 10;
		self->dx = 5;
		self->dy = -6;
		self->react = NULL;
		Math::removeBit(&self->flags, ENT_FLIES);
		Math::removeBit(&self->flags, ENT_FIRETRAIL);
		presentPlayerMedal("Final_Battle");
	}
	
	self->setActionFinished(1);
	self->setThinkTime(1);
	
	self->health--;
	
	if (self->health > -35)
		return;
	
	if (self->health == -99)
	{
		checkObjectives(self->name, false);
	}
	
	Math::wrapInt(&self->health, -99, -41);
	
	if ((self->health % 3) == 0)
	{
		self->setActionFinished(5);
		self->setThinkTime(2);
		
		self->dx = Math::rrand(-5, 5);
		self->dy = Math::rrand(-5, 5);
		
		addExplosion(self->x, self->y, 50, &player);
		addSmokeAndFire(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 2);
	}
}

void galdovMiniFire()
{	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	if (!hasClearShot(self))
		return;
	
	self->setActionFinished(9);
	self->setThinkTime(2);
	self->think = &galdovMiniFire;
	
	addBullet((Entity*)self, 0, 0);
	
	self->custom--;
	if (self->custom == 0)
	{
		self->think = &galdovMiniAttack;
	}
}

void galdovFinalTeleport()
{
	self->setThinkTime(2);
	self->setActionFinished(2);
	
	int x = (int)player.x >> BRICKSHIFT;
	int y = (int)player.y >> BRICKSHIFT;
	x += Math::rrand(-10, 10);
	y += Math::rrand(-10, 10);
	
	Math::limitInt(&x, 2, 62);
	Math::limitInt(&y, 2, 26);
	
	if (self == map.boss[0])
	{
		if (self->health < 45 * game.skill)
		{
			Math::limitInt(&x, 23, 62);
			Math::limitInt(&y, 42, 61);
			self->currentWeapon = getRandomGaldovWeapon();
		}
	}
	
	if (map.data[x][y] == MAP_AIR)
	{
		x = x << BRICKSHIFT;
		y = y << BRICKSHIFT;
		self->x = x;
		self->y = y;
		addTeleportParticles(self->x + 10, self->y + 10, 50, SND_TELEPORT2);
		self->think = &galdovMiniAttack;
	}
}

void galdovMiniChasePlayer()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	float dx, dy;
	
	Math::calculateSlope(player.x, player.y, self->x, self->y, &dx, &dy);
	
	self->dx = (dx * 2);
	self->dy = (dy * 2);
	
	self->setThinkTime(10);
	self->setActionFinished(10);
	self->think = &galdovMiniAttack;
}

void galdovMiniMoveRandom()
{
	self->think = &galdovMiniAttack;
	
	if (self->flags & ENT_FLIES)
	{
		self->dx = Math::rrand(-200, 200);
		self->dy = Math::rrand(-200, 200);
	
		self->dx *= 0.01;
		self->dy *= 0.01;
	
		self->setThinkTime(2);
		self->setActionFinished(Math::rrand(60, 90));
	}
	else
	{
		self->dy = Math::rrand(-12, 0);
	
		(player.x < self->x) ? self->dx = -3 : self->dx = 3;
		
		self->think = &galdovMiniAttack;
		self->setActionFinished(45);
	}
}

void galdovMiniAttack()
{
	if (player.health < 1)
	{
		self->dx = self->dy = 0;
		return;
	}
	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->think = &galdovMiniAttack;
	self->setThinkTime(2);
	self->setActionFinished(2);
	
	int r = Math::prand() % 100;
	
	if (r < 10)
	{
		self->think = &galdovMiniFire;
		self->custom = Math::rrand(1, 8);
	}
	else if (r < 15)
	{
		addTeleportParticles(self->x + 10, self->y + 10, 50, SND_TELEPORT2);
		self->x = -9999;
		self->y = -9999;
		self->setActionFinished(120);
		self->setThinkTime(2);
		self->think = &galdovFinalTeleport;
	}
	else if (r < 80)
	{
		self->think = &galdovMiniChasePlayer;
	}
	else if (r < 90)
	{
		self->think = &galdovMiniMoveRandom;
	}
	else
	{
		if ((self->flags & ENT_FLIES) && (Math::prand() % 5) == 0)
		{
			Math::removeBit(&self->flags, ENT_FLIES);
			Math::removeBit(&self->flags, ENT_FIRETRAIL);
		}
		else
		{
			Math::addBit(&self->flags, ENT_FLIES);
			Math::addBit(&self->flags, ENT_FIRETRAIL);
		}
	}
}

void galdovMiniDie()
{
	addExplosion(self->x, self->y, 5, self);
	dropItems((int)self->x, (int)self->y);
	self->setThinkTime(2);
	self->setActionFinished(2);
	self->active = false;
	self->place(9999, 9999);
	
	// Don't kill it properly(!!)
	self->health = -1;
	
	bool allDead = true;
	
	for (int i = 1 ; i < 10 ; i++)
	{
		if (map.boss[i]->health > 0)
		{
			allDead = false;
		}
	}
	
	if (allDead)
	{
		map.boss[0]->think = &galdovFinalDropCrystal;
		map.boss[0]->setThinkTime(2);
		map.boss[0]->setActionFinished(2);
	}
}

void galdovRejoin()
{
	debug(("galdovRejoin\n"));
	
	for (int i = 1 ; i < 10 ; i++)
	{
		if (map.boss[i] != NULL)
		{
			addTeleportParticles(map.boss[i]->x + 4, map.boss[i]->y + 4, 75, -1);
			
			if ((Math::prand() % (2 + (game.skill))) == 0)
			{
				dropItems((int)map.boss[i]->x, (int)map.boss[i]->y);
			}
			
			map.boss[i]->place(9999, 9999);
			map.boss[i]->active = false;
		}
	}
	
	self->setThinkTime(2);
	self->setActionFinished(120);
	self->think = &galdovFinalTeleport;
	self->currentWeapon = getRandomGaldovWeapon();
	
	audio.playSound(SND_BOSSCUSTOM1, CH_AMBIANCE, self->x);
	
	map.setMainBossPart(map.boss[0]);
	
	debug(("galdovRejoin: Done\n"));
}

void galdovFinalMiniReact()
{
	if (self->health > 0)
	{
		map.setMainBossPart(self);
	}
	else
	{
		map.setMainBossPart(NULL);
	}
}

void galdovFinalSplit()
{
	debug(("galdovFinalSplit\n"));
	
	for (int i = 1 ; i < 10 ; i++)
	{
		if (map.boss[i] == NULL)
		{
			map.boss[i] = new Boss();
			strlcpy(map.boss[i]->name, "MiniGaldov", sizeof map.boss[i]->name);
			map.boss[i]->health = 10 * game.skill;
			map.boss[i]->maxHealth = 10 * game.skill;
		}
		map.boss[i]->setSprites(graphics.getSprite("GaldovMiniRight", true), graphics.getSprite("GaldovMiniLeft", true), graphics.getSprite("GaldovMiniLeft", true));
		map.boss[i]->currentWeapon = getRandomAimedWeapon();
		map.boss[i]->face = 0;
		map.boss[i]->active = true;
		
		if (map.boss[i]->health > 0)
		{
			map.boss[i]->dx = Math::rrand(-2, 2);
			map.boss[i]->dy = Math::rrand(-2, 2);
			map.boss[i]->x = map.boss[0]->x;
			map.boss[i]->y = map.boss[0]->y;
		}
		else
		{
			map.boss[i]->active = false;
		}
		
		map.boss[i]->immune = 0;
		map.boss[i]->setThinkTime(2);
		map.boss[i]->setActionFinished(120);
		map.boss[i]->think = &galdovMiniAttack;
		map.boss[i]->react = &galdovFinalMiniReact;
		map.boss[i]->die = &galdovMiniDie;
		Math::addBit(&map.boss[i]->flags, ENT_AIMS);
		Math::addBit(&map.boss[i]->flags, ENT_FLIES);
		Math::addBit(&map.boss[i]->flags, ENT_FIRETRAIL);
		
		addTeleportParticles(map.boss[i]->x + 10 + map.boss[i]->dx, map.boss[i]->y + 10 + map.boss[i]->dy, 75, -1);
	}
	
	audio.playSound(SND_BOSSCUSTOM1, CH_AMBIANCE, map.boss[0]->x);

	map.boss[0]->place(9999, 9999);
	map.boss[0]->setThinkTime(2);
	map.boss[0]->setActionFinished(Math::rrand(10, 15) * 60);
	map.boss[0]->think = &galdovRejoin;
	map.boss[0]->active = true;
	
	map.setMainBossPart(NULL);
	
	debug(("galdovFinalSplit: Done\n"));
}

void galdovFinalReact()
{
	map.boss[0]->health = 45 * game.skill;
	
	if ((Math::prand() % 5) == 0)
	{
		map.boss[0]->setThinkTime(2);
		map.boss[0]->setActionFinished(2);
		map.boss[0]->think = &galdovFinalSplit;
	}
}

void galdovFinalInit()
{
	debug(("galdovFinalInit\n"));
	
	map.boss[0] = new Boss();
	strlcpy(map.boss[0]->name, "Galdov", sizeof map.boss[0]->name);
	map.boss[0]->health = 45 * game.skill;
	map.boss[0]->maxHealth = 45 * game.skill;
	map.boss[0]->setSprites(graphics.getSprite("GaldovRight", true), graphics.getSprite("GaldovLeft", true), graphics.getSprite("GaldovDie", true));
	map.boss[0]->currentWeapon = &weapon[WP_AIMEDPISTOL];
	map.boss[0]->face = 0;
	map.boss[0]->active = true;
	map.boss[0]->x = 480;
	map.boss[0]->y = 576;
	map.boss[0]->immune = 0;
	map.boss[0]->setThinkTime(2);
	map.boss[0]->setActionFinished(2);
	map.boss[0]->think = &galdovMiniAttack;
	//map.boss[0]->think = &galdovFinalDropCrystal;
	map.boss[0]->react = &galdovFinalReact;
	map.boss[0]->die = &galdovFinalDie;
	Math::addBit(&map.boss[0]->flags, ENT_AIMS);
	
	audio.loadSound(SND_BOSSCUSTOM1, "sound/galdovSplit");
	audio.loadSound(SND_BOSSCUSTOM2, "sound/galdovPain");
	audio.loadSound(SND_BOSSCUSTOM3, "sound/galdovDie");
	
	map.setMainBossPart(map.boss[0]);
	
	debug(("galdovFinalInit: Done\n"));
}

// ******************** Second part of Final Battle *********************************

void galdovFinalReact2()
{
	if (self->health < 1)
		return;
	
	if ((Math::prand() % 12) == 0)
	{
		addTeleportParticles(self->x + (self->width / 2), self->y + (self->height / 2), 50, SND_TELEPORT3);
		map.boss[0]->setThinkTime(2);
		map.boss[0]->setActionFinished(2);
		map.boss[0]->think = &galdovFinalTeleport;
	}
}

void galdovFinalStandUp()
{
	map.boss[0]->setSprites(graphics.getSprite("GaldovShieldRight", true), graphics.getSprite("GaldovShieldLeft", true), graphics.getSprite("GaldovShieldLeft", true));
	self->setThinkTime(2);
	self->setActionFinished(2);
	addTeleportParticles(self->x + (self->width / 2), self->y + (self->height / 2), 50, SND_TELEPORT3);
	self->think = &galdovFinalTeleport;
	
	if (map.boss[0]->health <= 15 * game.skill)
	{
		self->setSprites(graphics.getSprite("GaldovRight", true), graphics.getSprite("GaldovLeft", true), graphics.getSprite("GaldovDie", true));
		Math::removeBit(&map.boss[0]->flags, ENT_IMMUNE);
		map.boss[0]->react = &galdovFinalReact2;
	}
}

void galdovFinalPainThrow2()
{
	self->dx = -3;
	self->dy = -12;
	
	self->think = &galdovFinalStandUp;
	self->setThinkTime(2);
	self->setActionFinished(90);
}

void orbSeekGaldov()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	float dx, dy;
	
	Math::calculateSlope(map.boss[0]->x, map.boss[0]->y, self->x, self->y, &dx, &dy);
	
	self->dx = (dx * (1 + game.skill));
	self->dy = (dy * (1 + game.skill));
	
	self->setThinkTime(2);
	self->setActionFinished(5);
	self->think = &orbSeekGaldov;
	
	int distX = (int)fabs(map.boss[0]->x - self->x);
	int distY = (int)fabs(map.boss[0]->y - self->y);

	distX *= distX;
	distY *= distY;

	int distance = (int)sqrt(distX + distY);
	
	if (distance <= 10)
	{
		self->active = false;
		self->setActionFinished(60);
		self->setThinkTime(60);
		addExplosion(self->x, self->y, 75, &player);
		audio.playSound(SND_BOSSCUSTOM2, CH_AMBIANCE, self->x);
		self->place(9999, 9999);
		map.boss[0]->setSprites(graphics.getSprite("GaldovPain", true), graphics.getSprite("GaldovPain", true), graphics.getSprite("GaldovPain", true));
		map.boss[0]->think = &galdovFinalPainThrow2;
		map.boss[0]->health -= (3 * game.skill);
		Math::removeBit(&map.boss[0]->flags, ENT_FLIES);
		Math::removeBit(&map.boss[0]->flags, ENT_FIRETRAIL);
	}
}

void galdovFinalShieldInit()
{
	self->setActionFinished(60);
	self->setThinkTime(60);
	self->think = &galdovMiniAttack;
	self->react = NULL;
	
	Math::addBit(&map.boss[0]->flags, ENT_IMMUNE);
	
	for (int i = 1 ; i < 6 ; i++)
	{
		if (map.boss[i] != NULL)
		{
			delete map.boss[i];
		}
		map.boss[i] = new Boss();
		strlcpy(map.boss[i]->name, "OrbBomb", sizeof map.boss[i]->name);
		map.boss[i]->setSprites(graphics.getSprite("DroidOrb", true), graphics.getSprite("DroidOrb", true), graphics.getSprite("DroidOrb", true));
		map.boss[i]->health = 999999;
		map.boss[i]->maxHealth = 99999;
		map.boss[i]->immune = 0;
		Math::addBit(&map.boss[0]->flags, ENT_IMMUNE);
		map.boss[i]->active = false;
		map.boss[i]->setActionFinished(2);
		map.boss[i]->setThinkTime(2);
		map.boss[i]->think = &orbSeekGaldov;
		map.boss[i]->react = NULL;
		Math::addBit(&map.boss[i]->flags, ENT_FLIES);
	}
	
	map.boss[1]->place(864, 1536);
	map.boss[2]->place(864, 1792);
	map.boss[3]->place(1856, 1792);
	map.boss[4]->place(1888, 1440);
	map.boss[5]->place(1408, 1472);
	
	galdovInitBlackDroids();
}

void galdovFinalWaitForObjective()
{
	self->setActionFinished(120);
	self->setThinkTime(2);
	
	Objective *objective = (Objective*)map.objectiveList.getHead();

	while (objective->next != NULL)
	{
		objective = (Objective*)objective->next;

		if (strstr(objective->target, "Reality"))
		{
			if (objective->completed)
			{
				player.dx = 1000;
				player.dy = 1408;
				Math::addBit(&player.flags, ENT_TELEPORTING);
				self->think = &galdovFinalShieldInit;
				map.boss[0]->setSprites(graphics.getSprite("GaldovShieldRight", true), graphics.getSprite("GaldovShieldLeft", true), graphics.getSprite("GaldovShieldLeft", true));
				self->setActionFinished(2);
				self->setThinkTime(2);
				map.setMainBossPart(map.boss[0]);
			}
		}
	}
}

void galdovFinalFlee1()
{
	self->dx = 800;
	self->dy = 1408;
	addTeleportParticles(self->x + (self->width / 2), self->y + (self->height / 2), 50, SND_TELEPORT3);
	Math::addBit(&self->flags, ENT_TELEPORTING);
	Math::addBit(&self->flags, ENT_FLIES);
	Math::addBit(&self->flags, ENT_FIRETRAIL);
	self->think = &galdovFinalWaitForObjective;
	self->setActionFinished(2);
	self->setThinkTime(2);
}

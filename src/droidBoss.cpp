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

#include "droidBoss.h"

void droidBossAttack();

void droidBossChangeParticles()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	int color = 0;
	float dx, dy;
	
	dx = 0;
	
	switch (self->shieldFrequency)
	{
		case 0:
			color = graphics.red;
			break;
		case 1:
			color = graphics.yellow;
			break;
		case 2:
			color = graphics.green;
			break;
		case 3:
			color = graphics.cyan;
			break;
		case 4:
			color = graphics.white;
			break;
	}
	
	for (int i = 0 ; i < 20 ; i++)
	{
		dy = Math::rrand(-150, -50);
		dy /= 100;
		map.addParticle(self->x + (Math::prand() % self->width), self->y  + (Math::prand() % self->height), dx, dy, Math::rrand(1, 60), color, NULL, PAR_WEIGHTLESS);
	}
	
	self->setThinkTime(2);
	self->setActionFinished(10);
	self->think = &droidBossChangeParticles;
	self->custom--;
	
	if (self->custom == 0)
	{
		self->setThinkTime(2);
		self->setActionFinished(10);
		self->think = &droidBossAttack;
	}
}

void droidBossChasePlayer()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	float dx, dy;
	
	Math::calculateSlope(player.x, player.y, self->x, self->y, &dx, &dy);
	
	self->dx = (dx * 2);
	self->dy = (dy * 2);
	
	self->setThinkTime(10);
	self->setActionFinished(10);
	self->think = &droidBossAttack;
}

void droidBossChangeFrequency()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->shieldFrequency = Math::rrand(WP_PISTOL, WP_SPREAD);
	
	self->think = &droidBossChangeParticles;
	self->setThinkTime(2);
	self->setActionFinished(2);
	self->custom = 5;
	
	Math::removeBit(&self->flags, ENT_AIMS);
	Math::removeBit(&self->flags, ENT_RAPIDFIRE);
	
	switch (self->shieldFrequency)
	{
		case WP_PISTOL:
			self->currentWeapon = getWeaponByName("Aimed Pistol");
			Math::addBit(&self->flags, ENT_AIMS);
			self->setSprites(graphics.getSprite("JetpackBlobRight1", true), graphics.getSprite("JetpackBlobLeft1", true), graphics.getSprite("JetpackBlobLeft1", true));
			break;
		case WP_MACHINEGUN:
			self->currentWeapon = getWeaponByName("Machine Gun");
			Math::addBit(&self->flags, ENT_RAPIDFIRE);
			self->setSprites(graphics.getSprite("JetpackBlobRight2", true), graphics.getSprite("JetpackBlobLeft2", true), graphics.getSprite("JetpackBlobLeft2", true));
			break;
		case WP_LASER:
			self->currentWeapon = getWeaponByName("Alien Laser Cannon");
			self->setSprites(graphics.getSprite("JetpackBlobRight3", true), graphics.getSprite("JetpackBlobLeft3", true), graphics.getSprite("JetpackBlobLeft3", true));
			break;
		case WP_GRENADES:
			self->currentWeapon = getWeaponByName("Aimed Pistol");
			self->shieldFrequency = WP_PISTOL;
			Math::addBit(&self->flags, ENT_AIMS);
			self->setSprites(graphics.getSprite("JetpackBlobRight1", true), graphics.getSprite("JetpackBlobLeft1", true), graphics.getSprite("JetpackBlobLeft1", true));
			break;
		case WP_SPREAD:
			self->currentWeapon = &weapon[WP_ALIENSPREAD];
			Math::addBit(&self->flags, ENT_AIMS);
			self->setSprites(graphics.getSprite("JetpackBlobRight4", true), graphics.getSprite("JetpackBlobLeft4", true), graphics.getSprite("JetpackBlobLeft4", true));
			break;
	}
	
	audio.playSound(SND_BOSSCUSTOM1, CH_AMBIANCE, self->x);
}

void droidBossDie()
{
	self->health -= Math::rrand(1, 2);
	self->setActionFinished(30);
	self->setThinkTime(2);
	
	self->dx = Math::rrand(-3, 3);
	self->dy = Math::rrand(-3, 3);
	
	addExplosion(self->x + Math::prand() % self->width, self->y + Math::prand() % self->height, 50, &player);
	addSmokeAndFire(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 2);
	
	if (self->health <= -100)
	{
		checkObjectives(self->name, false);
		
		map.mainBossPart = NULL;
	}
}

void droidGoCrazy()
{
	self->dx = Math::rrand(-8, 8);
	self->dy = Math::rrand(-8, 8);
	
	(self->dx > 0) ? self->face = 0 : self->face = 1;
	
	if (self->y >= 4800)
		self->dy = -2;
	
	self->setThinkTime(2);
	self->setActionFinished(5);
	self->think = &droidGoCrazy;
	
	self->custom--;
	
	if (self->custom == 0)
	{
		Math::removeBit(&self->flags, ENT_IMMUNE);
		self->think = &droidBossChangeFrequency;
		self->setActionFinished(2);
		self->setThinkTime(2);
	}
}

void droidBossReact()
{
	if ((Math::prand() % 10) == 0)
	{
		Math::addBit(&self->flags, ENT_IMMUNE);
		audio.playSound(SND_BOSSCUSTOM2, CH_SPAWN, self->x);
		self->custom = 5;
		self->think = &droidGoCrazy;
		self->setActionFinished(2);
		self->setThinkTime(2);
	}
}

void droidBossFire()
{	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	if (hasClearShot((Entity*)self))
	{
		self->flags & ENT_AIMS ? addBullet((Entity*)self, 0, 0) : addBullet((Entity*)self, self->currentWeapon->getSpeed(self->face), 0);
		
		if (self->currentWeapon == &weapon[WP_ALIENSPREAD])
		{
			addBullet(self, self->currentWeapon->getSpeed(self->face), 2);
			addBullet(self, self->currentWeapon->getSpeed(self->face), -2);
		}
	}
					
	self->setThinkTime(2);
	self->setActionFinished(self->currentWeapon->reload);
	
	self->custom--;
	if (self->custom == 0)
		self->think = &droidBossAttack;
}

void orbWait()
{
	self->setThinkTime(60);
	self->setActionFinished(60);
}

void orbExplode()
{
	addExplosion(self->x, self->y, 50, self);
	self->setThinkTime(2);
	self->setActionFinished(2);
	self->active = false;
	self->face = 1;
	self->think = &orbWait;
	self->x = 9999;
	self->y = 9999;
}

void doOrb()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	float dx, dy;
	
	Math::calculateSlope(player.x, player.y, self->x, self->y, &dx, &dy);
	
	self->dx = (dx * (1 + game.skill));
	self->dy = (dy * (1 + game.skill));
	
	self->setThinkTime(2);
	self->setActionFinished(Math::rrand(10, 90));
	self->think = &doOrb;
	
	if (game.skill >= 2)
	{
		int distX = (int)fabs(player.x - self->x);
		int distY = (int)fabs(player.y - self->y);
	
		distX *= distX;
		distY *= distY;
	
		int distance = (int)sqrt(distX + distY);
		
		if (distance <= 25)
			self->health = -1;
	}
	
	if ((Math::prand() % 10) == 0)
		self->health = -1;
}

void fireOrb(int i)
{
	debug(("fireOrb()\n"));
	
	map.boss[i]->active = true;
	map.boss[i]->maxHealth = map.boss[i]->health = 1 * game.skill;
	map.boss[i]->x = self->x;
	map.boss[i]->y = self->y + 20;
	
	map.boss[i]->think = &doOrb;
	map.boss[i]->die = &orbExplode;
	map.boss[i]->setActionFinished(2);
	map.boss[i]->setThinkTime(2);
}

void droidBossOrbAttack()
{
	debug(("droidBossClawAttack\n"));
	
	for (int i = 1 ; i < 6 ; i++)
	{
		if (!map.boss[i]->active)
		{
			fireOrb(i);
			self->setThinkTime(2);
			self->setActionFinished(90);
			self->think = &droidBossAttack;
			return;
		}
	}
	
	// nothing to do(!)
	self->think = &droidBossAttack;
}

void droidBossMoveRandomly()
{
	self->dx = Math::rrand(-200, 200);
	self->dy = Math::rrand(-200, 200);
	
	self->dx *= 0.01;
	self->dy *= 0.01;
	
	self->setThinkTime(2);
	self->setActionFinished(90);
	self->think = &droidBossAttack;
}

void droidBossAttack()
{
	if (player.health < 1)
	{
		self->dx = self->dy = 0;
		return;
	}
	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	int r = Math::prand() % 110;
	
	if (r < 30)
	{
		self->think = &droidBossChasePlayer;
		self->setThinkTime(2);
		self->setActionFinished(2);
		self->custom = 10;
	}
	else if (r < 70)
	{
		self->think = &droidBossFire;
		self->setThinkTime(2);
		self->setActionFinished(2);
		self->custom = Math::rrand(1, 3);
	}
	else if (r < 95)
	{
		self->think = &droidBossMoveRandomly;
		self->setThinkTime(2);
		self->setActionFinished(2);
		self->custom = 10;
	}
	else if (r < 108)
	{
		self->think = &droidBossOrbAttack;
		self->setThinkTime(2);
		self->setActionFinished(2);
		self->custom = Math::rrand(1, 3);
	}
	else
	{
		self->think = &droidBossChangeFrequency;
		self->setThinkTime(2);
		self->setActionFinished(2);
	}
}

void droidBossInit()
{
	debug(("droidBossInit\n"));
	
	map.boss[0] = new Boss();
	strlcpy(map.boss[0]->name, "BioMech Jetpack Blob", sizeof map.boss[0]->name);
	map.boss[0]->health = 30 * game.skill;
	map.boss[0]->maxHealth = 30 * game.skill;
	map.boss[0]->setSprites(graphics.getSprite("JetpackBlobRight1", true), graphics.getSprite("JetpackBlobLeft1", true), graphics.getSprite("JetpackBlobLeft1", true));
	map.boss[0]->currentWeapon = getWeaponByName("Aimed Pistol");
	map.boss[0]->face = 1;
	map.boss[0]->active = true;
	map.boss[0]->x = 3172;
	map.boss[0]->y = 4646;
	map.boss[0]->immune = 0;
	map.boss[0]->setThinkTime(2);
	map.boss[0]->setActionFinished(2);
	map.boss[0]->think = &droidBossChangeFrequency;
	map.boss[0]->react = &droidBossReact;
	map.boss[0]->die = &droidBossDie;
	Math::addBit(&map.boss[0]->flags, ENT_FLIES);
	
	map.setMainBossPart(map.boss[0]);
	
	for (int i = 1 ; i < 6 ; i++)
	{
		map.boss[i] = new Boss();
		strlcpy(map.boss[i]->name, "Orb Bomb", sizeof map.boss[i]->name);
		map.boss[i]->setSprites(graphics.getSprite("DroidOrb", true), graphics.getSprite("DroidOrb", true), graphics.getSprite("DroidOrb", true));
		map.boss[i]->active = false;
		map.boss[i]->face = 1;
		map.boss[i]->think = &orbWait;
		map.boss[i]->x = 9999;
		map.boss[i]->y = 9999;
		Math::addBit(&map.boss[i]->flags, ENT_FLIES);
	}
	
	audio.loadSound(SND_BOSSCUSTOM1, "sound/droidChangeFreq");
	audio.loadSound(SND_BOSSCUSTOM2, "sound/droidFreakOut");
	
	debug(("droidBossInit: Done\n"));
}

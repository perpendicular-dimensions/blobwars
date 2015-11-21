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

#include "tankBoss.h"

// **************** Machine Gun Boss ***********************

void tankBossMGFight();
void tankBossGLFight();
void activateGLBoss();

/**
* Adds a load of particles within an area of the specified location
* @param x The x location
* @param y The y location
*/
void plasmaBurstTraceParticles(int x, int y)
{
	int color[7];
	
	for (int i = 0 ; i < 7 ; i++)
	{
		color[i] = SDL_MapRGB(graphics.screen->format, 0, 100 + (i * 20), 0);
	}
	
	int dx = 0;

	for (int i = 0 ; i < 25 ; i++)
	{
		dx = Math::rrand(0, 5);
		
		if (self->face == 1)
		{
			dx = -dx;
		}

		map.addParticle(x + Math::rrand(-6, 6), y + Math::rrand(-2, 2), dx, 0, Math::rrand(1, 90), color[Math::rrand(0, 6)], NULL, PAR_WEIGHTLESS);
	}
}

/**
* Adds particles to the front of the MG Tank Boss's gun
*/
void plasmaChargeParticles()
{
	int x = (int)self->x;
	int y = (int)self->y + 10;
	
	if (self->face == 0)
	{
		x += self->getFaceImage()->w;
	}
	
	float dx, dy;
	
	for (int i = 0 ; i < 2 ; i++)
	{
		dx = Math::rrand(-30, 30); dx /= 90;
		dy = Math::rrand(-30, 30); dy /= 90;
		map.addParticle(x, y, dx, dy, Math::rrand(30, 60), graphics.green, NULL, PAR_WEIGHTLESS);
	}
}

/**
* Traces a line from the MG Tank Boss to 800 pixel in the
* direction it is facing. Anything in the path is killed immediately.
*/
void traceTankBossMGFireLine()
{
	int dx = 1;
	
	if (self->face == 1)
	{
		dx = -1;
	}
	
	int x;
	int y = (int)self->y + 10;
	
	Entity *enemy = NULL;
	
	for (int i = 0 ; i < 800 ; i++)
	{
		x = (int)self->x;
		
		if (self->face == 0)
		{
			x += self->getFaceImage()->w;
		}
		
		x += (i * dx);
		
		plasmaBurstTraceParticles(x, y);
		
		if (Collision::collision(player.x, player.y, 20, 20, x - 1, y - 1, 2, 2))
		{
			throwAndDamageEntity(&player, 100, dx * 10, dx * 10, 0);
			
			for (int i = 0 ; i < 10 ; i++)
			{
				addExplosion(player.x + Math::rrand(-25, 25), player.y + Math::rrand(-25, 25), 10, self);
			}
		}
		
		enemy = (Entity*)map.enemyList.getHead();

		while (enemy->next != NULL)
		{
			enemy = (Entity*)enemy->next;
			
			if (Collision::collision(enemy->x, enemy->y, enemy->getFaceImage()->w, enemy->getFaceImage()->h, x - 1, y - 1, 2, 2))
			{
				throwAndDamageEntity(enemy, 100, dx * 10, dx * 10, 0);
				
				enemy->health = -1;
				
				for (int i = 0 ; i < 10 ; i++)
				{
					addExplosion(enemy->x + Math::rrand(-25, 25), enemy->y + Math::rrand(-25, 25), 10, self);
				}
			}
		}	
	}
	
	self->setThinkTime(2);
	self->setActionFinished(90);
	Math::removeBit(&self->flags, ENT_IMMUNE);
	self->think = &tankBossMGFight;
}

/**
* Tells the Machine Gun Tank to fire it's beam laser
*/
void tankBossMGCannonFire()
{
	self->setThinkTime(2);
	self->setActionFinished(2);
	self->think = &tankBossMGCannonFire;
	
	traceTankBossMGFireLine();
}

/**
* Tells the Machine Gun boss to begin charging it's
* cannon and play the appropriate sound
*/
void tankBossMGCannonChargeFire()
{
	plasmaChargeParticles();
	
	self->setActionFinished(2);
	
	self->custom--;
	
	if (self->custom == 0)
	{
		self->think = tankBossMGCannonFire;
		audio.playSound(SND_BOSSCUSTOM2, CH_AMBIANCE, self->x);
	}
}

void tankBossMGRapidFire()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	addBullet((Entity*)self, 0, 0);
	
	self->think = &tankBossMGRapidFire;
	self->setActionFinished(4);
	self->custom--;
	
	if (self->custom <= 0)
	{
		self->think = &tankBossMGFight;
		self->setThinkTime(2);
		self->setActionFinished(90);
	}
}

void tankBossMGFire()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	addBullet((Entity*)self, 0, 0);
	
	self->think = &tankBossMGFire;
	self->setActionFinished(15);
	self->custom--;
	
	if (self->custom <= 0)
	{
		self->think = &tankBossMGFight;
		self->setThinkTime(2);
		self->setActionFinished(60);
	}
}

void tankBossMGJump()
{
	self->dy = -12;
	
	(player.x < self->x) ? self->dx = -6 : self->dx = 6;
	
	self->think = &tankBossMGFight;
	self->setActionFinished(45);
}

void tankBossMGDie()
{
	self->setThinkTime(60);
	self->setActionFinished(60);
	self->health -= Math::rrand(1, 2);
	
	// don't hurt the player(!)
	addExplosion(self->x, self->y, 50, &player);
	addSmokeAndFire(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 2);
	addBlood(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 3);
	
	if ((Math::prand() % 5) == 0)
	{
		self->dx = Math::rrand(-15, 15);
		self->dy = Math::rrand(-8, 0);
	}
	
	if (self->health <= -100)
	{
		checkObjectives(self->name, false);
		
		activateGLBoss();
	}
}

void tankBossMGAttack()
{
	(player.x > self->x) ? self->face = 0 : self->face = 1;
	
	int r = Math::prand() % 12;
	
	if (r < 6)
	{
		self->custom = Math::rrand(1, 6);
		self->think = &tankBossMGFire;
		self->setThinkTime(0);
		self->setActionFinished(2);
	}
	else if (r < 10)
	{
		self->custom = Math::rrand(35, 80);
		self->think = &tankBossMGCannonChargeFire;
		self->setThinkTime(0);
		self->setActionFinished(2);
		audio.playSound(SND_BOSSCUSTOM1, CH_AMBIANCE, self->x);
		Math::addBit(&self->flags, ENT_IMMUNE);
	}
	else
	{
		self->custom = 15;
		self->think = &tankBossMGRapidFire;
		self->setThinkTime(0);
		self->setActionFinished(2);
	}
}

void tankBossMGFight()
{
	if (player.health < 1)
	{
		self->dx = 0;
		return;
	}
	
	int r = Math::prand() % 10;
	
	if (r < 5)
	{
		if (player.x - 320 < self->x)
			self->dx = -2;
		else if (player.x + 320 > self->x)
			self->dx = 2;
		else
			self->dx = Math::rrand(-2, 2);
		
		(self->x < player.x) ? self->face = 0 : self->face = 1;
		self->setActionFinished(60);
		self->setThinkTime(1);
	}
	else if (r < 6)
	{
		self->think = &tankBossMGJump;
		self->setActionFinished(2);
		self->setThinkTime(2);
	}
	else
	{
		self->dx = 0;
		self->setThinkTime(2);
		self->think = &tankBossMGAttack;
	}
}

void activateGLBoss()
{
	if (map.boss[1] == NULL)
	{
		map.mainBossPart = NULL;
		return;
	}
	
	map.boss[1]->active = true;
	map.boss[1]->dx = Math::rrand(416, 928);
	map.boss[1]->dy = 8576;
	map.boss[1]->setThinkTime(2);
	map.boss[1]->setActionFinished(2);
	map.boss[1]->think = &tankBossGLFight;
	Math::addBit(&map.boss[1]->flags, ENT_TELEPORTING);
	
	map.setMainBossPart(map.boss[1]);
}

void tankBossMGSwap()
{
	// Machine Gun Boss wait state
	self->active = false;
	self->face = 0;
	
	// Grenade launcher boss active state
	activateGLBoss();
}

void tankBossMGReact()
{
	if (map.boss[1] == NULL)
	{
		return;
	}
	
	if (self->flags & ENT_IMMUNE)
	{
		return;
	}
	
	if ((Math::prand() % 25) > 0)
	{
		return;
	}
	
	self->dx = 915;
	self->dy = 8256;
	self->think = &tankBossMGSwap;
	self->setThinkTime(2);
	self->setActionFinished(2);
	Math::addBit(&self->flags, ENT_TELEPORTING);
}

void tankBossMGStart()
{
	self->dx = Math::rrand(416, 928);
	self->dy = 8576;
	Math::addBit(&self->flags, ENT_TELEPORTING);
	self->think = &tankBossMGFight;
}

void tankBossMGInit()
{
	debug(("tankBossMGInit\n"));
	
	map.boss[0] = new Boss();
	strlcpy(map.boss[0]->name, "BioMech Tank V1.1", sizeof map.boss[0]->name);
	map.boss[0]->health = 65 * game.skill;
	map.boss[0]->maxHealth = 65 * game.skill;
	map.boss[0]->setSprites(graphics.getSprite("BlobTankCannonRight", true), graphics.getSprite("BlobTankCannonLeft", true), graphics.getSprite("BlobTankCannonLeft", true));
	map.boss[0]->currentWeapon = &weapon[WP_PLASMARIFLE];
	map.boss[0]->face = 0;
	map.boss[0]->active = false;
	map.boss[0]->x = 900;
	map.boss[0]->y = 8256;
	map.boss[0]->immune = 0;
	map.boss[0]->setThinkTime(1);
	map.boss[0]->setActionFinished(1);
	map.boss[0]->think = &tankBossMGStart;
	map.boss[0]->react = &tankBossMGReact;
	map.boss[0]->die = &tankBossMGDie;
	Math::addBit(&map.boss[0]->flags, ENT_AIMS);
	
	audio.loadSound(SND_BOSSCUSTOM1, "sound/tankBeam");
	audio.loadSound(SND_BOSSCUSTOM2, "sound/tankBeamFire");
	
	map.setMainBossPart(map.boss[0]);
	
	debug(("tankBossMGInit: Done\n"));
}

// **************** Grenade Launcher Boss ***********************

void tankBossGLJump()
{
	self->dy = -12;
	
	(player.x < self->x) ? self->dx = -6 : self->dx = 6;
	
	self->think = &tankBossGLFight;
	self->setActionFinished(45);
}

void tankBossGLRapidFire()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	addBullet((Entity*)self, 0, 0);
	
	self->think = &tankBossGLRapidFire;
	self->setActionFinished(4);
	self->custom--;
	
	if (self->custom <= 0)
	{
		self->think = &tankBossGLFight;
		self->setThinkTime(2);
		self->setActionFinished(90);
	}
}

void tankBossGLMortor()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	int speed = Math::rrand(3, 5);
	if (self->face == 1)
		speed = -speed;
	
	addBullet((Entity*)self, speed, 0);
	
	self->think = &tankBossGLMortor;
	self->setActionFinished(20);
	self->custom--;
	
	if (self->custom <= 0)
	{
		self->think = &tankBossGLFight;
		self->setThinkTime(2);
		self->setActionFinished(60);
	}
}

void tankBossGLAttack()
{
	(player.x > self->x) ? self->face = 0 : self->face = 1;
	
	int r = Math::prand() % 10;
	
	if (r < 5)
	{
		Math::removeBit(&self->flags, ENT_AIMS);
		self->currentWeapon = getWeaponByName("Mortor Shells");
		self->think = &tankBossGLMortor;
		self->setThinkTime(0);
		self->setActionFinished(2);
		self->custom = Math::rrand(2, 5);
	}
	else if (r < 8)
	{
		Math::addBit(&self->flags, ENT_AIMS);
		self->currentWeapon = getWeaponByName("Aimed Pistol");
		self->custom = 15;
		self->think = &tankBossGLRapidFire;
		self->setThinkTime(0);
		self->setActionFinished(2);
	}
	else
	{
		self->custom = Math::rrand(45, 60);
		//self->think = &tankBossGLCannonChargeFire;
		self->setThinkTime(0);
		self->setActionFinished(2);
	}
}

void tankBossGLFight()
{
	if (player.health < 1)
	{
		self->dx = 0;
		return;
	}
	
	int r = Math::prand() % 10;
	
	if (r < 5)
	{
		if (player.x - 320 < self->x)
			self->dx = -2;
		else if (player.x + 320 > self->x)
			self->dx = 2;
		else
			self->dx = Math::rrand(-2, 2);
		
		(self->x < player.x) ? self->face = 0 : self->face = 1;
		self->setActionFinished(60);
		self->setThinkTime(1);
	}
	else if (r < 6)
	{
		self->think = &tankBossGLJump;
		self->setActionFinished(2);
		self->setThinkTime(2);
	}
	else
	{
		self->dx = 0;
		self->setThinkTime(2);
		self->think = &tankBossGLAttack;
	}
}

void activateMGBoss()
{
	if (map.boss[0] == NULL)
	{
		map.mainBossPart = NULL;
		return;
	}
	
	map.boss[0]->active = true;
	map.boss[0]->dx = Math::rrand(416, 928);
	map.boss[0]->dy = 8576;
	map.boss[0]->setThinkTime(2);
	map.boss[0]->setActionFinished(2);
	map.boss[0]->think = &tankBossMGFight;
	Math::addBit(&map.boss[0]->flags, ENT_TELEPORTING);
	
	map.setMainBossPart(map.boss[0]);
}

void tankBossGLSwap()
{
	// Grenade launcher Boss wait state
	self->active = false;
	self->face = 1;
	
	// Machine Gun boss active state
	activateMGBoss();
}

void tankBossGLReact()
{
	if (map.boss[0] == NULL)
		return;
	
	if ((Math::prand() % 25) > 0)
		return;
	
	self->dx = 420;
	self->dy = 8256;
	self->think = &tankBossGLSwap;
	self->setThinkTime(2);
	self->setActionFinished(2);
	Math::addBit(&self->flags, ENT_TELEPORTING);
}

void tankBossGLDie()
{
	self->setThinkTime(60);
	self->setActionFinished(60);
	self->health -= Math::rrand(1, 2);
	
	// don't hurt the player(!)
	addExplosion(self->x, self->y, 50, &player);
	addSmokeAndFire(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 2);
	addBlood(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 3);
	
	if ((Math::prand() % 5) == 0)
	{
		self->dx = Math::rrand(-15, 15);
		self->dy = Math::rrand(-8, 0);
	}
	
	if (self->health <= -100)
	{
		checkObjectives(self->name, false);
		
		activateMGBoss();
	}
}

void tankBossGLInit()
{
	debug(("tankBossGLInit\n"));
	
	map.boss[1] = new Boss();
	strlcpy(map.boss[1]->name, "BioMech Tank V2.6", sizeof map.boss[1]->name);
	map.boss[1]->health = 65 * game.skill;
	map.boss[1]->maxHealth = 65 * game.skill;
	map.boss[1]->setSprites(graphics.getSprite("BlobTankGrenadeRight", true), graphics.getSprite("BlobTankGrenadeLeft", true), graphics.getSprite("BlobTankGrenadeLeft", true));
	map.boss[1]->currentWeapon = getWeaponByName("Mortor Shells");
	map.boss[1]->face = 1;
	map.boss[1]->active = false;
	map.boss[1]->x = 420;
	map.boss[1]->y = 8256;
	map.boss[1]->immune = 0;
	map.boss[1]->setThinkTime(1);
	map.boss[1]->setActionFinished(1);
	map.boss[1]->think = &tankBossGLFight;
	map.boss[1]->react = &tankBossGLReact;
	map.boss[1]->die = &tankBossGLDie;
	
	debug(("tankBossMGInit: Done\n"));
}

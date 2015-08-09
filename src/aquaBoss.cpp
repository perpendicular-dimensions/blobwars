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

#include "aquaBoss.h"

void aquaBossAttack();

void leachParticles(Entity *enemy)
{
	float dx, dy;
	
	Sprite *blood = graphics.getSprite("RedBloodParticle", true);
	
	Math::calculateSlope(self->x, self->y, enemy->x, enemy->y, &dx, &dy);
	
	dx *= Math::rrand(1, 2);
	dy *= Math::rrand(1, 2);

	for (int i = 0 ; i < 25 ; i++)
	{
		map.addParticle(enemy->x + 10 + Math::rrand(-2, 2), enemy->y + 10 + Math::rrand(-2, 2), dx, dy, Math::rrand(1, 60), graphics.red, blood, PAR_WEIGHTLESS);
	}
	
	addBlood(enemy, 0, 0, 1);
}

void aquaBossRecharge()
{
	debug(("aquaBossRecharge\n"));
	
	if (self->health == self->maxHealth)
	{
		self->think = aquaBossAttack;
		Math::removeBit(&self->flags, ENT_IMMUNE);
		self->setSprites(graphics.getSprite("AquaBossRight", true), graphics.getSprite("AquaBossLeft", true), graphics.getSprite("AquaBossLeft", true));
		return;
	}
	
	bool leachedEnemy = false;
	int diffX, diffY;
	
	Entity *enemy = (Entity*)map.enemyList.getHead();
	
	while (enemy->next != NULL)
	{
		enemy = (Entity*)enemy->next;
		
		// don't leach enemies not in the pool!!
		if (enemy->x >= 1490)
			continue;
		
		diffX = abs((int)(enemy->x - self->x));
		diffY = abs((int)(enemy->y - self->y));
		
		if ((diffX <= 160) && (diffY <= 120))
		{
			if (enemy->health > -100)
			{
				enemy->dx = enemy->dy = 0;
				enemy->health = -1;
				if (enemy->health % 10)
				{
					audio.playSound(SND_DEATH1 + Math::prand() % 3, CH_DEATH, enemy->x);
				}
				
				Math::limitInt(&(++self->health), 0, self->maxHealth);
				self->setActionFinished(25);
				leachParticles(enemy);
				leachedEnemy = true;
			}
		}
	}
	
	self->setActionFinished(5);
	
	if (!leachedEnemy)
	{
		self->think = aquaBossAttack;
		Math::removeBit(&self->flags, ENT_IMMUNE);
		self->setSprites(graphics.getSprite("AquaBossRight", true), graphics.getSprite("AquaBossLeft", true), graphics.getSprite("AquaBossLeft", true));
	}
}

void aquaBossCircleStrafe()
{
	debug(("aquaBossCircleStrafe\n"));
	
	self->think = &aquaBossAttack;
}

void aquaBossRapidLaserFire()
{
	debug(("aquaBossRapidLaserFire\n"));
	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->setThinkTime(2);
	self->setActionFinished(5);
	
	addBullet((Entity*)self, self->currentWeapon->getSpeed(self->face), 0);
	self->think = &aquaBossRapidLaserFire;
	
	self->custom--;
	
	if (self->custom == 0)
	{
		self->think = &aquaBossAttack;
		self->setActionFinished(90);
	}
}

void aquaBossRapidPreLaserFire()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->dx = 0;
	if (self->x < 1150) self->dx = 3;
	if (self->x > 1170) self->dx = -3;
	
	if (self->custom == 0)
	{
		if (self->y > 490)
			self->dy = -2;
		
		if (self->y <= 490)
		{
			self->think = &aquaBossRapidLaserFire;
			self->dy = 3;
			self->custom = Math::rrand(8, 16);
		}
	}
	else
	{
		if (self->y < 940)
			self->dy = 2;
		
		if (self->y >= 940)
		{
			self->think = &aquaBossRapidLaserFire;
			self->dy = -3;
			self->custom = Math::rrand(8, 16);
		}
	}
}

void aquaBossFire()
{
	debug(("aquaBossFire\n"));
	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->setActionFinished(5);
	self->setThinkTime(2);
	self->think = &aquaBossFire;
	
	addBullet((Entity*)self, 0, 0);
	
	self->custom--;
	
	if (self->custom == 0)
	{
		Math::removeBit(&self->flags, ENT_AIMS);
		self->currentWeapon = &weapon[WP_ALIENLASER];
		self->think = &aquaBossAttack;
	}
}

void aquaBossUnProtect()
{
	debug(("aquaBossUnProtect\n"));
	
	Math::removeBit(&self->flags, ENT_IMMUNE);
	self->think = &aquaBossAttack;
	
	self->setSprites(graphics.getSprite("AquaBossRight", true), graphics.getSprite("AquaBossLeft", true), graphics.getSprite("AquaBossLeft", true));
}

void aquaBossProtect()
{
	debug(("aquaBossProtect\n"));
	
	Math::addBit(&self->flags, ENT_IMMUNE);
	self->think = &aquaBossUnProtect;
	self->setThinkTime(Math::rrand(90, 120));
	
	self->setSprites(graphics.getSprite("AquaBossProtectRight", true), graphics.getSprite("AquaBossProtectLeft", true), graphics.getSprite("AquaBossProtectLeft", true));
}

void aquaBossReact()
{
	// They can keep firing as much as they want, but it won't drop its shield now!
	if (self->flags & ENT_IMMUNE)
	{
		self->setThinkTime(Math::rrand(90, 120));
		return;
	}
	
	if ((Math::prand() % 12) == 0)
		aquaBossProtect();
}

void aquaBossDie()
{
	self->health -= Math::rrand(1, 2);
	self->setActionFinished(30);
	self->setThinkTime(2);
	
	self->dx = Math::rrand(-3, 3);
	self->dy = Math::rrand(-3, 3);
	
	addExplosion(self->x, self->y, 50, &player);
	addSmokeAndFire(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 2);
	addBlood(self, Math::rrand(-5, 5), Math::rrand(-5, 5), 3);
	
	if (self->health <= -100)
	{
		checkObjectives(self->name, false);
		
		map.mainBossPart = NULL;
	}
}

void aquaBossAttack()
{
	if (player.health < 1)
	{
		self->dx = 0;
		return;
	}
	
	debug(("aquaBossAttack\n"));
	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	int r = Math::prand() % (15 + game.skill);
	
	if (r < 5)
	{
		self->dx = Math::rrand(-2, 2);
		self->dy = Math::rrand(-2, 2);
		self->setThinkTime(Math::rrand(60, 120));
		self->think = &aquaBossAttack;
	}
	else if (r < 8)
	{
		self->dx = self->dy = 0;
		self->custom = Math::rrand(5, 8);
		self->currentWeapon = &weapon[WP_AIMEDPISTOL];
		Math::addBit(&self->flags, ENT_AIMS);
		self->think = &aquaBossFire;
	}
	else if (r == 8)
	{
		self->think = &aquaBossRapidPreLaserFire;
		self->custom = Math::prand() % 2;
	}
	else if (r == 9)
	{
		//self->think = &aquaBossCircleStrafe;
	}
	else
	{
		self->think = &aquaBossRecharge;
		self->custom = Math::rrand(10, 25) * game.skill;
	}
}

void aquaBossMainInit()
{
	debug(("aquaBossMainInit\n"));
	
	map.boss[0] = new Boss();
	strlcpy(map.boss[0]->name, "BioMech Aqua Blob", sizeof map.boss[0]->name);
	map.boss[0]->health = 45 * game.skill;
	map.boss[0]->maxHealth = 45 * game.skill;
	map.boss[0]->setSprites(graphics.getSprite("AquaBossRight", true), graphics.getSprite("AquaBossLeft", true), graphics.getSprite("AquaBossLeft", true));
	map.boss[0]->currentWeapon = &weapon[WP_ALIENLASER];
	map.boss[0]->face = 0;
	map.boss[0]->active = false;
	map.boss[0]->x = 1072;
	map.boss[0]->y = 740;
	map.boss[0]->immune = 0;
	map.boss[0]->setThinkTime(1);
	map.boss[0]->setActionFinished(1);
	map.boss[0]->think = &aquaBossAttack;
	map.boss[0]->react = &aquaBossReact;
	map.boss[0]->die = &aquaBossDie;
	
	Math::addBit(&map.boss[0]->flags, ENT_SWIMS);
	
	map.setMainBossPart(map.boss[0]);
	
	debug(("aquaBossMainInit: Done\n"));
}

void aquaBossInit()
{
	aquaBossMainInit();
}

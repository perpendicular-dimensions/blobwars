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

#include "galdov.h"

void galdovAttack();
void galdovCheckSplit();

void splitParticles()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	int color = 0;
	float dx, dy;
	
	dx = 0;
	
	switch (Math::prand() % 5)
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
	
	self->setThinkTime(1);
	self->setActionFinished(1);
	self->think = &splitParticles;
	self->custom--;
	
	if (self->custom == 0)
	{
		self->setThinkTime(2);
		self->setActionFinished(2);
		self->think = &galdovAttack;
		
		if (self == map.boss[0])
		{
			if ((Math::prand() % self->health) == 0)
			{
				self->think = &galdovCheckSplit;
			}
		}
		
	}
}

void galdovTeleport()
{
	self->setThinkTime(2);
	self->setActionFinished(2);
	
	int x = (int)player.x >> BRICKSHIFT;
	int y = (int)player.y >> BRICKSHIFT;
	x += Math::rrand(-10, 10);
	y += Math::rrand(-10, 10);
	
	Math::limitInt(&x, 1, 28);
	Math::limitInt(&y, 259, 283);
	
	if (map.data[x][y] == MAP_AIR)
	{
		x = x << BRICKSHIFT;
		y = y << BRICKSHIFT;
		self->x = x;
		self->y = y;
		addTeleportParticles(self->x + 10, self->y + 10, 50, SND_TELEPORT2);
		self->think = &galdovAttack;
	}
}

void galdovReact()
{
	if (self->health < 0)
		return;
	
	int r = Math::prand() % 10;
	
	if (r == 0)
	{
		self->setThinkTime(2);
		self->setActionFinished(2);
		self->think = &galdovTeleport;
		
		r = Math::prand() % 3;
		
		if (r == 0)
		{
			self->setThinkTime(2);
			self->setActionFinished(2);
			self->think = &galdovCheckSplit;
		}
	}
}

void galdovSplit(int i)
{
	audio.playSound(SND_BOSSCUSTOM1, CH_AMBIANCE, map.boss[0]->x);
	
	map.boss[i]->active = true;
	map.boss[i]->health = 4 * game.skill;
	map.boss[i]->maxHealth = 4 * game.skill;
	map.boss[i]->x = map.boss[0]->x;
	map.boss[i]->y = map.boss[0]->y;
	map.boss[i]->flags = map.boss[0]->flags;
	map.boss[i]->think = &galdovAttack;
	
	if ((Math::prand() % 2) == 0)
	{
		map.boss[i]->currentWeapon = getRandomAimedWeapon();
		Math::addBit(&map.boss[i]->flags, ENT_AIMS);
	}
	else
	{
		map.boss[i]->currentWeapon = getRandomStraightWeapon();
		Math::removeBit(&map.boss[i]->flags, ENT_AIMS);
	}
	
	if ((Math::prand() % 2) == 0)
	{
		map.boss[0]->dx = -.5;
		map.boss[0]->dy = 0;
		map.boss[i]->dx = .5;
		map.boss[i]->dy = 0;
	}
	else
	{
		map.boss[0]->dx = .5;
		map.boss[0]->dy = 0;
		map.boss[i]->dx = -.5;
		map.boss[i]->dy = 0;
	}
	
	map.boss[0]->think = &splitParticles;
	map.boss[0]->setThinkTime(1);
	map.boss[0]->setActionFinished(1);
	map.boss[0]->custom = 60;
	
	map.boss[i]->think = &splitParticles;
	map.boss[i]->setThinkTime(1);
	map.boss[i]->setActionFinished(1);
	map.boss[i]->custom = 60;
}

void galdovCheckSplit()
{
	for (int i = 1 ; i < 10 ; i++)
	{
		if (!map.boss[i]->active)
		{
			galdovSplit(i);
			return;
		}
	}
}

void galdovFire()
{	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->setActionFinished(9);
	self->setThinkTime(2);
	self->think = &galdovFire;
	
	if (hasClearShot(self))
	{
		self->flags & ENT_AIMS ? addBullet((Entity*)self, 0, 0) : addBullet((Entity*)self, self->currentWeapon->getSpeed(self->face), 0);
			
		if (self->currentWeapon == &weapon[WP_ALIENSPREAD])
		{
			addBullet(self, self->currentWeapon->getSpeed(self->face), 2);
			addBullet(self, self->currentWeapon->getSpeed(self->face), -2);
		}
	}
	
	self->custom--;
	if (self->custom == 0)
	{
		self->think = &galdovAttack;
	}
}

void galdovRocketAttack()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	if (!hasClearShot(self))
		return;
	
	self->setActionFinished(5);
	self->setThinkTime(2);
	self->think = &galdovRocketAttack;
	
	addBullet((Entity*)self, 0, 0);
	
	self->custom--;
	
	if (self->custom == 0)
	{
		self->think = &galdovAttack;
		self->currentWeapon = getRandomGaldovWeapon();
	}
}

void galdovChasePlayer()
{
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	float dx, dy;
	
	Math::calculateSlope(player.x, player.y, self->x, self->y, &dx, &dy);
	
	self->dx = (dx * 2);
	self->dy = (dy * 2);
	
	self->setThinkTime(10);
	self->setActionFinished(10);
	self->think = &galdovAttack;
}

void galdovMoveRandom()
{
	self->think = &galdovAttack;
	
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
		
		self->think = &galdovAttack;
		self->setActionFinished(45);
	}
}

void galdovDoInvisible()
{
	if ((Math::prand() % 2) == 0)
	{
		map.boss[0]->setSprites(graphics.getSprite("GaldovRight", true), graphics.getSprite("GaldovLeft", true), graphics.getSprite("GaldovLeft", true));
	}
	else
	{
		map.boss[0]->setSprites(graphics.getSprite("GaldovInvsRight", true), graphics.getSprite("GaldovInvsLeft", true), graphics.getSprite("GaldovInvsLeft", true));
	}
	
	self->setThinkTime(2);
	self->setActionFinished(2);
	self->think = &galdovAttack;
}

void galdovAttack()
{	
	if (player.health < 1)
	{
		self->dx = self->dy = 0;
		return;
	}
	
	(self->x < player.x) ? self->face = 0 : self->face = 1;
	
	self->think = &galdovAttack;
	self->setThinkTime(2);
	self->setActionFinished(2);
	
	int r = Math::prand() % 100;
	
	if (r < 10)
	{
		self->think = &galdovFire;
		self->custom = Math::rrand(1, 8);
	}
	else if (r < 17)
	{
		// Make sure only the real Galdov does this!
		if (self == map.boss[0])
		{
			addTeleportParticles(self->x + 10, self->y + 10, 50, SND_TELEPORT2);
			self->x = -9999;
			self->y = -9999;
			self->setActionFinished(120);
			self->setThinkTime(2);
			self->think = &galdovTeleport;
		}
	}
	else if (r < 40)
	{
		self->think = &galdovChasePlayer;
	}
	else if (r < 60)
	{
		self->think = &galdovMoveRandom;
	}
	else if (r < 80)
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
	else if (r < 85)
	{
		if (self == map.boss[0])
		{
			self->think = &galdovDoInvisible;
		}
	}
	else if (r < 90)
	{
		map.boss[0]->currentWeapon = getRandomGaldovWeapon();
	}
	else if (r == 91)
	{
		// Make sure only the real Galdov does this!
		if (self == map.boss[0])
		{
			self->custom = Math::rrand(4, 8);
			self->currentWeapon = &weapon[WP_ROCKETS];
			self->think = &galdovRocketAttack;
		}
	}
	else
	{
		// Make sure only the real Galdov does this!
		if (self == map.boss[0])
		{
			self->think = &galdovCheckSplit;
		}
	}
}

void galdovDie()
{	
	self->health--;
	
	if (map.mainBossPart != NULL)
	{
		map.mainBossPart = NULL;
		audio.playSound(SND_BOSSCUSTOM2, CH_AMBIANCE, self->x);
	}
	
	if ((self->health % 5) == 0)
	{
		self->setActionFinished(5);
		self->setThinkTime(2);
		
		self->dx = Math::rrand(-5, 5);
		self->dy = Math::rrand(-5, 5);
		
		addExplosion(self->x, self->y, 5, &player);
		addSmokeAndFire(self, Math::rrand(-4, 4), Math::rrand(-4, 4), 2);
	}
	
	for (int i = 1 ; i < 10 ; i++)
	{
		if (map.boss[i]->health > -1)
		{
			map.boss[i]->health = -1;
		}
	}
	
	if (self->health <= -100)
	{
		checkObjectives(self->name, false);	
		map.mainBossPart = NULL;
		addTeleportParticles(self->x, self->y, 75, SND_TELEPORT3);
	}
}

void dropItems(int x, int y)
{
	int r;
	
	for (int i = 0 ; i < 2 ; i++)
	{
		r = Math::rrand(ITEM_PISTOL, ITEM_DOUBLECHERRY);
		
		// Grenades make this battle far too easy(!)
		if (r == ITEM_GRENADES)
			r = ITEM_LASER;
		
		if ((Math::prand() % 10) == 0)
			r = ITEM_TRIPLECHERRY;
		
		addItem(defItem[r].id, defItem[r].name, x, y, defItem[r].sprite[0]->name, 240, defItem[r].value, ENT_DYING, true);
	}
}

void fakeDie()
{
	dropItems((int)self->x, (int)self->y);
	
	addExplosion(self->x, self->y, 50, self);
	self->setThinkTime(2);
	self->setActionFinished(2);
	self->active = false;
	self->face = 1;
	self->x = 9999;
	self->y = 9999;
}

void galdovInit()
{
	debug(("galdovInit\n"));
	
	map.boss[0] = new Boss();
	strlcpy(map.boss[0]->name, "Galdov", sizeof map.boss[0]->name);
	map.boss[0]->health = 45 * game.skill;
	map.boss[0]->maxHealth = 45 * game.skill;
	map.boss[0]->setSprites(graphics.getSprite("GaldovRight", true), graphics.getSprite("GaldovLeft", true), graphics.getSprite("GaldovLeft", true));
	map.boss[0]->currentWeapon = &weapon[WP_AIMEDPISTOL];
	map.boss[0]->face = 0;
	map.boss[0]->active = true;
	map.boss[0]->x = 448;
	map.boss[0]->y = 9024;
	map.boss[0]->immune = 0;
	map.boss[0]->setThinkTime(2);
	map.boss[0]->setActionFinished(2);
	map.boss[0]->think = &galdovCheckSplit;
	map.boss[0]->react = &galdovReact;
	map.boss[0]->die = &galdovDie;
	
	Math::addBit(&map.boss[0]->flags, ENT_AIMS);
	
	SDL_SetAlpha(graphics.getSprite("GaldovInvsLeft", true)->image[0], 100);
	SDL_SetAlpha(graphics.getSprite("GaldovInvsRight", true)->image[0], 100);
	
	for (int i = 1 ; i < 10 ; i++)
	{
		map.boss[i] = new Boss();
		strlcpy(map.boss[i]->name, "Fake", sizeof map.boss[i]->name);
		map.boss[i]->setSprites(graphics.getSprite("GaldovRight", true), graphics.getSprite("GaldovLeft", true), graphics.getSprite("GaldovLeft", true));
		map.boss[i]->x = 9999;
		map.boss[i]->y = 9999;
		map.boss[i]->active = false;
		map.boss[i]->think = &galdovAttack;
		map.boss[i]->react = NULL;
		map.boss[i]->die = &fakeDie;
	}
	
	map.setMainBossPart(map.boss[0]);
	
	audio.loadSound(SND_BOSSCUSTOM1, "sound/galdovSplit");
	audio.loadSound(SND_BOSSCUSTOM2, "sound/galdovPain");
	
	debug(("galdovInit: Done\n"));
}

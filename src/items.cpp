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

#include "items.h"

void addItem(int itemType, const char *name, int x, int y, const char *spriteName, int health, int value, int flags, bool randomMovement)
{
	Entity *item = new Entity();

	item->id = itemType;
	item->setName(name);
	item->place(x, y);
	item->setSprites(graphics.getSprite(spriteName, true), graphics.getSprite(spriteName, true), graphics.getSprite(spriteName, true));
	item->health = health;
	item->value = value;
	item->flags = ENT_INANIMATE + ENT_BOUNCES + ENT_COLLECTABLE;

	// raise items taller than the enemy
 	int x1 = x >> BRICKSHIFT;
	int x2 = (x + item->width - 1) >> BRICKSHIFT;
	int y2 = (y + item->height - 1) >> BRICKSHIFT;
	if ((map.isSolid(x1, y2)) || (map.isSolid(x2, y2)))
	{
		item->y = (y2 * BRICKSIZE) - item->height;
	}

	if (randomMovement)
	{
		item->setRandomVelocity();
	}

	item->health += Math::prand() % 120;

	Math::addBit(&item->flags, flags);
	
	if (item->id == ITEM_MISC_INVISIBLE)
	{
		if ((gameData.completedWorld) || (game.skill == 3))
		{
			item->id = ITEM_MISC_NOSHOW;
		}
	}

	map.addItem(item);
}

void dropBossItems(int x, int y)
{
	if ((Math::prand() % 5) > 0)
	{
		return;
	}
	
	int r = Math::rrand(ITEM_PISTOL, ITEM_DOUBLECHERRY);
	
	if (player.environment == ENV_WATER)
	{
		r = Math::rrand(ITEM_CHERRY, ITEM_DOUBLECHERRY);
	}
	
	if ((Math::prand() % 10) == 0)
	{
		r = ITEM_TRIPLECHERRY;
	}
	
	addItem(defItem[r].id, defItem[r].name, x, y, defItem[r].sprite[0]->name, 240, defItem[r].value, ENT_DYING, true);
}

void dropRandomItems(int x, int y)
{
	int mapX = x >> BRICKSHIFT;
	int mapY = y >> BRICKSHIFT;
	
	if (map.isSolid(mapX, mapY))
	{
		return;
	}
	
	if (map.isBossMission)
	{
		dropBossItems(x, y);
		return;
	}
	
	int amount = Math::rrand(1, 5);
	int r = Math::rrand(ITEM_POINTS, ITEM_POINTS7);

	int cherryChance = 10 + (10 * game.skill);

	for (int i = 0 ; i < amount ; i++)
	{
		if ((Math::prand() % 8) == 0)
		{
			r = Math::rrand(ITEM_PISTOL, ITEM_SPREAD);
		}

		if ((Math::prand() % 13) == 0)
		{
			switch (Math::prand() % cherryChance)
			{
				case 0:
					r = ITEM_TRIPLECHERRY;
					break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					r = ITEM_DOUBLECHERRY;
					break;
				default:
					r = ITEM_CHERRY;
					break;
			}
		}

		addItem(defItem[r].id, defItem[r].name, x, y, defItem[r].sprite[0]->name, 240, defItem[r].value, ENT_DYING, true);

		r = Math::rrand(ITEM_POINTS, ITEM_POINTS7);
	}
}

void dropHelperItems(int x, int y)
{
	int amount = Math::rrand(1, 5);
	int r;

	for (int i = 0 ; i < amount ; i++)
	{
		r = Math::rrand(ITEM_PISTOL, ITEM_TRIPLECHERRY);

		addItem(defItem[r].id, defItem[r].name, x, y, defItem[r].sprite[0]->name, 240, defItem[r].value, ENT_DYING, true);
	}
}

void stealCrystal()
{
	Entity *item = (Entity*)map.itemList.getHead();
	
	Objective *objective = (Objective*)map.objectiveList.getHead();

	while (objective->next != NULL)
	{
		objective = (Objective*)objective->next;
		objective->completed = true;
		objective->currentValue = objective->targetValue;
	}

	while (item->next != NULL)
	{
		item = (Entity*)item->next;

		if (strcmp(item->name, "Reality Crystal") == 0)
		{
			item->dx = 0;
			item->dy = 0;
			Math::addBit(&item->flags, ENT_TELEPORTING);
			addTeleportParticles(item->x + (item->width / 2), item->y + (item->height / 2), 50, SND_TELEPORT3);
			return;
		}
	}
}

/*
We have to do this to avoid items being permanently lost.
To ensure we drop them in a safe place we put them in the player's
last check point position...
*/
void dropCarriedItems()
{
	Entity *item = (Entity*)map.itemList.getHead();

	while (item->next != NULL)
	{
		item = (Entity*)item->next;

		if (item->owner != &player)
			continue;

		Math::removeBit(&item->flags, ENT_DYING);
		
		item->owner = item;
		item->health = 240;
		item->dx = 0;
		item->dy = -2;
		item->x = game.checkPointX + Math::rrand(0, 6);
		item->y = game.checkPointY;
		item->flags = ENT_INANIMATE + ENT_BOUNCES + ENT_NOCOLLISIONS;
	}
}

void pickUpItem(Entity *item)
{
	char string[100];

	if (item->flags & ENT_DYING)
	{
		game.totalBonusesCollected++;
	}
	else if (item->id >= ITEM_MISC)
	{
		game.currentMissionItemsCollected++;
		map.foundItems++;
	}

	item->health = 10;
	item->flags = ENT_WEIGHTLESS + ENT_DYING + ENT_NOCOLLISIONS;
	item->dx = 0;
	item->dy = -5;

	switch (item->id)
	{
		case ITEM_PISTOL:
		case ITEM_MACHINEGUN:
		case ITEM_LASER:
		case ITEM_GRENADES:
		case ITEM_SPREAD:
			player.currentWeapon = &weapon[item->id];
			game.currentWeapon = item->id;
			audio.playSound(SND_GETWEAPON, CH_ITEM, item->x);
			break;
		case ITEM_POINTS:
		case ITEM_POINTS2:
		case ITEM_POINTS3:
		case ITEM_POINTS4:
		case ITEM_POINTS5:
		case ITEM_POINTS6:
		case ITEM_POINTS7:
			addPlayerScore(item->value);
			audio.playSound(SND_ITEM, CH_ITEM, item->x);
			break;
		case ITEM_CHERRY:
		case ITEM_DOUBLECHERRY:
		case ITEM_TRIPLECHERRY:
			Math::limitInt(&(player.health += item->value), 0, MAX_HEALTH);
			audio.playSound(SND_GULP + (Math::prand() % 2), CH_ITEM, item->x);
			break;
		case ITEM_MISC:
			item->owner = &player;
		case ITEM_MISC_NOSHOW:
			audio.playSound(SND_ITEM, CH_ITEM, item->x);
			break;
	}

	if ((item->id < ITEM_POINTS) || (item->id > ITEM_POINTS7))
	{
		/*
			oh yeah, right... because "Picked up a Ancient Cog" is really good English, isn't it? It's almost
			as bad as Medal of Honor: Frontline where it said "Picked up 3 Stick Grenade(s)"... Would it really
			have taken that much effort to pass the item number to a function that worked out some basic grammar
			for how many items had been picked up??! Yeah... and EA expect us to pay �45 for that! Probably the
			worst bit about that game was that it was just fecking crap anyway!
		*/
		switch (item->name[0])
		{
			case 'A':
			case 'a':
			case 'I':
			case 'i':
			case 'O':
			case 'o':
			case 'U':
			case 'u':
				snprintf(string, sizeof string, _("Picked up an %s"), item->name);
				break;
			default:
				snprintf(string, sizeof string, _("Picked up a %s"), item->name);
				break;
		}

		if (!map.isBossMission)
			engine.setInfoMessage(string, 0, INFO_NORMAL);

		checkObjectives(item->name, true);
		
		if (strcmp(item->name, "LRTS") == 0)
		{
			presentPlayerMedal("LRTS_PART");
		}
	}
}

bool carryingItem(const char *name)
{
	Entity *item = (Entity*)map.itemList.getHead();

	while (item->next != NULL)
	{
		item = (Entity*)item->next;

		if (item->owner != &player)
			continue;

		if (strcmp(item->name, name) == 0)
		{
			item->owner = item;
			item->health = -999;
			return true;
		}
	}

	return false;
}

void showCarriedItems()
{
	int x = 0;
	int y = 210;
	int itemCount = 0;

	Entity *item = (Entity*)map.itemList.getHead();

	while (item->next != NULL)
	{
		item = (Entity*)item->next;

		if (item->owner != &player)
			continue;

		x += (item->width + 8);
		itemCount++;
	}

	x = ((640 - x) / 2);

	item = (Entity*)map.itemList.getHead();

	while (item->next != NULL)
	{
		item = (Entity*)item->next;

		if (item->owner != &player)
			continue;

		graphics.blit(item->getFaceImage(), x, y, graphics.screen, false);

		x += (item->width + 8);
	}

	if (itemCount == 0)
		graphics.drawString(_("Not carrying anything"), 320, 210, TXT_CENTERED, graphics.screen);
}

void doItems()
{
	Entity *item = (Entity*)map.itemList.getHead();

	while (item->next != NULL)
	{
		Entity *previous = item;
		
		item = (Entity*)item->next;
		
		if (item->id == ITEM_MISC_INVISIBLE)
		{
			continue;
		}

		int x = (int)(item->x - engine.playerPosX);
		int y = (int)(item->y - engine.playerPosY);

		item->think();

		if (item->flags & ENT_TELEPORTING)
		{
			moveEntity(item);
		}
		else if ((abs(x) <= 800) && (abs(y) <= 600) && (item->owner == item))
		{
			// Gravity
			if (!(item->flags & ENT_WEIGHTLESS))
				item->applyGravity();
				
			if (!map.isIceLevel)
				item->dx *= 0.98;

			moveEntity(item);

			if ((item->health >= 60) || ((engine.getFrameLoop() % 3) == 0))
			{
				graphics.blit(item->getFaceImage(), x, y, graphics.screen, false);
			}

			item->animate();

			if ((player.health > 0) && (!(player.flags & ENT_TELEPORTING)))
			{
				if (Collision::collision(&player, item))
				{
					if (item->flags & ENT_COLLECTABLE)
					{
						pickUpItem(item);
					}
				}
			}
		}

		if ((item->health <= 0) && (item->owner != &player))
		{
			map.itemList.remove(previous, item);
			item = previous;
		}
	}
}

void loadDefItems()
{
	if (!engine.loadData("data/defItems"))
		graphics.showErrorAndExit("Couldn't load item definitions file (%s)", "data/defItems");

	char *token = strtok((char*)engine.dataBuffer, "\n");

	int id;
	char name[50];
	char sprite[100];
	int value;

	while (true)
	{
		if (strcmp(token, "@EOF@") == 0)
			break;

		sscanf(token, "%d %*c %[^\"] %*c %s %d", &id, name, sprite, &value);

		defItem[id].id = id;
		defItem[id].setName(name);
		defItem[id].setSprites(graphics.getSprite(sprite, true), graphics.getSprite(sprite, true), graphics.getSprite(sprite, true));
		defItem[id].value = value;

		token = strtok(NULL, "\n");
	}
}

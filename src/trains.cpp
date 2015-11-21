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

#include "trains.h"

/**
* Opens a door and converts it into a normal door (TR_DOOR)
* at the same time. Uses up keys if needed
* @param train The door to open.
*/
void openDoor(Train *train)
{
	if (train->active)
	{
		return;
	}

	switch (train->type)
	{
		case TR_GOLD_DOOR:
		case TR_GOLD_SLIDEDOOR:
			engine.setInfoMessage("Used Gold Key", 1, INFO_NORMAL);
			break;
		case TR_SILVER_DOOR:
		case TR_SILVER_SLIDEDOOR:
			engine.setInfoMessage("Used Silver Key", 1, INFO_NORMAL);
			break;
		case TR_BRONZE_DOOR:
		case TR_BRONZE_SLIDEDOOR:
			engine.setInfoMessage("Used Bronze Key", 1, INFO_NORMAL);
			break;
	}

	if ((train->type != TR_LOCKED_DOOR) && (train->type != TR_LOCKED_SLIDEDOOR))
	{
		if (train->type < TR_SLIDEDOOR)
		{
			train->type = TR_DOOR;
		}
		else
		{
			train->type = TR_SLIDEDOOR;
		}
	}

	train->active = true;

	audio.playSound(SND_OPENDOOR, CH_TOUCH, train->x);
}

/**
* Blocks an entity from moving any further and shows
* a message for the reason the door would not open
* @param ent The entity to block
* @param message The message to show when blocking the Player
* @param train The door that performs the blocking
* @param dir The direction the entity was moving in (required for clipping)
*/
void trainBlockEntity(Entity *ent, const char *message, Train *train, int dir)
{
	if (ent == &player)
	{
		if ((train->isReady()) && (!train->active))
		{
			engine.setInfoMessage(message, 1, INFO_NORMAL);
			audio.playSound(SND_LOCKEDDOOR, CH_TOUCH, train->x);
		}
	}
	
	if ((ent->flags & ENT_BULLET) && (!(ent->flags & ENT_BOUNCES)))
	{
		if (dir & DIR_X)
		{
			if (ent->dx < 0) ent->x = train->x + train->sprite->image[0]->w;
			if (ent->dx > 0) ent->x = train->x - ent->width;
		}
	}

	if (dir & DIR_Y)
	{
		if ((ent->dy >= 0) && (train->type >= TR_SLIDEDOOR))
		{
			ent->dy = 0;
			ent->falling = false;
		}
	}
}

void getTrainMotion(Entity *ent, int &dx, int &dy)
{
	dx = 0;
	dy = 0;
	Train *train = (Train*)map.trainList.getHead();
	while (train->next != NULL) {
		train = (Train*)train->next;
		bool collision = (Collision::collision(ent->x, ent->y + ent->dy, ent->width, ent->height - 1, train->x, train->y, train->width, train->height));
		if(collision) {
			dx = train->getDX();
			dy = train->getDY();
			break;
		}
	}
}

/**
* Checks to see if an entity has touched this train. Depending on
* the trains status certain other functions will be invoked
* @param ent The entity to test
* @param dir The direction the entity was moving in
* @return Whether a collision took place
*/
bool checkTrainContact(Entity *ent, int dir)
{
	Train *train = (Train*)map.trainList.getHead();

	bool collision = false;
	int x, y, mapAttribute;

	while (train->next != NULL)
	{
		train = (Train*)train->next;

		if (dir == DIR_X)
		{
			collision = (Collision::collision(ent->x + ent->dx, ent->y, ent->width, ent->height - 1, train->x, train->y, train->width, train->height));
		}
		else if (dir == DIR_Y)
		{
			collision = (Collision::collision(ent->x, ent->y + ent->dy, ent->width, ent->height - 1, train->x, train->y, train->width, train->height));
		}
		else
		{
			collision = (Collision::collision(ent->x + ent->dx, ent->y + ent->dy, ent->width, ent->height - 1, train->x, train->y, train->width, train->height));
		}

		if (collision)
		{
			switch (train->type)
			{
				case TR_TRAIN:
					if (ent->flags & ENT_BULLET)
					{
						return true;
					}
					
					if (ent->flags & ENT_FLIES)
					{
						return false;
					}

					if ((ent == &player) && (train->waitsForPlayer()))
					{
						train->active = true;
					}

					x = (int)(ent->x + ent->dx) >> BRICKSHIFT;
					y = (int)(ent->y + ent->height - 1) >> BRICKSHIFT;

					mapAttribute = map.data[x][y];

					evaluateMapAttribute(ent, mapAttribute);

					if (ent->dy >= 0)
					{
						if (train->active)
						{
							if (!map.isIceLevel)
							{
								ent->x -= train->getDX();
							}
						}

						ent->dy = 1;

						ent->y = train->y;
						ent->y -= ent->height;

						ent->falling = false;
					}

					break;

				case TR_DOOR:
				case TR_SLIDEDOOR:
					if (!(ent->flags & ENT_BULLET))
					{
						openDoor(train);
					}
					
					if (dir & DIR_Y)
					{
						ent->dy = 0;
						ent->falling = false;
					}
					return true;
					break;

				case TR_LOCKED_DOOR:
				case TR_LOCKED_SLIDEDOOR:
					trainBlockEntity(ent, "Door is locked", train, dir);
					return true;
					break;

				case TR_GOLD_DOOR:
				case TR_GOLD_SLIDEDOOR:
					if ((ent == &player) && (carryingItem("Gold Key")))
					{
						openDoor(train);
					}
					else
					{
						trainBlockEntity(ent, "Gold Key Required", train, dir);
					}
					return true;
					break;

				case TR_SILVER_DOOR:
				case TR_SILVER_SLIDEDOOR:
					if ((ent == &player) && (carryingItem("Silver Key")))
					{
						openDoor(train);
					}
					else
					{
						trainBlockEntity(ent, "Silver Key Required", train, dir);
					}
					return true;
					break;

				case TR_BRONZE_DOOR:
				case TR_BRONZE_SLIDEDOOR:
					if ((ent == &player) && (carryingItem("Bronze Key")))
					{
						openDoor(train);
					}
					else
					{
						trainBlockEntity(ent, "Bronze Key Required", train, dir);
					}
					return true;
					break;
			}
		}
	}

	return false;
}

/**
* Lazy way of setting the sprite for the train
* @param train The train to set the Sprite for
*/
void setTrainSprite(Train *train)
{
	switch (train->type)
	{
		case TR_TRAIN:
			train->sprite = graphics.getSprite("Platform", true);
			break;
		case TR_DOOR:
		case TR_LOCKED_DOOR:
			train->sprite = graphics.getSprite("NormalDoor", true);
			break;
		case TR_GOLD_DOOR:
			train->sprite = graphics.getSprite("GoldDoor", true);
			break;
		case TR_SILVER_DOOR:
			train->sprite = graphics.getSprite("SilverDoor", true);
			break;
		case TR_BRONZE_DOOR:
			train->sprite = graphics.getSprite("BronzeDoor", true);
			break;
		case TR_SLIDEDOOR:
		case TR_LOCKED_SLIDEDOOR:
			train->sprite = graphics.getSprite("SlideDoor", true);
			break;
		case TR_GOLD_SLIDEDOOR:
			train->sprite = graphics.getSprite("GoldSlideDoor", true);
			break;
		case TR_SILVER_SLIDEDOOR:
			train->sprite = graphics.getSprite("SilverSlideDoor", true);
			break;
		case TR_BRONZE_SLIDEDOOR:
			train->sprite = graphics.getSprite("BronzeSlideDoor", true);
			break;
	}
}

/**
* Checks to see if a door has attempted to open or close on the Player
* or an enemy
* @param train The door to perform the check on
* @return Whether an entity was in the path of the door
*/
bool doorClosedOnEntity(Train *train)
{
	// allow entities to stand on an horizontal moving door without blocking its movement.
	int y = (train->type < TR_SLIDEDOOR) ? (int)train->y : (int)train->y + 1;
	
	if (Collision::collision(player.x, player.y, player.width, player.height, train->x, y, train->width, train->height))
	{
		return true;
	}
	
	Entity *enemy = (Entity*)map.enemyList.getHead();

	while (enemy->next != NULL)
	{
		enemy = (Entity*)enemy->next;
		
		if (Collision::collision(enemy->x, enemy->y, enemy->width, enemy->height, train->x, y, train->width, train->height))
		{
			return true;
		}
	}
	
	return false;
}

/**
* Peforms actions for all the trains on the level. Moves them, etc.
*/
void doTrains()
{
	Train *train = (Train*)map.trainList.getHead();

	int x, y, oldX, oldY;
	int playSound = false;

	while (train->next != NULL)
	{
		train = (Train*)train->next;

		x = (int)(train->x - engine.playerPosX);
		y = (int)(train->y - engine.playerPosY);

		if ((train->type == TR_TRAIN) && (train->active))
		{
			train->move();
		}

		if (train->type != TR_TRAIN)
		{
			oldX = (int)train->x;
			oldY = (int)train->y;
			
			playSound = train->openClose();
			
			// only check if the door actually moved(!)
			if ((oldX != (int)train->x) || (oldY != (int)train->y))
			{
				if (doorClosedOnEntity(train))
				{
					train->x = oldX;
					train->y = oldY;
				}
				else if (playSound)
				{
					audio.playSound(SND_DOOROPENED, CH_TOUCH, train->x);
				}
			}
		}

		if (train->sprite == NULL)
		{
			setTrainSprite(train);
		}

		if (train->sprite && (abs(x) <= 800) && (abs(y) <= 600))
		{
			graphics.blit(train->sprite->getCurrentFrame(), x, y, graphics.screen, false);
		}
	}
}

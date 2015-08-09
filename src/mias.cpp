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

#include "mias.h"

void initMIAPhrases()
{
	strlcpy(mia_scared[0], "help me...", sizeof mia_scared[0]);
	strlcpy(mia_scared[1], "i don't wanna die...", sizeof mia_scared[1]);
	strlcpy(mia_scared[2], "please... someone help...", sizeof mia_scared[2]);
	strlcpy(mia_scared[3], "i... i'm scared...", sizeof mia_scared[3]);
	strlcpy(mia_scared[4], "i wanna go home...", sizeof mia_scared[4]);
	strlcpy(mia_scared[5], "what was that?!", sizeof mia_scared[5]);
	strlcpy(mia_scared[6], "i don't like it here...", sizeof mia_scared[6]);
}

void addMIA(const char *name, int x, int y, int type)
{
	Entity *mia = new Entity();

	strlcpy(mia->name, name, sizeof mia->name);
	mia->id = type;
	mia->baseThink = 60;
	mia->health = 180;
	mia->place(x, y);
	mia->value = Math::rrand(0, 5);
	mia->flags = ENT_INANIMATE; // MIAs don't drown

	switch (mia->id)
	{
		case MIA_NORMAL:
			mia->setSprites(graphics.getSprite("ScaredMIA", true), graphics.getSprite("ScaredMIA", true), graphics.getSprite("ScaredMIA", true));
			break;
		case MIA_AQUA:
			mia->setSprites(graphics.getSprite("AquaMIA", true), graphics.getSprite("AquaMIA", true), graphics.getSprite("AquaMIA", true));
			break;
	}

	map.addMIA(mia);
}

void doMIAs()
{
	Entity *mia = (Entity*)map.miaList.getHead();

	int x, y;

	char message[256];

	while (mia->next != NULL)
	{
		mia = (Entity*)mia->next;

		if (mia->health <= 0)
			continue;

		mia->think();

		if (mia->flags & ENT_TELEPORTING)
		{
			moveEntity(mia);
		}
		else
		{
			x = (int)(mia->x - engine.playerPosX);
			y = (int)(mia->y - engine.playerPosY);

			if ((abs(x) <= 2048) && (abs(y) <= 768))
			{
				moveEntity(mia);

				if (mia->value < 100)
				{
					if ((Math::prand() % 250) == 0)
						mia->value = 100;
				}
				else
				{
					if ((Math::prand() % 250) == 0)
						mia->value = Math::rrand(0, 6);
				}

				if ((mia->value != 100) && (!(mia->flags & ENT_DYING)))
				{
					static Graphics::SurfaceCache cache;
					graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
					graphics.drawString(_((char*)mia_scared[mia->value]), x + 10, y - 10, true, graphics.screen, cache);
				}

				graphics.blit(mia->getFaceImage(), x, y, graphics.screen, false);
				mia->animate();

			}

			if ((Collision::collision(&player, mia)) && (player.health > 0) && (!(player.flags & ENT_TELEPORTING)))
			{
				if (!(mia->flags & ENT_DYING))
				{
					Math::addBit(&mia->flags, ENT_WEIGHTLESS);
					Math::addBit(&mia->flags, ENT_DYING);
					audio.playSound(SND_TELEPORT1, CH_ANY, mia->x);
				}
			}
			
			if ((mia->id == MIA_NORMAL) && (mia->environment == ENV_WATER))
			{
				mia->id = MIA_AQUA;
				mia->setSprites(graphics.getSprite("AquaMIA", true), graphics.getSprite("AquaMIA", true), graphics.getSprite("AquaMIA", true));
				debug(("MIA '%s' fell into water. Became Aqua Mia\n", mia->name));
			}

			if (mia->flags & ENT_DYING)
			{
				for (int i = 0 ; i < 2 ; i++)
					map.addParticle(mia->x + Math::rrand(-2, 15), mia->y + Math::prand() % mia->height, 0, Math::rrand(-5, -1), Math::rrand(30, 60), graphics.red, graphics.getSprite("TeleportStar", true), PAR_WEIGHTLESS);

				if (mia->health <= 100)
					mia->y -= 5;

				if (mia->health <= 0)
				{
					map.foundMIAs++;
					game.totalMIAsRescued++;

					if ((map.foundMIAs == (map.requiredMIAs / 2)) || (game.skill == 0))
					{
						snprintf(message, sizeof message, _("Rescued %s - Checkpoint Reached!"), mia->name);
						game.setObjectiveCheckPoint();
					}
					else
					{
						snprintf(message, sizeof message, _("Rescued %s!"), mia->name);
					}

					if (map.foundMIAs == map.requiredMIAs)
					{
						snprintf(message, sizeof message, _("Rescue %d MIAs - Objective Complete - Checkpoint Reached!"), map.requiredMIAs);
						game.setObjectiveCheckPoint();
					}
					
					// MIA Medals
					if (game.totalMIAsRescued == 50)
					{
						presentPlayerMedal("50_MIA");
					}
					else if (game.totalMIAsRescued == 100)
					{
						presentPlayerMedal("100_MIA");
					}
					else if (game.totalMIAsRescued == 200)
					{
						presentPlayerMedal("200_MIA");
					}
					else if (game.totalMIAsRescued == 227)
					{
						presentPlayerMedal("ALL_MIA");
					}

					engine.setInfoMessage(message, 1, INFO_OBJECTIVE);
				}
			}
		}
	}
}

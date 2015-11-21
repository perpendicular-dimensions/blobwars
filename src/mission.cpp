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

#include "mission.h"

// This is for easy mode
void skipBossMission()
{
	if (game.stagesCleared == 5)
	{
		gameData.addCompletedObjective("BioMech Supply Depot", "Easy Mode Skip", 1, 1);
		game.stagesCleared = 6;
	}
	
	if (game.stagesCleared == 10)
	{
		gameData.addCompletedObjective("BioMech Communications", "Easy Mode Skip", 1, 1);
		game.hasAquaLung = true;
		game.stagesCleared = 11;
		SDL_FillRect(graphics.screen, NULL, graphics.black);
		graphics.delay(250);
		graphics.drawString(_("You got the Aqua Lung!"), 320, 420, TXT_CENTERED, graphics.screen);
		graphics.delay(2000);
		SDL_FillRect(graphics.screen, NULL, graphics.black);
	}
	
	if (game.stagesCleared == 15)
	{
		gameData.addCompletedObjective("BioMech Assimilator", "Easy Mode Skip", 1, 1);
		game.hasJetPack = true;
		game.stagesCleared = 16;
		SDL_FillRect(graphics.screen, NULL, graphics.black);
		graphics.delay(250);
		graphics.drawString(_("You got the Jet Pack!"), 320, 420, TXT_CENTERED, graphics.screen);
		graphics.delay(2000);
		SDL_FillRect(graphics.screen, NULL, graphics.black);
	}
	
	if (game.stagesCleared == 21)
	{
		gameData.addCompletedObjective("BioMech HQ", "Easy Mode Skip", 1, 1);
		game.stagesCleared = 22;
	}
}

void processPostMissionData()
{
	char string[1024];
	
	if (!gameData.stagePreviouslyCleared(game.stageName))
	{
		presentPlayerMedal(game.stageName);
		game.stagesCleared++;
	}
	
	if (game.skill == 0)
	{
		skipBossMission();
	}
	
	gameData.setMIARescueCount(map.name, map.foundMIAs, map.totalMIAs);

	Objective *objective = (Objective*)map.objectiveList.getHead();
	Entity *mia = (Entity*)map.miaList.getHead();

	while (objective->next != NULL)
	{
		objective = (Objective*)objective->next;
		
		gameData.addCompletedObjective(map.name, objective->description, objective->currentValue, objective->targetValue);
	}

	bool miaFound;

	while (mia->next != NULL)
	{
		mia = (Entity*)mia->next;

		miaFound = true;

		if (mia->health > 0)
			miaFound = false;

		snprintf(string, sizeof string, "MIA_%s", mia->name);
		
		if (miaFound)
		{
			gameData.addCompletedObjective(map.name, string, 1, 1);
		}
		else
		{
			gameData.addCompletedObjective(map.name, string, 0, 1);
		}
	}

	game.totalUpStats();
	
	if (game.totalEnemiesDefeated >= 1000)
	{
		presentPlayerMedal("1000_Enemies");
	}
	
	if (game.totalEnemiesDefeated >= 2500)
	{
		presentPlayerMedal("2500_Enemies");
	}
}

void clearAllMissionData()
{
	char levelMIAKey[100];
	
	snprintf(levelMIAKey, sizeof levelMIAKey, "%s MIAs", game.stageName);
	
	Data *data = (Data*)gameData.dataList.getHead();
	Data *previous = data;
	
	while (data->next != NULL)
	{
		data = (Data*)data->next;
		
		if ((strcmp(data->key, game.stageName) == 0) || (strstr(data->key, levelMIAKey)))
		{
			gameData.dataList.remove(previous, data);
			data = previous;
		}
		else
		{
			previous = data;
		}
	}
	
	map.destroyPersistant(map.name);
}

void showMissionClear()
{
	int colCount = 7;
	bool perfect = perfectlyCompleted();

	SDL_Surface *clear;

	(perfect) ? clear = graphics.loadImage("gfx/main/areaPerfect.png") : clear = graphics.loadImage("gfx/main/areaClear.png");

	graphics.loadBackground("gfx/main/areaClearBackGround.jpg");

	SDL_Surface *panel = graphics.alphaRect(550, 420, 0x00, 0x00, 0x00);
	SDL_SetAlpha(panel, 180);
	graphics.drawRect(1, 1, panel->w - 2, panel->h - 2 , graphics.black, graphics.white, panel);

	graphics.blit(panel, (640 - panel->w) / 2, (480 - panel->h) / 2, graphics.background, false);

	audio.loadSound(0, "sound/pop1");
	audio.loadSound(1, "sound/cheer");

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.updateScreen();

	int y = 520;
	int miaY = 335;
	int clearY = 520;
	Entity *mia = (Entity*)map.miaList.getHead();
	Sprite *teleportStar = graphics.getSprite("TeleportStar", true);
	char message[256];
	int col1 = 360;
	int col2 = 380;
	int count = 0;
	int place = 0;
	int nextStarBurst = 10 * (Math::prand() % 6);

	float px, py, dx, dy;


	while (mia->next != NULL)
	{
		mia = (Entity*)mia->next;

		if (mia->health > 0)
			continue;

		mia->x = 700;
		
		mia->y = miaY;
		
		count++;

		if (count > colCount)
		{
			miaY += 25;
			count = 0;
		}

		place += 25;
	}

	if (place > (colCount * 25))
		place = colCount * 25;

	place = (640 - place) / 2;

	graphics.setFontSize(1);

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	engine.flushInput();
	engine.clearInput();

	y = 130;
	
	if (map.totalMIAs > 0)
	{
		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		snprintf(message, sizeof message, _("Rescue %d MIAs"), map.requiredMIAs);
		graphics.drawString(message, col1, y, TXT_RIGHT, graphics.background);

		if (map.foundMIAs < map.requiredMIAs)
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			snprintf(message, sizeof message, "%d / %d", map.foundMIAs, map.requiredMIAs);
			graphics.drawString(message, col2, y, TXT_LEFT, graphics.background);
		}
		else
		{
			graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Completed"), col2, y, TXT_LEFT, graphics.background);
		}
	}

	Objective *objective = (Objective*)map.objectiveList.getHead();

	engine.setPlayerPosition(0, 0, -1, -1, -1, -1);

	while (objective->next != NULL)
	{
		objective = (Objective*)objective->next;

		y += 20;

		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		
		if ((game.skill < 3) &&  (strstr(objective->description, "L.R.T.S.")) && (!gameData.completedWorld))
		{
			graphics.drawString(_("???? ???????? ????"), col1, y, TXT_RIGHT, graphics.background);
		}
		else
		{
			graphics.drawString(_(objective->description), col1, y, TXT_RIGHT, graphics.background);
		}

		if (objective->currentValue < objective->targetValue)
		{
			if (objective->targetValue == 1)
			{
				graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
				graphics.drawString(_("Incomplete"), col2, y, TXT_LEFT, graphics.background);
			}
			else
			{
				graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
				snprintf(message, sizeof message, "%d / %d", objective->currentValue, objective->targetValue);
				graphics.drawString(message, col2, y, TXT_LEFT, graphics.background);
			}
		}
		else
		{
			graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Completed"), col2, y, TXT_LEFT, graphics.background);
		}
	}
	
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	unsigned int frameLimit = SDL_GetTicks() + 16;

	if (perfect)
		audio.playSound(1, 1);

	while (true)
	{
		engine.getInput();
		config.populate();
		graphics.updateScreen();
		graphics.animateSprites();

		if (engine.userAccepts())
			break;

		graphics.drawBackground();

		graphics.blit(clear, 320, clearY, graphics.screen, true);

		Math::limitInt(&(clearY -= 5), 70, 520);

		count = 0;

		mia = (Entity*)map.miaList.getHead();

		if (clearY == 70)
		{
			while (mia->next != NULL)
			{
				mia = (Entity*)mia->next;

				if (mia->health > 0)
					continue;

				if (count > colCount)
					count = 0;

				graphics.blit(mia->getFaceImage(), (int)mia->x, (int)mia->y, graphics.screen, false);

				if (mia->x == 700)
					audio.playSound(0, 0);

				if (mia->x > (place + (count * 25)))
					Math::limitFloat(&(mia->x -= 35), place + (count * 25), 640);

				if (mia->x > place + (count * 25))
					break;

				count++;
			}
			
			if (perfect)
			{
				Math::limitInt(&(--nextStarBurst), 0, 999);

				if (nextStarBurst == 0)
				{
					nextStarBurst = 10 * (Math::prand() % 6);

					px = Math::rrand(50, 585);
					py = Math::rrand(55, 85);

					for (int i = 0 ; i < 50 ; i++)
					{
						dx = Math::rrand(-30, 30); dx /= 20;
						dy = Math::rrand(-30, 30); dy /= 20;
						map.addParticle(px, py, dx, dy, Math::rrand(30, 60), graphics.red, teleportStar, PAR_WEIGHTLESS);
					}
				}

				doParticles();
			}
		}

		static Graphics::SurfaceCache cache;
		snprintf(message, sizeof message, "%s - %.2d:%.2d:%.2d", _("Mission Time"), game.currentMissionHours, game.currentMissionMinutes, game.currentMissionSeconds);
		graphics.drawString(message, 320, 420, true, graphics.screen, cache);

		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}

	audio.stopMusic();

	processPostMissionData();

	if (!engine.practice)
	{
		saveGame();
	}
}


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

#include "hub.h"

extern void doMusicInfo(unsigned int);

void createStatsPanel(int page)
{
	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	graphics.drawRect(1, 1, image->w - 2, image->h - 2, graphics.black, graphics.white, image);

	char string[100];
	int x1 = 80;
	int x2 = 270;
	int x3 = 300;
	int y = 15;

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	graphics.drawString(_("Metal Blob Solid : Statistics"), 200, y, true, image);
	y += 10;
	
	Sprite *arrows = graphics.getSprite("HubArrows", true);

	switch (page)
	{
		case 0:
			graphics.drawString(_("Skill Level:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%s", _(skill[game.skill]));
			graphics.drawString(string, x2, y, false, image);

			graphics.drawString(_("Score:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.score);
			graphics.drawString(string, x2, y, false, image);

			graphics.drawString(_("MIAs Saved:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.totalMIAsRescued);
			graphics.drawString(string, x2, y, false, image);

			graphics.drawString(_("Objectives Completed:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.totalObjectivesCompleted);
			graphics.drawString(string, x2, y, false, image);

			// Don't do this on Extreme or it will always say 100%
			if (game.skill < 3)
			{
				graphics.drawString(_("Percentage Complete:"), x1, y += 20, false, image);
				snprintf(string, sizeof string, "%d%%", gameData.getPercentageComplete());
				graphics.drawString(string, x2, y, false, image);
			}

			snprintf(string, sizeof string, "%s - %.2d:%.2d:%.2d", _("Total Game Time"), game.totalHours, game.totalMinutes, game.totalSeconds);
			graphics.drawString(string, 200, y += 110, true, image);

			break;

		case 1:

			graphics.drawString(_("Enemies Defeated:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.totalEnemiesDefeated);
			graphics.drawString(string, x2, y, false, image);

			graphics.drawString(_("Items Collected:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.totalItemsCollected);
			graphics.drawString(string, x2, y, false, image);

			graphics.drawString(_("Bonuses Picked Up:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.totalBonusesCollected);
			graphics.drawString(string, x2, y, false, image);

			graphics.drawString(_("Best Combo:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, _("%d Hits"), game.maxComboHits);
			graphics.drawString(string, x2, y, false, image);
			
			graphics.drawString(_("Missions Started:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.levelsStarted);
			graphics.drawString(string, x2, y, false, image);

			graphics.drawString(_("Continues Used:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.continuesUsed);
			graphics.drawString(string, x2, y, false, image);
			
			if ((game.continuesUsed > 0) && (game.levelsStarted > 0))
			{
				graphics.drawString(_("Average Continue Usage:"), x1, y += 20, false, image);
				snprintf(string, sizeof string, "%d", (game.continuesUsed / game.levelsStarted));
				graphics.drawString(string, x2, y, false, image);
			}

			graphics.drawString(_("Escapes Used:"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.escapes);
			graphics.drawString(string, x2, y, false, image);

			break;

		case 2:
		
			x1 = 20;
			x2 = 170;
			x3 = 300;

			graphics.drawString(_("Weapon"), x1, y += 20, false, image);
			graphics.drawString(_("Ammo Used"), x2, y, false, image);
			graphics.drawString(_("Accuracy"), x3, y, false, image);

			graphics.drawString(_("Pistol"), x1, y += 30, false, image);
			snprintf(string, sizeof string, "%d", game.bulletsFired[WP_PISTOL]);
			graphics.drawString(string, x2, y, false, image);
			snprintf(string, sizeof string, "%d%%", game.getWeaponAccuracy(WP_PISTOL));
			graphics.drawString(string, x3, y, false, image);

			graphics.drawString(_("Machine Gun"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.bulletsFired[WP_MACHINEGUN]);
			graphics.drawString(string, x2, y, false, image);
			snprintf(string, sizeof string, "%d%%", game.getWeaponAccuracy(WP_MACHINEGUN));
			graphics.drawString(string, x3, y, false, image);

			graphics.drawString(_("Laser Cannon"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.bulletsFired[WP_LASER]);
			graphics.drawString(string, x2, y, false, image);
			snprintf(string, sizeof string, "%d%%", game.getWeaponAccuracy(WP_LASER));
			graphics.drawString(string, x3, y, false, image);
			
			graphics.drawString(_("Grenades"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.bulletsFired[WP_GRENADES]);
			graphics.drawString(string, x2, y, false, image);
			snprintf(string, sizeof string, "%d%%", game.getWeaponAccuracy(WP_GRENADES));
			graphics.drawString(string, x3, y, false, image);

			graphics.drawString(_("Spread Gun"), x1, y += 20, false, image);
			snprintf(string, sizeof string, "%d", game.bulletsFired[WP_SPREAD]);
			graphics.drawString(string, x2, y, false, image);
			snprintf(string, sizeof string, "%d%%", game.getWeaponAccuracy(WP_SPREAD));
			graphics.drawString(string, x3, y, false, image);
			
			graphics.drawString(_("Total"), x1, y += 30, false, image);
			snprintf(string, sizeof string, "%d", game.getTotalBulletsFired());
			graphics.drawString(string, x2, y, false, image);
			snprintf(string, sizeof string, "%d%%", game.getTotalAccuracy());
			graphics.drawString(string, x3, y, false, image);

			snprintf(string, sizeof string, "%s - %s", _("Most Used Weapon"), _(weapon[game.getMostUsedWeapon()].name));
			graphics.drawString(string, 200, y += 50, true, image);

			break;
	}

	graphics.blit(arrows->image[0], 150, 260, image, false);
	graphics.blit(arrows->image[2], 200, 260, image, false);

	if (page == 0)
		graphics.blit(arrows->image[1], 150, 260, image, false);

	if (page == 2)
		graphics.blit(arrows->image[3], 200, 260, image, false);
}

void loadLevelBrief(const char *levelName)
{
	char briefLine[1024];

	if (!engine.loadData(_("data/levelBrief")))
	{
		graphics.showErrorAndExit("Couldn't load mission briefing file (%s)", _("data/levelBrief"));
	}

	char *line = strtok((char*)engine.dataBuffer, "\n");
	bool collectData = false;
	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	int i = 0 ;

	graphics.clearChatString();

	while (true)
	{
		if (collectData)
		{
			sscanf(line, "%[^\n\r]", briefLine);

			if (strcmp(briefLine, "@EOF@") == 0)
				break;

			if (line[0] == '[')
				break;

			graphics.createChatString(line);
		}
		else
		{
			if (line[0] == '[')
			{
				sscanf(line, "%*c %[^]]", briefLine);
				if (strcmp(briefLine, levelName) == 0)
					collectData = true;
			}
		}

		line = strtok(NULL, "\n");
		if (line == NULL)
			break;

		i++;

		// sanity check!
		if (i == 10000)
			break;
	}
	
	graphics.drawChatString(image, 40);
}

void createObjectivesPanel(const char *levelName)
{
	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	Data *data = (Data*)gameData.dataList.getHead();

	bool found = false;

	graphics.drawRect(1, 1, image->w - 2, image->h - 2, graphics.black, graphics.white, image);

	char string[100];
	char levelMIAKey[100];
	int x1 = 60;
	int x2 = 260;
	int y = 15;
	
	snprintf(levelMIAKey, sizeof levelMIAKey, "%s MIAs", levelName);

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	snprintf(string, sizeof string, _("Information for %s"), _(levelName));
	graphics.drawString(string, 200, y, true, image);
	y += 10;

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if ((strcmp(data->key, levelName) == 0) || (strstr(data->key, levelMIAKey)))
		{
			found = true;

			if (strstr(data->key, "MIAs"))
			{
				// if it says 0 / 0 don't bother...
				if (strcmp(data->value, "0 / 0"))
				{
					graphics.drawString(_("MIAs Rescued"), x1, y += 20, false, image);
					snprintf(string, sizeof string, "%d / %d", data->current, data->target);
					(data->isComplete()) ? graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00) : graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
					graphics.drawString(string, x2, y, false, image);
				}
			}
			else if (!strstr(data->value, "MIA_"))
			{
				if ((game.skill < 3) &&  (strstr(data->value, "L.R.T.S.")) && (!gameData.completedWorld))
				{
					graphics.drawString(_("???? ???????? ????"), x1, y += 20, false, image);
				}
				else
				{
					strlcpy(string, _(data->value), sizeof string);
					
					if (strlen(string) >= 25)
					{
						int cut_char = 25;
						// don't break unicode characters
						while (((string[cut_char] >> 6) & 3) == 2)
						{
							cut_char--;
						}

						string[cut_char++] = '.';
						string[cut_char++] = '.';
						string[cut_char++] = '.';
						string[cut_char] = '\0';
					}
					
					graphics.drawString(string, x1, y += 20, false, image);
				}
				
				if (data->target == 1)
				{
					snprintf(string, sizeof string, "%s", (data->isComplete()) ? _("Completed") : _("Incomplete"));
				}
				else
				{
					snprintf(string, sizeof string, "%d / %d", data->current, data->target);
				}
				
				(data->isComplete()) ? graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00) : graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
				graphics.drawString(string, x2, y, false, image);
			}
		}

		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	}

	if (!found)
		loadLevelBrief(levelName);
}

void createMIAPanel(int start, int max)
{
	char string[100];
	char name[100];
	int x1 = 20;
	int x2 = 130;
	int x3 = 310;
	int y = 15;

	int current = 0;
	int end = 0;

	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	Data *data = (Data*)gameData.dataList.getHead();

	graphics.drawRect(1, 1, image->w - 2, image->h - 2, graphics.black, graphics.white, image);

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	graphics.drawString(_("MIA Statistics"), 200, y, true, image);

	graphics.drawString(_("Name"), x1, y += 20, false, image);
	graphics.drawString(_("Location"), x2, y, false, image);
	graphics.drawString(_("Status"), x3, y, false, image);

	y += 10;

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (strstr(data->value, "MIA_"))
		{
			if (current < start)
			{
				current++;
				continue;
			}

			snprintf(name, sizeof name, "%s", data->value);
			strtok(name, "_");

			snprintf(string, sizeof string, "%s", strtok(NULL, "_"));
			graphics.drawString(string, x1, y += 20, false, image);

			graphics.drawString(_(data->key), x2, y, false, image);

			if (data->isComplete())
			{
				snprintf(string, sizeof string, "%s", _("Found"));
				graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			}
			else
			{
				snprintf(string, sizeof string, "%s", _("Missing"));
				graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			}

			graphics.drawString(string, x3, y, false, image);

			graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

			end++;

			if (end == 9)
				break;
		}
	}

	Sprite *arrows = graphics.getSprite("HubArrows", true);

	if (start > 0)
 		graphics.blit(arrows->image[0], 150, 260, image, false);
	else
		graphics.blit(arrows->image[1], 150, 260, image, false);

	if ((end == 9) && (current + 9 < max))
		graphics.blit(arrows->image[2], 200, 260, image, false);
	else
		graphics.blit(arrows->image[3], 200, 260, image, false);
}

bool requirementMet(const char *requiredLevel)
{
	if (strstr(requiredLevel, "Stages"))
	{
		int stages = 0;
		sscanf(requiredLevel, "%*s %d", &stages);

		if (game.stagesCleared >= stages)
		{
			presentPlayerMedal(requiredLevel);
			return true;
		}
	}
	
	if (engine.cheatLevels)
	{
		return true;
	}

	if (strcmp(requiredLevel, "@none@") == 0)
	{
		return true;
	}

	return gameData.requiredLevelCleared(requiredLevel);
}

int doHub()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(1000);
	
	graphics.free();
	audio.free();

	graphics.loadBackground("gfx/main/worldMap.jpg");
	audio.loadMusic("music/hub");
	
	// we might want this now, for medals
	audio.loadSound(SND_ITEM, "sound/item");
	
	graphics.quickSprite("cheatHeader", graphics.loadImage("gfx/main/cheats.png"));
	graphics.quickSprite("optionsHeader", graphics.loadImage("gfx/main/options.png"));
	graphics.quickSprite("keyHeader", graphics.loadImage("gfx/main/keyConfig.png"));
	graphics.quickSprite("joystickHeader", graphics.loadImage("gfx/main/joystickConfig.png"));
	graphics.quickSprite("optionsBackground", graphics.loadImage("gfx/main/optionsBackground.png"));

	char string[80];

	Sprite *cursor = graphics.addSprite("Cursor");
	for (int i = 0 ; i < 6 ; i++)
	{
		snprintf(string, sizeof string, "gfx/main/cursor%d.png", i + 1);
		cursor->setFrame(i, graphics.loadImage(string), 10);
	}

	Sprite *newTarget = graphics.addSprite("NewTarget");
	Sprite *visitedTarget = graphics.addSprite("VisitedTarget");
	
	for (int i = 0 ; i < 5 ; i++)
	{
		snprintf(string, sizeof string, "gfx/sprites/miaSignal%d.png", i + 1);
		
		newTarget->setFrame(i, graphics.loadImage(string, -60, 0, 0), 15);
		visitedTarget->setFrame(i, graphics.loadImage(string, 0, 0, 0), 15);
	}

	Sprite *hubIcons = graphics.addSprite("HubIcons");
	for (int i = 0 ; i < 6 ; i++)
	{
		snprintf(string, sizeof string, "gfx/main/hubIcon%d.png", i + 1);
		hubIcons->setFrame(i, graphics.loadImage(string), 60);
	}

	SDL_Surface *infoPanel = graphics.quickSprite("infoPanel", graphics.createSurface(400, 300));

	Sprite *hubArrows = graphics.addSprite("HubArrows");
	hubArrows->setFrame(0, graphics.loadImage("gfx/main/hubArrowLeft.png"), 60);
	hubArrows->setFrame(1, graphics.loadImage("gfx/main/hubArrowLeft2.png"), 60);
	hubArrows->setFrame(2, graphics.loadImage("gfx/main/hubArrowRight.png"), 60);
	hubArrows->setFrame(3, graphics.loadImage("gfx/main/hubArrowRight2.png"), 60);

	List hubList;
	HubLevel *hubPoint;

	engine.loadData("data/hub");

	char *line = strtok((char*)engine.dataBuffer, "\n");

	char name[50], level[50], requiredLevel[50];
	int x, y;

	#if DEBUG
	char pos[50];
	#endif

	graphics.setFontSize(0);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	
	int numberOfHubs = 0;
	
	gameData.calculateWorldCompleted();

	while (true)
	{
		sscanf(line, "%*c %[^\"] %*c %*c %[^\"] %*c %d %d %*c %[^\"] %*c", name, level, &x, &y, requiredLevel);

		if (strcmp(name, "@EOF@") == 0)
		{
			break;
		}
		
		// don't show boss levels on Easy (they might be cheating!)
		if (game.skill == 0)
		{
			if (strstr(name, "BioMech"))
			{
				line = strtok(NULL, "\n");
				continue;
			}
		}

		if (requirementMet(requiredLevel))
		{
			if (!gameData.levelPrefectlyCleared(name))
			{
				hubPoint = new HubLevel;
				hubPoint->set(name, level, x, y);
				hubPoint->levelNameImage = graphics.getString(_(name), false);
				
				(!gameData.stagePreviouslyCleared(name)) ? hubPoint->target = newTarget : hubPoint->target = visitedTarget;
				
				hubList.add(hubPoint);
				numberOfHubs++;
			}
		}

		line = strtok(NULL, "\n");
	}

	int rtn = -1;
	
	strlcpy(level, "@none@", sizeof level);
	strlcpy(game.stageName, "@none@", sizeof game.stageName);

	bool showData = false;
	bool showStats = false;
	bool showMIAs = false;
	
	int miaStart = 0;
	int miaMax = 0;
	int page = 0;
	
	int labelX, labelY;

	Data *data = (Data*)gameData.dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;

		if (strstr(data->value, "MIA_"))
		{
			miaMax++;
		}
	}
	
	bool validStage = false;
	
	if ((numberOfHubs == 0) && (gameData.completedWorld))
	{
		game.setMapName("data/spaceStation");
		strlcpy(game.stageName, "Space Station", sizeof game.stageName);
		createObjectivesPanel("Space Station");
		showData = true;
		showStats = showMIAs = false;
		validStage = true;
		
		// Wait wait wait... we're in easy mode. Game ends here.
		if (game.skill == 0)
		{
			hubList.clear();
			return SECTION_EASYOVER;
		}
		
		presentPlayerMedal("World_Complete");
	}
	
	audio.playMusic();

	int labelWidth = 0;
	Uint32 frameLimit = SDL_GetTicks() + 16;
	Uint32 now = SDL_GetTicks();
	
	while (rtn == -1)
	{
		graphics.updateScreen();
		graphics.drawBackground();
		graphics.animateSprites();

		if (validStage)
		{
			static Graphics::SurfaceCache cache;
			graphics.drawRect(10, 400, 620, 20, graphics.black, graphics.white, graphics.screen);
			snprintf(string, sizeof string, "%s : %s", _("Selected Destination"), _(game.stageName));
			graphics.drawString(string, 320, 409, true, graphics.screen, cache);
		}
		
		graphics.drawRect(10, 430, 620, 40, graphics.black, graphics.white, graphics.screen);

		#if DEBUG
		static Graphics::SurfaceCache posCache;
		snprintf(pos, sizeof pos, "%.3d:%.3d", engine.getMouseX(), engine.getMouseY());
		graphics.drawString(pos, 320, 15, true, graphics.screen, posCache);
		#endif

		engine.getInput();
		config.populate();
		
		int mouseXDelta = 0;
		int mouseYDelta = 0;

		if (config.isControl(CONTROL::RIGHT))
		{
			mouseXDelta += 5;
		}
		
		if (config.isControl(CONTROL::LEFT))
		{
			mouseXDelta -= 5;
		}
		
		if (config.isControl(CONTROL::DOWN))
		{
			mouseYDelta += 5;
		}
		
		if (config.isControl(CONTROL::UP) || config.isControl(CONTROL::JUMP))
		{
			mouseYDelta -= 5;
		}

		if ((mouseXDelta != 0) || (mouseYDelta != 0))
		{
			engine.moveMouse(mouseXDelta, mouseYDelta);
		}

		hubPoint = (HubLevel*)hubList.getHead();

		// Collisions for Hub Points
		while (hubPoint->next != NULL)
		{
			hubPoint = (HubLevel*)hubPoint->next;

			graphics.blit(hubPoint->target->getCurrentFrame(), hubPoint->x, hubPoint->y, graphics.screen, false);

			if (Collision::collision(engine.getMouseX(), engine.getMouseY(), 1, 1, hubPoint->x, hubPoint->y, 16, 16))
			{
				labelX = engine.getMouseX();
				labelY = engine.getMouseY() - 20;

				labelWidth = hubPoint->levelNameImage->w / 2;

				Math::limitInt(&labelX, 10 + labelWidth, 630 - labelWidth);

				graphics.blit(hubPoint->levelNameImage, labelX, labelY, graphics.screen, true);

				if (engine.mouseLeft || config.isControl(CONTROL::FIRE))
				{
					game.setMapName(hubPoint->filename);
					game.setStageName(hubPoint->stageName);
					validStage = true;
					engine.mouseLeft = 0;
					config.resetControl(CONTROL::FIRE);
				}

				if (engine.mouseRight || config.isControl(CONTROL::MAP))
				{
					game.setMapName(hubPoint->filename);
					game.setStageName(hubPoint->stageName);
					createObjectivesPanel(game.stageName);
					showData = true;
					showStats = false;
					engine.mouseRight = 0;
					config.resetControl(CONTROL::MAP);
				}
			}
		}

		// Collisions for Panel
		for (int i = ((1 - validStage) * 2) ; i < 6 ; i++)
		{
			graphics.blit(hubIcons->image[i], 50 + (i * 100), 440, graphics.screen, false);
			if (Collision::collision(engine.getMouseX(), engine.getMouseY(), 1, 1, 50 + (i * 100), 440, hubIcons->image[i]->w, hubIcons->image[i]->h))
			{
				if (engine.mouseLeft || config.isControl(CONTROL::FIRE))
				{
					switch (i)
					{
						case 0:
							rtn = SECTION_GAME;
							break;
						case 1:
							createObjectivesPanel(game.stageName);
							showData = !showData;
							showStats = showMIAs = false;
							break;
						case 2:
							createMIAPanel(miaStart, miaMax);
							showStats = showData = false;
							showMIAs = !showMIAs;
							break;
						case 3:
							createStatsPanel(page);
							showStats = !showStats;
							showData = showMIAs = false;
							break;
						case 4:
							showOptions();
							graphics.setFontSize(0);
							graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
							break;
						case 5:
							rtn = SECTION_TITLE;
							break;
					}

					engine.mouseLeft = false;
					config.resetControl(CONTROL::FIRE);
				}
			}
		}
		
		// Collisions for Panel Buttons
		if ((showMIAs) || (showStats))
		{
		    // SDL_SCANCODE_LCTRL is the "fire" button.
			if (engine.mouseLeft || config.isControl(CONTROL::FIRE))
			{
				if (Collision::collision(engine.getMouseX(), engine.getMouseY(), 1, 1, 270, 310, 32, 32))
				{
					if (showMIAs)
					{
						if (miaStart > 0)
						{
							Math::limitInt(&(miaStart -= 9), 0, miaMax);
							createMIAPanel(miaStart, miaMax);
						}
					}

					if (showStats)
					{
						Math::limitInt(&(--page), 0, 2);
						createStatsPanel(page);
					}

					engine.mouseLeft = 0;
					config.resetControl(CONTROL::FIRE);
				}


				if (Collision::collision(engine.getMouseX(), engine.getMouseY(), 1, 1, 320, 310, 32, 32))
				{
					if (showMIAs)
					{
						if (miaStart + 9 < miaMax)
						{
							Math::limitInt(&(miaStart += 9), 0, miaMax);
							createMIAPanel(miaStart, miaMax);
						}
					}

					if (showStats)
					{
						Math::limitInt(&(++page), 0, 2);
						createStatsPanel(page);
					}

					engine.mouseLeft = 0;
					config.resetControl(CONTROL::FIRE);
				}
			}
		}

		if ((showData) || (showStats) || (showMIAs))
		{
			graphics.blit(infoPanel, 320, 200, graphics.screen, true);
			if (engine.mouseLeft || config.isControl(CONTROL::FIRE))
			{
				showData = showStats = showMIAs = false;
			}
		}

		graphics.blit(cursor->getCurrentFrame(), engine.getMouseX(), engine.getMouseY(), graphics.screen, true);

		doMusicInfo(SDL_GetTicks() - (now + 60000));
		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}

	hubList.clear();

	return rtn;
}

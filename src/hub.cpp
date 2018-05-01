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

#include "headers.h"

static void createStatsPanel(int page)
{
	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	graphics.drawRect(1, 1, image->w - 2, image->h - 2, graphics.black, graphics.white, image);

	std::string message;
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
			message = fmt::format("{}", _(skill[game.skill]));
			graphics.drawString(message, x2, y, false, image);

			graphics.drawString(_("Score:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.score);
			graphics.drawString(message, x2, y, false, image);

			graphics.drawString(_("MIAs Saved:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.totalMIAsRescued);
			graphics.drawString(message, x2, y, false, image);

			graphics.drawString(_("Objectives Completed:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.totalObjectivesCompleted);
			graphics.drawString(message, x2, y, false, image);

			// Don't do this on Extreme or it will always say 100%
			if (game.skill < 3)
			{
				graphics.drawString(_("Percentage Complete:"), x1, y += 20, false, image);
				message = fmt::format("{}%", gameData.getPercentageComplete());
				graphics.drawString(message, x2, y, false, image);
			}

			message = fmt::format("{} - {:02d}:{:02d}:{:02d}", _("Total Game Time"), game.totalHours, game.totalMinutes, game.totalSeconds);
			graphics.drawString(message, 200, y += 110, true, image);

			break;

		case 1:

			graphics.drawString(_("Enemies Defeated:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.totalEnemiesDefeated);
			graphics.drawString(message, x2, y, false, image);

			graphics.drawString(_("Items Collected:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.totalItemsCollected);
			graphics.drawString(message, x2, y, false, image);

			graphics.drawString(_("Bonuses Picked Up:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.totalBonusesCollected);
			graphics.drawString(message, x2, y, false, image);

			graphics.drawString(_("Best Combo:"), x1, y += 20, false, image);
			message = fmt::format(_("{} Hits"), game.maxComboHits);
			graphics.drawString(message, x2, y, false, image);
			
			graphics.drawString(_("Missions Started:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.levelsStarted);
			graphics.drawString(message, x2, y, false, image);

			graphics.drawString(_("Continues Used:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.continuesUsed);
			graphics.drawString(message, x2, y, false, image);
			
			if ((game.continuesUsed > 0) && (game.levelsStarted > 0))
			{
				graphics.drawString(_("Average Continue Usage:"), x1, y += 20, false, image);
				message = fmt::format("{}", (game.continuesUsed / game.levelsStarted));
				graphics.drawString(message, x2, y, false, image);
			}

			graphics.drawString(_("Escapes Used:"), x1, y += 20, false, image);
			message = fmt::format("{}", game.escapes);
			graphics.drawString(message, x2, y, false, image);

			break;

		case 2:
		
			x1 = 20;
			x2 = 170;
			x3 = 300;

			graphics.drawString(_("Weapon"), x1, y += 20, false, image);
			graphics.drawString(_("Ammo Used"), x2, y, false, image);
			graphics.drawString(_("Accuracy"), x3, y, false, image);

			graphics.drawString(_("Pistol"), x1, y += 30, false, image);
			message = fmt::format("{}", game.bulletsFired[WP_PISTOL]);
			graphics.drawString(message, x2, y, false, image);
			message = fmt::format("{}%", game.getWeaponAccuracy(WP_PISTOL));
			graphics.drawString(message, x3, y, false, image);

			graphics.drawString(_("Machine Gun"), x1, y += 20, false, image);
			message = fmt::format("{}", game.bulletsFired[WP_MACHINEGUN]);
			graphics.drawString(message, x2, y, false, image);
			message = fmt::format("{}%", game.getWeaponAccuracy(WP_MACHINEGUN));
			graphics.drawString(message, x3, y, false, image);

			graphics.drawString(_("Laser Cannon"), x1, y += 20, false, image);
			message = fmt::format("{}", game.bulletsFired[WP_LASER]);
			graphics.drawString(message, x2, y, false, image);
			message = fmt::format("{}%", game.getWeaponAccuracy(WP_LASER));
			graphics.drawString(message, x3, y, false, image);
			
			graphics.drawString(_("Grenades"), x1, y += 20, false, image);
			message = fmt::format("{}", game.bulletsFired[WP_GRENADES]);
			graphics.drawString(message, x2, y, false, image);
			message = fmt::format("{}%", game.getWeaponAccuracy(WP_GRENADES));
			graphics.drawString(message, x3, y, false, image);

			graphics.drawString(_("Spread Gun"), x1, y += 20, false, image);
			message = fmt::format("{}", game.bulletsFired[WP_SPREAD]);
			graphics.drawString(message, x2, y, false, image);
			message = fmt::format("{}%", game.getWeaponAccuracy(WP_SPREAD));
			graphics.drawString(message, x3, y, false, image);
			
			graphics.drawString(_("Total"), x1, y += 30, false, image);
			message = fmt::format("{}", game.getTotalBulletsFired());
			graphics.drawString(message, x2, y, false, image);
			message = fmt::format("{}%", game.getTotalAccuracy());
			graphics.drawString(message, x3, y, false, image);

			message = fmt::format("{} - {}", _("Most Used Weapon"), _(weapon[game.getMostUsedWeapon()].name));
			graphics.drawString(message, 200, y += 50, true, image);

			break;
	}

	graphics.blit(arrows->image[0], 150, 260, image, false);
	graphics.blit(arrows->image[2], 200, 260, image, false);

	if (page == 0)
		graphics.blit(arrows->image[1], 150, 260, image, false);

	if (page == 2)
		graphics.blit(arrows->image[3], 200, 260, image, false);
}

static void loadLevelBrief(const std::string &levelName)
{
	if (!engine.loadData(_("data/levelBrief")))
	{
		graphics.showErrorAndExit("Couldn't load mission briefing file ({})", _("data/levelBrief"));
	}

	bool collectData = false;
	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	graphics.clearChatString();

	auto data_view = std::string_view(engine.dataBuffer.data(), engine.dataBuffer.size());

	for (auto line_view: split(data_view, '\n'))
	{
		if (collectData)
		{
			if (line_view == "@EOF@")
				break;

			if (line_view[0] == '[')
				break;

			graphics.createChatString(std::string(line_view));
		}
		else
		{
			if (line_view[0] == '[' && line_view.size() > 2)
			{
				if (line_view.substr(1, line_view.size() - 2) == levelName)
					collectData = true;
			}
		}
	}
	
	graphics.drawChatString(image, 40);
}

static void createObjectivesPanel(const std::string &levelName)
{
	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	bool found = false;

	graphics.drawRect(1, 1, image->w - 2, image->h - 2, graphics.black, graphics.white, image);

	std::string text;
	std::string levelMIAKey;
	int x1 = 60;
	int x2 = 260;
	int y = 15;
	
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	text = fmt::format(_("Information for {}"), _(levelName));
	graphics.drawString(text, 200, y, true, image);
	y += 10;

	{
		int current;
		int target;

		gameData.getMIARescueCount(levelName, &current, &target);

		if (target > 0) {
			graphics.drawString(_("MIAs Rescued"), x1, y += 20, false, image);
			text = fmt::format("{} / {}", current, target);
			(current == target) ? graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00) : graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(text, x2, y, false, image);
		}
	}

	for (auto &&subobjective: gameData.objectives[levelName])
	{
		auto &[name, completion] = subobjective;

		if ((game.skill < 3) && contains(name, "L.R.T.S.") && (!gameData.completedWorld))
		{
			graphics.drawString(_("???? ???????? ????"), x1, y += 20, false, image);
		}
		else
		{
			text = _(name);
			
			if (text.size() >= 25)
			{
				int cut_char = 25;
				// don't break unicode characters
				while (((text[cut_char] >> 6) & 3) == 2)
				{
					cut_char--;
				}

				text.resize(cut_char);
				text.append("...");
			}
			
			graphics.drawString(text, x1, y += 20, false, image);
		}
		
		if (completion.target == 1)
		{
			text = (completion.isComplete()) ? _("Completed") : _("Incomplete");;
		}
		else
		{
			text = fmt::format("{} / {}", completion.current, completion.target);
		}
		
		(completion.isComplete()) ? graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00) : graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
		graphics.drawString(text, x2, y, false, image);

		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	}

	if (!found)
		loadLevelBrief(levelName);
}

static void createMIAPanel(int start, int max)
{
	int x1 = 20;
	int x2 = 130;
	int x3 = 310;
	int y = 15;

	int current = 0;
	int end = 0;

	SDL_Surface *image = graphics.getSprite("infoPanel", true)->image[0];

	graphics.drawRect(1, 1, image->w - 2, image->h - 2, graphics.black, graphics.white, image);

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	graphics.drawString(_("MIA Statistics"), 200, y, true, image);

	graphics.drawString(_("Name"), x1, y += 20, false, image);
	graphics.drawString(_("Location"), x2, y, false, image);
	graphics.drawString(_("Status"), x3, y, false, image);

	y += 10;

	for (auto &&objective: gameData.objectives)
	{
		auto levelName = objective.first;

		for (auto &&subobjective: objective.second)
		{
			auto &[name, completion] = subobjective;

			if (!contains(name, "MIA_"))
				continue;

			if (current < start)
			{
				current++;
				continue;
			}

			std::string text = name.substr(4);

			graphics.drawString(text, x1, y += 20, false, image);

			graphics.drawString(_(levelName), x2, y, false, image);

			if (completion.isComplete())
			{
				text = _("Found");
				graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			}
			else
			{
				text = _("Missing");
				graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			}

			graphics.drawString(text, x3, y, false, image);

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

static bool requirementMet(const std::string &requiredLevel)
{
	if (contains(requiredLevel, "Stages"))
	{
		int stages = 0;
		scan(requiredLevel, "%*s %d", &stages);

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

	if (requiredLevel == "@none@")
	{
		return true;
	}

	return gameData.requiredLevelCleared(requiredLevel);
}

int doHub()
{
	SDL_FillRect(graphics.screen, nullptr, graphics.black);
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

	std::string filename;

	auto &cursor = graphics.addSprite("Cursor");
	for (int i = 0 ; i < 6 ; i++)
	{
		filename = fmt::format("gfx/main/cursor{}.png", i + 1);
		cursor.setFrame(i, graphics.loadImage(filename), 10);
	}

	auto &newTarget = graphics.addSprite("NewTarget");
	auto &visitedTarget = graphics.addSprite("VisitedTarget");
	
	for (int i = 0 ; i < 5 ; i++)
	{
		filename = fmt::format("gfx/sprites/miaSignal{}.png", i + 1);
		
		newTarget.setFrame(i, graphics.loadImage(filename, -60, 0, 0), 15);
		visitedTarget.setFrame(i, graphics.loadImage(filename, 0, 0, 0), 15);
	}

	auto &hubIcons = graphics.addSprite("HubIcons");
	for (int i = 0 ; i < 6 ; i++)
	{
		filename = fmt::format("gfx/main/hubIcon{}.png", i + 1);
		hubIcons.setFrame(i, graphics.loadImage(filename), 60);
	}

	SDL_Surface *infoPanel = graphics.quickSprite("infoPanel", graphics.createSurface(400, 300));

	auto &hubArrows = graphics.addSprite("HubArrows");
	hubArrows.setFrame(0, graphics.loadImage("gfx/main/hubArrowLeft.png"), 60);
	hubArrows.setFrame(1, graphics.loadImage("gfx/main/hubArrowLeft2.png"), 60);
	hubArrows.setFrame(2, graphics.loadImage("gfx/main/hubArrowRight.png"), 60);
	hubArrows.setFrame(3, graphics.loadImage("gfx/main/hubArrowRight2.png"), 60);

	std::vector<HubLevel> hubs;

	engine.loadData("data/hub");

	graphics.setFontSize(0);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	
	gameData.calculateWorldCompleted();

	for (auto line: split(engine.dataBuffer, '\n'))
	{
		char name[50], level[50], requiredLevel[50];
		int x, y;
		scan(line, "%*c %[^\"] %*c %*c %[^\"] %*c %d %d %*c %[^\"] %*c", name, level, &x, &y, requiredLevel);

		if (!strcmp(name, "@EOF@"))
		{
			break;
		}
		
		// don't show boss levels on Easy (they might be cheating!)
		if (game.skill == 0)
		{
			if (strstr(name, "BioMech"))
			{
				continue;
			}
		}

		if (requirementMet(requiredLevel))
		{
			if (!gameData.levelPrefectlyCleared(name))
			{
				auto &hubPoint = hubs.emplace_back(name, level, x, y);

				hubPoint.levelNameImage = graphics.getString(_(name), false);
				hubPoint.target = !gameData.stagePreviouslyCleared(name) ? &newTarget : &visitedTarget;
			}
		}
	}

	int rtn = -1;
	
	game.stageName = "@none@";

	bool showData = false;
	bool showStats = false;
	bool showMIAs = false;
	
	int miaStart = 0;
	int miaMax = gameData.countMIAs();
	int page = 0;
	
	int labelX, labelY;

	bool validStage = false;
	
	if ((hubs.empty()) && (gameData.completedWorld))
	{
		game.setMapName("data/spaceStation");
		game.stageName = "Space Station";
		createObjectivesPanel("Space Station");
		showData = true;
		showStats = showMIAs = false;
		validStage = true;
		
		// Wait wait wait... we're in easy mode. Game ends here.
		if (game.skill == 0)
		{
			hubs.clear();
			return SECTION_EASYOVER;
		}
		
		presentPlayerMedal("World_Complete");
	}
	
	audio.playMusic();

	int labelWidth = 0;
	uint32_t frameLimit = SDL_GetTicks() + 16;
	uint32_t now = SDL_GetTicks();
	
	while (rtn == -1)
	{
		graphics.updateScreen();
		graphics.drawBackground();
		graphics.animateSprites();

		if (validStage)
		{
			static Graphics::SurfaceCache cache;
			graphics.drawRect(10, 400, 620, 20, graphics.black, graphics.white, graphics.screen);
			std::string message = fmt::format("{} : {}", _("Selected Destination"), _(game.stageName));
			graphics.drawString(message, 320, 409, true, graphics.screen, cache);
		}
		
		graphics.drawRect(10, 430, 620, 40, graphics.black, graphics.white, graphics.screen);

		#if DEBUG
		static Graphics::SurfaceCache posCache;
		std::string pos = fmt::format("{:03d}:{:03d}", engine.getMouseX(), engine.getMouseY());
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

		// Collisions for Hub Points
		for (auto &&hubPoint: hubs)
		{
			graphics.blit(hubPoint.target->getCurrentFrame(), hubPoint.x, hubPoint.y, graphics.screen, false);

			if (Collision::collision(engine.getMouseX(), engine.getMouseY(), 1, 1, hubPoint.x, hubPoint.y, 16, 16))
			{
				labelX = engine.getMouseX();
				labelY = engine.getMouseY() - 20;

				labelWidth = hubPoint.levelNameImage->w / 2;

				Math::limitInt(&labelX, 10 + labelWidth, 630 - labelWidth);

				graphics.blit(hubPoint.levelNameImage, labelX, labelY, graphics.screen, true);

				if (engine.mouseLeft || config.isControl(CONTROL::FIRE))
				{
					game.setMapName(hubPoint.filename);
					game.setStageName(hubPoint.stageName);
					validStage = true;
					engine.mouseLeft = 0;
					config.resetControl(CONTROL::FIRE);
				}

				if (engine.mouseRight || config.isControl(CONTROL::MAP))
				{
					game.setMapName(hubPoint.filename);
					game.setStageName(hubPoint.stageName);
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
			graphics.blit(hubIcons.image[i], 50 + (i * 100), 440, graphics.screen, false);
			if (Collision::collision(engine.getMouseX(), engine.getMouseY(), 1, 1, 50 + (i * 100), 440, hubIcons.image[i]->w, hubIcons.image[i]->h))
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

		graphics.blit(cursor.getCurrentFrame(), engine.getMouseX(), engine.getMouseY(), graphics.screen, true);

		doMusicInfo(SDL_GetTicks() - (now + 60000));
		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}

	hubs.clear();

	return rtn;
}

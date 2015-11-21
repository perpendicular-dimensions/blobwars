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

#include "title.h"

extern void doMusicInfo(unsigned int);

/**
* Displays the skill level widgets and hide the others
*/
void showSkillLevels()
{
	engine.showWidgetGroup("skill", true);
	engine.showWidgetGroup("mainMenu", false);
	engine.showWidgetGroup("gameSlots", false);

	engine.showWidget("back", true);

	engine.highlightWidget("normal");
		  
	engine.enableWidget("extreme", engine.extremeAvailable);
}

/**
* Displays the save game widgets and hide the others
*/
void showGameSlots()
{
	engine.showWidgetGroup("gameSlots", true);
	engine.showWidgetGroup("mainMenu", false);

	engine.showWidget("back", true);

	engine.highlightWidget("slot1");
}

void showSaves()
{
	engine.showWidgetGroup("saveGames", true);
	engine.showWidgetGroup("mainMenu", false);

	engine.showWidget("back", true);

	engine.highlightWidget("save1");
}

/**
* Displays the manual widgets and hide the others
*/
void showManualHelp()
{
	#ifdef FRAMEWORK_SDL
	openHelpURL();
	#else
	engine.showWidgetGroup("help", true);
	engine.showWidgetGroup("mainMenu", false);
	
	engine.showWidget("back", true);

	engine.highlightWidget("back");
	#endif
}

/**
* Displays the title widgets and hide the others.
* Also disables Continue and Load Game if games are not
* available to be loaded.
*/
void showTitleWidgets()
{
	engine.showWidgetGroup("skill", false);
	engine.showWidgetGroup("saveGames", false);
	engine.showWidgetGroup("gameSlots", false);
	engine.showWidgetGroup("help", false);
	engine.showWidget("back", false);
	
	engine.showWidgetGroup("mainMenu", true);
	
	engine.highlightWidget("newGame");

	if (!engine.continueSaveSlot)
	{
		engine.enableWidget("loadGame", false);
		engine.enableWidget("continue", false);
	}
	else
	{
		engine.highlightWidget("continue");
	}
}

/**
* Creates the data labels for the save game slots
* according to the save game data available
*/
void setupSaveWidgets()
{
	char widgetName[10];
	widgetName[0] = 0;
	
	for (int i = 0 ; i < 5 ; i++)
	{
		snprintf(widgetName, sizeof widgetName, "save%d", i + 1);
		strlcpy(engine.getWidgetByName(widgetName)->label, engine.saveSlot[i], sizeof engine.getWidgetByName(widgetName)->label);
		
		if ((strstr(engine.saveSlot[i], _("Empty"))) || (strstr(engine.saveSlot[i], _("Corrupt"))))
		{
			engine.enableWidget(widgetName, false);
		}
		
		snprintf(widgetName, sizeof widgetName, "slot%d", i + 1);
		strlcpy(engine.getWidgetByName(widgetName)->label, engine.saveSlot[i], sizeof engine.getWidgetByName(widgetName)->label);
	}
}

/**
* Loads the title widgets
*/
void loadTitleWidgets()
{
	if (!engine.loadWidgets(_("data/titleWidgets")))
	{
		graphics.showErrorAndExit(ERR_FILE, _("data/titleWidgets"));
	}

	initSaveSlots();

	setupSaveWidgets();
	
	Widget *widget = engine.getWidgetByName("labelManual");
	strlcpy(widget->label, GAMEPLAYMANUAL, sizeof widget->label);

	showTitleWidgets();
}

/**
* Does the main game title sequence
* @return The section to proceed to (see main.cpp)
*/
int title()
{
	audio.free();
	graphics.free();

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(1000);

	newGame();

	loadTitleWidgets();

	float backAlpha = 0;
	float titleAlpha = 0;
	bool allFadedOn = false;

	graphics.quickSprite("cheatHeader", graphics.loadImage("gfx/main/cheats.png"));
	graphics.quickSprite("optionsHeader", graphics.loadImage("gfx/main/options.png"));
	graphics.quickSprite("keyHeader", graphics.loadImage("gfx/main/keyConfig.png"));
	graphics.quickSprite("joystickHeader", graphics.loadImage("gfx/main/joystickConfig.png"));
	graphics.quickSprite("optionsBackground", graphics.loadImage("gfx/main/optionsBackground.png"));

	SDL_Surface *prlogo = graphics.quickSprite("PRLogo", graphics.loadImage("gfx/main/prlogo.gif"));
	SDL_Surface *title = graphics.quickSprite("Title", graphics.loadImage("gfx/main/title.png"));
	
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	graphics.setFontSize(3);
	SDL_Surface *presents = graphics.quickSprite("Presents", graphics.getString(_("Presents"), true));
	SDL_Surface *sdl = graphics.quickSprite("SDL", graphics.getString(_("An SDL2 Game"), true));

	graphics.setFontSize(2);
	SDL_Surface *subTitle = graphics.quickSprite("SubTitle", graphics.getString(_("Blob Wars : Episode I"), true));

	graphics.setFontSize(0);
	SDL_Surface *copyright[] = {
		graphics.quickSprite("Copyright1", graphics.getString(_("Copyright (C) 2011-2015 Perpendicular Dimensions"), true)),
		graphics.quickSprite("Copyright2", graphics.getString(_("Copyright (C) 2004-2011 Parallel Realities"), true)),
		NULL,
	};

	char v[50];
	#define STRINGIFY_VALUE(x) STRINGIFY(x)
	#define STRINGIFY(x) #x
	snprintf(v, sizeof v, _("Version %s"), STRINGIFY_VALUE(VERSION));
	SDL_Surface *version = graphics.quickSprite("Version", graphics.getString(v, true));

	SDL_SetAlpha(title, 0);

	audio.loadMusic("music/title");

	graphics.loadBackground("gfx/main/CircuitBoard.jpg");
	SDL_SetAlpha(graphics.background, 0);

	int startNewGame, options, quit, help, easy, normal, hard, extreme, back, practice;
	int load, cont, save[5], slot[6];
	int autoSaveSlot;

	startNewGame = options = quit = easy = normal = hard = extreme = back = help = practice = 0;
	load = cont = save[0] = save[1] = save[2] = save[3] = save[4] = 0;
	slot[0] = slot[1] = slot[2] = slot[3] = slot[4] = slot[5] = 0;
	autoSaveSlot = 0;

	engine.setWidgetVariable("newGame", &startNewGame);
	engine.setWidgetVariable("loadGame", &load);
	engine.setWidgetVariable("continue", &cont);
	engine.setWidgetVariable("options", &options);
	engine.setWidgetVariable("help", &help);
	engine.setWidgetVariable("quit", &quit);

	float offX = 0;

	engine.flushInput();
	engine.clearInput();

	audio.playMusic();

	Uint32 now = SDL_GetTicks();

	unsigned int frameLimit = SDL_GetTicks() + 16;

	while (true)
	{
		unsigned int ticks = SDL_GetTicks();
		graphics.updateScreen();

		engine.getInput();
		config.populate();

		for (int x = (int)offX ; x < 640 ; x += graphics.background->w)
		{
			for (int y = 0 ; y < 480 ; y += graphics.background->h)
			{
				graphics.blit(graphics.background, x, y, graphics.screen, false);
			}
		}

		if (!allFadedOn)
		{
			if ((ticks >= now + 2000) && (ticks <= now + 10000))
			{
				graphics.blit(prlogo, 320, 240, graphics.screen, true);
			}
			else if ((ticks >= now + 13000) && (ticks <= now + 19000))
			{
				graphics.blit(presents, 320, 240, graphics.screen, true);
			}
			else if ((ticks >= now + 22000) && (ticks <= now + 27000))
			{
				graphics.blit(sdl, 320, 240, graphics.screen, true);
			}

			if (engine.userAccepts())
			{
				allFadedOn = true;
				engine.flushInput();
				engine.clearInput();
				backAlpha = 255;
				titleAlpha = 255;
				SDL_SetAlpha(graphics.background, backAlpha);
				SDL_SetAlpha(title, titleAlpha);
			}
		}

		if ((ticks >= now + 4000) || (allFadedOn))
		{
			if (backAlpha < 255)
			{
				backAlpha++;
				SDL_SetAlpha(graphics.background, backAlpha);
			}
		}

		if ((ticks >= now + 29000) || (allFadedOn))
		{
			if (titleAlpha < 255)
			{
				titleAlpha++;
				SDL_SetAlpha(title, titleAlpha);
			}
			else
			{
				graphics.blit(subTitle, 320, 180, graphics.screen, true);
				for (int i = 0; copyright[i]; i++)
					graphics.blit(copyright[i], 10, 460 - i * 18, graphics.screen, false);
				graphics.blit(version, (630 - version->w), 460, graphics.screen, false);
				allFadedOn = true;
			}

			doMusicInfo(ticks - (now + 39000));
		}

		Math::wrapFloat(&(offX -= 0.25), -graphics.background->w, 0);

		if (backAlpha > 0)
		{
			graphics.blit(title, 320, 100, graphics.screen, true);
		}

		if (allFadedOn)
		{
			drawWidgets();
			audio.playMenuSound(engine.processWidgets());
		}

		if (options)
		{
			showOptions();
			loadTitleWidgets();
			engine.setWidgetVariable("newGame", &startNewGame);
			engine.setWidgetVariable("loadGame", &load);
			engine.setWidgetVariable("continue", &cont);
			engine.setWidgetVariable("options", &options);
			engine.setWidgetVariable("help", &help);
			engine.setWidgetVariable("quit", &quit);
			options = 0;
			engine.flushInput();
		}

		if (help)
		{
			showManualHelp();
			engine.setWidgetVariable("back", &back);
			help = 0;
		}

		if (startNewGame)
		{
			showGameSlots();
			engine.setWidgetVariable("slot1", &slot[0]);
			engine.setWidgetVariable("slot2", &slot[1]);
			engine.setWidgetVariable("slot3", &slot[2]);
			engine.setWidgetVariable("slot4", &slot[3]);
			engine.setWidgetVariable("slot5", &slot[4]);
			engine.setWidgetVariable("slot6", &slot[5]);
			engine.setWidgetVariable("back", &back);
			startNewGame = 0;
		}

		for (int i = 0 ; i < 6 ; i++)
		{
			if (slot[i])
			{
				showSkillLevels();
				engine.setWidgetVariable("practice", &practice);
				engine.setWidgetVariable("easy", &easy);
				engine.setWidgetVariable("normal", &normal);
				engine.setWidgetVariable("hard", &hard);
				engine.setWidgetVariable("extreme", &extreme);
				engine.setWidgetVariable("back", &back);
				autoSaveSlot = i;
				slot[i] = 0;
			}
		}

		if (load)
		{
			showSaves();
			engine.setWidgetVariable("save1", &save[0]);
			engine.setWidgetVariable("save2", &save[1]);
			engine.setWidgetVariable("save3", &save[2]);
			engine.setWidgetVariable("save4", &save[3]);
			engine.setWidgetVariable("save5", &save[4]);
			engine.setWidgetVariable("back", &back);
			load = 0;
		}

		if (easy) {engine.skill = 0; break;}
		if (normal) {engine.skill = 1; break;}
		if (hard) {engine.skill = 2; break;}
		if (extreme) {engine.skill = 3; break;}
		if (practice) {engine.skill = -1; break;}

		if (cont) {load = engine.continueSaveSlot; break;}

		if (save[0]) {load = 1; break;}
		if (save[1]) {load = 2; break;}
		if (save[2]) {load = 3; break;}
		if (save[3]) {load = 4; break;}
		if (save[4]) {load = 5; break;}

		if (engine.keyState[SDL_SCANCODE_ESCAPE])
		{
			audio.playMenuSound(2);
			back = 1;
			engine.clearInput();
			engine.flushInput();
		}

		if (back)
		{
			showTitleWidgets();
			back = 0;
		}

		if (quit)
		{
			break;
		}

		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}

	engine.deleteWidgets();

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.updateScreen();
	audio.stopMusic();
	
	if (quit)
	{
		doQuit();
		exit(0);
	}
	
	newGame();
	
	(autoSaveSlot < 5) ? game.autoSaveSlot = autoSaveSlot : game.autoSave = false;

	if (load)
	{
		loadGame(load - 1);
		
		debug(("Map Name = %s", game.mapName))
	}
	else if (engine.skill == -1)
	{
		game.setMapName("data/practice");
		game.setStageName("Training Mission");
		engine.practice = true;
		game.skill = 1;
		return SECTION_GAME;
	}
	else if (engine.skill == 3)
	{
		game.skill = 3;
		game.hasAquaLung = game.hasJetPack = true;
	}
	
	if (strcmp(game.mapName, "data/spaceStation") == 0)
	{
		game.setMapName("data/finalBattle");
		game.setStageName("Final Battle");
		return SECTION_GAME;
	}

	return SECTION_HUB;
}

/**
* Loads and shows the credits at the end of the game.
*/
void doCredits()
{
	map.clear();

	char *line;
	char string[100];
	int i = 0;
	int numberOfCredits = 0;
	int pos1 = 0, pos2 = 0, size = 0;
	float *y, deviceY = 0;

	SDL_Surface *backdrop = graphics.quickSprite("CreditsBackGround", graphics.loadImage("gfx/main/creditsBack.png"));

	engine.loadData(_("data/credits"));

	line = strtok((char*)engine.dataBuffer, "\n");

	sscanf(line, "%d", &numberOfCredits);

	y = new float[numberOfCredits];
	 SDL_Surface **credit = new SDL_Surface*[numberOfCredits];

	line = strtok(NULL, "\n");

	pos1 = 520;

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	while (line)
	{
		sscanf(line, "%d %d %[^\n\r]", &pos2, &size, string);

		pos1 += pos2;

		y[i] = pos1;

		if (pos2 == 220)
			deviceY = pos1 - 50;

		graphics.setFontSize(size);
		credit[i] = graphics.quickSprite("credit", graphics.getString(string, true));

		i++;

		if (i == numberOfCredits)
		{
			break;
		}

		line = strtok(NULL, "\n");
	}
	
	SDL_Surface *device = graphics.quickSprite("credit", graphics.loadImage("gfx/main/creditsDevice.png"));

	if (!deviceY)
		deviceY = y[numberOfCredits - 7] - 50;

	audio.loadMusic("music/credits");
	audio.playMusic();

	engine.resetTimeDifference();
	Uint32 now = SDL_GetTicks();

	while (y[numberOfCredits - 1] > 350)
	{
		unsigned int frameLimit = SDL_GetTicks() + 16;

		graphics.updateScreen();
		engine.getInput();
		config.populate();

		engine.doTimeDifference();

		SDL_FillRect(graphics.screen, NULL, graphics.black);
		graphics.blit(backdrop, 0, 365, graphics.screen, false);

		float speed = 0.25;

		if (engine.keyState[SDL_SCANCODE_DOWN] || engine.joyY > 25000)
			speed = 1.0;
		else if (engine.keyState[SDL_SCANCODE_UP] || engine.joyY < -25000)
			speed = -1.0;
		
		deviceY -= (speed* engine.getTimeDifference());
		
		if ((deviceY > 10) && (deviceY < 470))
		{
			graphics.blit(device, 320, (int)deviceY, graphics.screen, true);
		}

		for (i = 0 ; i < numberOfCredits ; i++)
		{
			y[i] -= (speed * engine.getTimeDifference());
			
			if ((y[i] > 10) && (y[i] < 470))
			{
				graphics.blit(credit[i], 320, (int)y[i], graphics.screen, true);
			}

			graphics.drawRect(0, 450, 640, 30, graphics.black, graphics.screen);
			graphics.drawRect(0, 0, 640, 30, graphics.black, graphics.screen);
		}

		doMusicInfo(SDL_GetTicks() - (now + 10000));

		engine.delay(frameLimit);
	}

	graphics.delay(12000);
	audio.fadeMusic();
	graphics.fadeToBlack();

	delete[] y;
	delete[] credit;
}

/**
* Shows the exit screen, mentioning the sequel and the book by Stephen Sweeney
*/
void doQuit()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	SDL_Surface *sequel = graphics.loadImage("gfx/main/sequel.png");
	SDL_Surface *book = graphics.loadImage("gfx/main/book.png");

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	graphics.setFontSize(1);
	graphics.drawString(_("If you like Blob Wars: Metal Blob Solid, you might also like:"), 320, 20, true, graphics.screen);
	graphics.blit(sequel, 160, 200, graphics.screen, true);
	graphics.blit(book, 480, 200, graphics.screen, true);
	graphics.setFontSize(0);
	graphics.drawString("http://blobandconquer.sf.net", 160, 380, true, graphics.screen);
	graphics.drawString("http://www.battleforthesolarsystem.com", 480, 380, true, graphics.screen);
	graphics.setFontSize(3);
	graphics.drawString(_("Thank you for playing Blob Wars!"), 320, 430, true, graphics.screen);
	graphics.setFontSize(0);
	graphics.drawString(_("Press Space to Exit."), 320, 460, true, graphics.screen);

	graphics.updateScreen();

	do {
		SDL_Delay(16);
		engine.getInput();
	} while(!engine.userAccepts());
}

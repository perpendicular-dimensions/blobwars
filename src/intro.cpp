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

static void playIntro(split &it, int tx, int ty, int delay)
{
	unsigned int frameLimit = SDL_GetTicks() + 16;
	unsigned int time = 0;

	graphics.setFontSize(1);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	std::string line[3];
	SDL_Surface *text[3];

	for (int i = 0 ; i < 3 ; i++)
	{
		line[i] = *it;
		++it;

		if (line[i].empty())
		{
			return graphics.showErrorAndExit("Malformed Intro Data", "");
		}

		text[i] = NULL;

		if (line[i] != "@none@")
		{
			text[i] = graphics.getString(line[i], true);
		}
	}

	player.tx += tx;
	player.ty += ty;

	while (true)
	{
		engine.setPlayerPosition((int)player.x, (int)player.y, map.limitLeft, map.limitRight, map.limitUp, map.limitDown);

		doGameStuff();
		doSpawnPoints();
		drawMapTopLayer();

		for (int i = 0 ; i < 3 ; i++)
			if (text[i] != NULL)
				graphics.blit(text[i], 320, 150 + (i * 30), graphics.screen, true);

		if (engine.userAccepts())
			break;

		if (player.x < player.tx) player.x++;
		if (player.x > player.tx) player.x--;

		if ((int)player.x == player.tx)
		{
			if (time == 0)
			{
				time = SDL_GetTicks() + (delay * 1000);
			}
		}

		if (time > 0)
			if (SDL_GetTicks() > time)
				break;

		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}
	
	for (int i = 0 ; i < 3 ; i++)
		if (text[i])
			SDL_FreeSurface(text[i]);
}

static void showIntroError()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);

	engine.flushInput();

	graphics.setFontSize(1);

	graphics.drawString("Couldn't play intro - Data file is missing.", 320, 150, true, graphics.screen);
	graphics.drawString("This is not a fatal error, but could mean that the intro", 320, 180, true, graphics.screen);
	graphics.drawString("file has not been found or was not in the expected format.", 320, 210, true, graphics.screen);
	graphics.drawString("However it may also be a sign that the game may not work correctly.", 320, 240, true, graphics.screen);
	graphics.drawString("Press Escape to Continue", 320, 350, true, graphics.screen);

	while (true)
	{
		graphics.updateScreen();
		engine.getInput();
		config.populate();
		if (engine.keyState[SDL_SCANCODE_ESCAPE])
			break;
		SDL_Delay(16);
	}
}

static void parseIntroCommand(std::string_view line)
{
	char command[25], param[25];

	scan(line, "%s %s", command, param);

	if (!strcmp(command, "SPAWN"))
	{
		for (auto &&sp: map.spawns)
		{
			if (param == sp->name)
			{
				sp->active = true;
			}
		}
	}
}

int doIntro()
{
	int x, y, delay;

	game.setMapName("data/introMap");

	loadResources();

	for (int i = 0 ; i < 4 ; i++)
	{
		addItem(101, "CherryPlant", Math::prand() % 640, 9050, "CherryPlant", 100, 1, 0, false);
	}

	if (!engine.loadData(_("data/introText")))
	{
		showIntroError();
		return SECTION_TITLE;
	}
	
	player.health = -100;

	audio.playMusic();

	auto lines = split(engine.dataBuffer, '\n');
	++lines;

	for (auto it = lines.begin(); it != lines.end(); ++it)
	{
		if (scan(*it, "%d %d %d", &x, &y, &delay) != 3)
			abort();

		if (delay == -1)
			break;

		parseIntroCommand(*++it);

		playIntro(it, x, y, delay);

		if (engine.userAccepts())
			break;
	}

	if (!engine.userAccepts())
	{
		audio.fadeMusic();
		graphics.fadeToBlack();
	}

	map.clear();

	return SECTION_TITLE;
}

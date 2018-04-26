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

static List sceneList;

static void createSceneList(split &it)
{
	Cutscene *scene = NULL;
	
	for (; it != it.end(); ++it)
	{
		auto line = *it;
		
		if (line == "@EOF@")
			break;
	
		if (line[0] == '[')
			break;	
		
		if (line == "END")
			break;
		
		if (line == "NEW")
		{
			scene = new Cutscene();
			sceneList.add(scene);
			
			// Assume graphics is first line after new
			line = *++it;
			if (line != "@none@")
			{
				scene->sprite = line;
				debug(("Loading cutscene image %s\n", scene->sprite));
				graphics.quickSprite(scene->sprite, graphics.loadImage(scene->sprite));
			}
			line = *++it;
			scene->waitTime = (stoi(line) * 100);
		}
		
		if (scene && line != "@none@")
		{
			scene->appendText(line);
		}
	}
}

static bool setupScene(const std::string &stagename)
{
	sceneList.clear();
	
	if (!engine.loadData(_("data/ending")))
		return graphics.showErrorAndExit("Couldn't load cutscene data file (%s)", _("data/ending")), false;

	graphics.clearChatString();
	
	std::string_view sv(&engine.dataBuffer.at(0), engine.dataBuffer.size());
	auto lines = split(sv, '\n');
	auto it = lines.begin();

	for (; it != lines.end(); ++it)
	{	
		auto line = std::string(*it);

		if (line[0] == '[')
		{
			char sceneLine[1024];
			scan(line, "%*c %[^]]", sceneLine);
			if (sceneLine == stagename)
			{
				createSceneList(it);
				return true;
			}
		}
	}
	
	return false;
}

static void showScene(bool allowSkip)
{
	graphics.setFontSize(0);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);
	
	Cutscene *scene = (Cutscene*)sceneList.getHead();
	
	SDL_Surface *panel = graphics.quickSprite("panel", graphics.createSurface(640, 90));
	SDL_Surface *image = NULL;
	SDL_FillRect(panel, NULL, graphics.black);
	
	float panelAlpha = 0;
	
	SDL_SetAlpha(panel, 0);
	
	engine.clearInput();
	engine.flushInput();
	
	float changeTime = 100;
	
	engine.resetTimeDifference();
	
	audio.playMusicOnce();
	
	while (true)
	{
		graphics.updateScreen();
		engine.getInput();
		config.populate();
		
		engine.doTimeDifference();
		
		if ((engine.userAccepts()) && (allowSkip))
		{
			changeTime = 0;
			panelAlpha = 255;
			engine.clearInput();
		}
		
		if (panelAlpha < 256)
		{
			panelAlpha += (1 * engine.getTimeDifference());
			SDL_SetAlpha(panel, panelAlpha);
			if (image != NULL)
			{
				SDL_SetAlpha(image, panelAlpha);
				graphics.blit(image, 0, 0, graphics.screen, false);
			}
			graphics.blit(panel, 0, 390, graphics.screen, false);
		}
		
		changeTime -= (1 * engine.getTimeDifference());
		
		if (changeTime <= 0)
		{
			if (scene->next != NULL)
			{
				scene = (Cutscene*)scene->next;
				panelAlpha = 0;
				changeTime = scene->waitTime;
				graphics.clearChatString();
				graphics.createChatString(scene->text);
				SDL_FillRect(panel, NULL, graphics.black);
				graphics.drawChatString(panel, 0);
				image = NULL;
				
				if (!scene->sprite.empty())
				{
					debug(("Getting cutscene %s\n", scene->sprite));
					image = graphics.getSprite(scene->sprite, true)->image[0];
					SDL_SetColorKey(image, 0, SDL_MapRGB(image->format, 0, 0, 0));
				}
			}
			else
			{
				break;
			}
		}

		SDL_Delay(16);
	}
	
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);
}

void checkStartCutscene()
{
	// Easy mode doesn't have cutscenes!
	if (game.skill == 0)
	{
		return;
	}
	
	std::string sceneName = game.stageName + " Start";
	
	if (setupScene(sceneName))
	{
		audio.loadMusic("music/cutscene");
		showScene(true);
	}
	
	graphics.free();
	audio.free();
}

void checkEndCutscene()
{
	// Easy mode doesn't have cutscenes!
	if (game.skill == 0)
	{
		return;
	}
	
	std::string sceneName = game.stageName + " Start";
	
	debug(("%s\n", sceneName));
	
	bool allowSkip = true;
	
	// Don't let the player skip the end of game cutscene...
	// So we get the music timed well! :)
	if (game.stageName == "Final Battle")
	{
		allowSkip = false;
	}
	
	if (setupScene(sceneName))
	{
		if (game.stageName != "Final Battle")
		{
			audio.loadMusic("music/cutscene");
		}
		else
		{
			audio.loadMusic("music/end");
		}

		showScene(allowSkip);
	}
	
	graphics.free();
	audio.free();
}

void easyGameFinished()
{
	graphics.free();
	audio.free();
	
	audio.loadMusic("music/gameover");
	setupScene("Easy Game Finished");
	showScene(true);
	audio.fadeMusic();
	graphics.delay(3500);
	
	graphics.free();
	audio.free();
}


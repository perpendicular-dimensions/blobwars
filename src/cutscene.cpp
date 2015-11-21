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

#include "cutscene.h"

List sceneList;

void createSceneList()
{
	char sceneLine[1024] = "";
	char *line = NULL;
	int waitTime = 0;
	Cutscene *scene = NULL;
	
	while (true)
	{
		line = strtok(NULL, "\n");
		if (line == NULL)
			break;
		
		if (strcmp(sceneLine, "@EOF@") == 0)
			break;
	
		if (line[0] == '[')
			break;	
		
		if (strcmp(line, "END") == 0)
			break;
		
		if (strcmp(line, "NEW") == 0)
		{
			scene = new Cutscene();
			sceneList.add(scene);
			
			// Assume graphics is first line after new
			line = strtok(NULL, "\n");
			if (strcmp(line, "@none@") != 0)
			{
				strlcpy(scene->sprite, line, sizeof scene->sprite);
				debug(("Loading cutscene image %s\n", scene->sprite));
				graphics.quickSprite(scene->sprite, graphics.loadImage(scene->sprite));
			}
			line = strtok(NULL, "\n");
			sscanf(line, "%d", &waitTime);
			scene->waitTime = (waitTime * 100);
			line = strtok(NULL, "\n");
		}
		
		if (scene && strcmp(line, "@none@") != 0)
		{
			scene->appendText(line);
		}
	}
}

bool setupScene(const char *stagename)
{
	sceneList.clear();
	
	char sceneLine[1024];

	if (!engine.loadData(_("data/ending")))
		return graphics.showErrorAndExit("Couldn't load cutscene data file (%s)", _("data/ending")), false;

	char *line = strtok((char*)engine.dataBuffer, "\n");
	int i = 0;

	graphics.clearChatString();
	
	bool found = false;

	while (!found)
	{	
		if (line[0] == '[')
		{
			sscanf(line, "%*c %[^]]", sceneLine);
			if (strcmp(sceneLine, stagename) == 0)
			{
				found = true;
			}
		}

		if (!found)
		{
			line = strtok(NULL, "\n");
			if (line == NULL)
				break;
		}

		i++;

		// sanity check!
		if (i == 10000)
		{
			exit(1);
		}
	}
	
	if (found)
	{
		createSceneList();
	}
	
	return found;
}

void showScene(bool allowSkip)
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
				
				if (strcmp(scene->sprite, "") != 0)
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
	
	char sceneName[1024];
	snprintf(sceneName, sizeof sceneName, "%s Start", game.stageName);
	
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
	

	char sceneName[1024];
	snprintf(sceneName, sizeof sceneName, "%s End", game.stageName);
	
	debug(("%s\n", sceneName));
	
	bool allowSkip = true;
	
	// Don't let the player skip the end of game cutscene...
	// So we get the music timed well! :)
	if (strcmp(game.stageName, "Final Battle") == 0)
	{
		allowSkip = false;
	}
	
	if (setupScene(sceneName))
	{
		if (strcmp(game.stageName, "Final Battle") != 0)
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


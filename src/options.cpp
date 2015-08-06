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

#include "options.h"

void showCheatConfig()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);

	SDL_Surface *header = graphics.getSprite("cheatHeader", true)->image[0];
	SDL_Surface *optionsBackground = graphics.getSprite("optionsBackground", true)->image[0];
	SDL_SetColorKey(optionsBackground, 0, SDL_MapRGB(optionsBackground->format, 0, 0, 0));

	if (!engine.loadWidgets(_("data/cheatWidgets")))
	{
		graphics.showErrorAndExit(ERR_FILE, _("data/cheatWidgets"));
	}
		
	int done = 0;

	engine.setWidgetVariable("health", &engine.cheatHealth);
	engine.setWidgetVariable("extras", &engine.cheatExtras);
	engine.setWidgetVariable("fuel", &engine.cheatFuel);
	engine.setWidgetVariable("rate", &engine.cheatReload);
	engine.setWidgetVariable("blood", &engine.cheatBlood);
	engine.setWidgetVariable("invulnerable", &engine.cheatInvulnerable);
	engine.setWidgetVariable("speed", &engine.cheatSpeed);
	engine.setWidgetVariable("levels", &engine.cheatLevels);
	engine.setWidgetVariable("skip", &engine.cheatSkipLevel);
	engine.setWidgetVariable("confirm", &done);
	
	graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
	graphics.blit(header, 320, 25, graphics.screen, true);
	drawWidgets();

	engine.flushInput();
	engine.clearInput();
	
	int menuSound = -1;

	while (!done)
	{
		graphics.updateScreen();
		
		if (menuSound)
			audio.playMenuSound(menuSound);

		engine.getInput();
		config.populate();

		menuSound = engine.processWidgets();

		if (menuSound)
		{
			graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
			graphics.blit(header, 320, 25, graphics.screen, true);
			drawWidgets();
		}

		if (engine.keyState[SDL_SCANCODE_ESCAPE])
		{
			engine.clearInput();
			engine.flushInput();
			done = 1;
		}

		SDL_Delay(16);
	}
	
	audio.playMenuSound(2);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);

	if (!engine.loadWidgets(_("data/optionWidgets")))
	{
		graphics.showErrorAndExit(ERR_FILE, _("data/optionWidgets"));
	}
	
	engine.highlightWidget("cheats");
}

void showKeyConfig()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);

	if (!engine.loadWidgets(_("data/keyboardWidgets")))
	{
		graphics.showErrorAndExit(ERR_FILE, _("data/keyboardWidgets"));
	}
		
	SDL_Surface *header = graphics.getSprite("keyHeader", true)->image[0];
	SDL_Surface *optionsBackground = graphics.getSprite("optionsBackground", true)->image[0];
	SDL_SetColorKey(optionsBackground, 0, SDL_MapRGB(optionsBackground->format, 0, 0, 0));

	int done = 0;
	int defaults = 0;

	engine.setWidgetVariable("left", &config.keyboard.control[CONTROL::LEFT]);
	engine.setWidgetVariable("right", &config.keyboard.control[CONTROL::RIGHT]);
	engine.setWidgetVariable("down", &config.keyboard.control[CONTROL::DOWN]);
	engine.setWidgetVariable("fire", &config.keyboard.control[CONTROL::FIRE]);
	engine.setWidgetVariable("jump", &config.keyboard.control[CONTROL::JUMP]);
	engine.setWidgetVariable("pause", &config.keyboard.control[CONTROL::PAUSE]);

	engine.setWidgetVariable("jetpack", &config.keyboard.control[CONTROL::JETPACK]);
	engine.setWidgetVariable("map", &config.keyboard.control[CONTROL::MAP]);

	engine.setWidgetVariable("defaults", &defaults);
	engine.setWidgetVariable("confirm", &done);
	
	graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
	graphics.blit(header, 320, 25, graphics.screen, true);
	drawWidgets();

	engine.flushInput();
	engine.clearInput();
	
	int menuSound = -1;
	
	engine.allowJoypad = false;

	while (!done)
	{
		graphics.updateScreen();
		
		if (menuSound)
		{
			audio.playMenuSound(menuSound);
		}

		engine.getInput();
		config.populate();

		menuSound = engine.processWidgets();

		graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
		graphics.blit(header, 320, 25, graphics.screen, true);
		drawWidgets();
		
		if (defaults)
		{
			config.restoreKeyDefaults();
			defaults = 0;
		}

		if (engine.keyState[SDL_SCANCODE_ESCAPE])
		{
			engine.clearInput();
			engine.flushInput();
			done = 1;
		}

		SDL_Delay(16);
	}
	
	engine.allowJoypad = true;
	
	config.saveKeyConfig();

	audio.playMenuSound(2);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);

	if (!engine.loadWidgets(_("data/optionWidgets")))
	{
		graphics.showErrorAndExit(ERR_FILE, _("data/optionWidgets"));
	}
	
	engine.highlightWidget("keys");
}

void showJoystickConfig()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);

	if (!engine.loadWidgets(_("data/joystickWidgets")))
		graphics.showErrorAndExit(ERR_FILE, _("data/joystickWidgets"));
		
	SDL_Surface *header = graphics.getSprite("joystickHeader", true)->image[0];
	SDL_Surface *optionsBackground = graphics.getSprite("optionsBackground", true)->image[0];
	SDL_SetColorKey(optionsBackground, 0, SDL_MapRGB(optionsBackground->format, 0, 0, 0));

	int done = 0;
	int sensitivity = (config.joystick.sensitivity / 100);

	engine.setWidgetVariable("left", &config.joystick.control[CONTROL::LEFT]);
	engine.setWidgetVariable("right", &config.joystick.control[CONTROL::RIGHT]);
	engine.setWidgetVariable("up", &config.joystick.control[CONTROL::UP]);
	engine.setWidgetVariable("down", &config.joystick.control[CONTROL::DOWN]);
	engine.setWidgetVariable("fire", &config.joystick.control[CONTROL::FIRE]);
	engine.setWidgetVariable("jump", &config.joystick.control[CONTROL::JUMP]);
	engine.setWidgetVariable("pause", &config.joystick.control[CONTROL::PAUSE]);

	engine.setWidgetVariable("jetpack", &config.joystick.control[CONTROL::JETPACK]);
	engine.setWidgetVariable("map", &config.joystick.control[CONTROL::MAP]);
	
	engine.setWidgetVariable("sensitivity", &sensitivity);

	engine.setWidgetVariable("confirm", &done);
	
	graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
	graphics.blit(header, 320, 25, graphics.screen, true);
	drawWidgets();

	engine.flushInput();
	engine.clearInput();
	
	int menuSound = -1;
	
	engine.allowJoypad = false;

	while (!done)
	{
		graphics.updateScreen();
		
		if (menuSound)
			audio.playMenuSound(menuSound);

		engine.getInput();
		config.populate();

		menuSound = engine.processWidgets();

		graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
		graphics.blit(header, 320, 25, graphics.screen, true);
		drawWidgets();

		if (engine.keyState[SDL_SCANCODE_ESCAPE])
		{
			engine.clearInput();
			engine.flushInput();
			done = 1;
		}

		SDL_Delay(16);
	}
	
	config.joystick.sensitivity = (sensitivity * 100);
	
	engine.allowJoypad = true;
	
	config.saveJoystickConfig();

	audio.playMenuSound(2);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);

	if (!engine.loadWidgets(_("data/optionWidgets")))
	{
		graphics.showErrorAndExit(ERR_FILE, _("data/optionWidgets"));
	}
	
	engine.highlightWidget("joysticks");
}

void showOptions()
{
	float brightness;

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);

	if (!engine.loadWidgets(_("data/optionWidgets")))
	{
		graphics.showErrorAndExit(ERR_FILE, _("data/optionWidgets"));
	}
		
	SDL_Surface *header = graphics.getSprite("optionsHeader", true)->image[0];
	SDL_Surface *optionsBackground = graphics.getSprite("optionsBackground", true)->image[0];
	SDL_SetColorKey(optionsBackground, 0, SDL_MapRGB(optionsBackground->format, 0, 0, 0));

	int done = 0;
	int joysticks = 0;
	int cheats = 0;
	int keys = 0;

	engine.setWidgetVariable("fullscreen", &engine.fullScreen);
	engine.setWidgetVariable("soundvol", &game.soundVol);
	engine.setWidgetVariable("musicvol", &game.musicVol);
	engine.setWidgetVariable("output", &game.output);
	engine.setWidgetVariable("autosave", &game.autoSave);
	engine.setWidgetVariable("gamma", &game.brightness);
	engine.setWidgetVariable("gore", &game.gore);
	engine.setWidgetVariable("keys", &keys);
	engine.setWidgetVariable("joysticks", &joysticks);
	engine.setWidgetVariable("cheats", &cheats);
	engine.setWidgetVariable("confirm", &done);

	if (!engine.useAudio)
	{
		engine.enableWidget("soundvol", false);
		engine.enableWidget("musicvol", false);
		engine.enableWidget("output", false);
	}

	if (SDL_NumJoysticks() == 0)
	{
		engine.enableWidget("joysticks", false);
	}

	engine.showWidget("cheats", engine.cheats);

	graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
	graphics.blit(header, 320, 25, graphics.screen, true);
	drawWidgets();

	engine.flushInput();
	engine.clearInput();
	
	int menuSound = -1;

	while (!done)
	{
		graphics.updateScreen();
		
		if (menuSound)
			audio.playMenuSound(menuSound);

		engine.getInput();
		config.populate();

		if (engine.compareLastKeyInputs())
		{
			if (engine.cheats)
			{
				audio.playSound(SND_CHEAT, CH_ANY);
				engine.clearCheatVars();
			}

			engine.showWidget("cheats", engine.cheats);
			drawWidgets();
		}
		
		menuSound = engine.processWidgets();

		if (menuSound)
		{
			if (engine.widgetChanged("soundvol"))
				audio.setSoundVolume(game.soundVol);

			if (engine.widgetChanged("musicvol"))
				audio.setMusicVolume(game.musicVol);

			if (engine.widgetChanged("fullscreen"))
				SDL_SetWindowFullscreen(graphics.window, engine.fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

			if (engine.widgetChanged("gamma"))
			{
				brightness = game.brightness;
				if (brightness > 0) {
					brightness /= 10;
					uint16_t ramp[256];
					SDL_CalculateGammaRamp(brightness, ramp);
					SDL_SetWindowGammaRamp(graphics.window, ramp, ramp, ramp);
				}
			}
			
			if ((joysticks) || (cheats) || (keys))
			{
				audio.playMenuSound(2);
				menuSound = 0;

				if (joysticks)
				{
					showJoystickConfig();
				}
				else if (cheats)
				{
					showCheatConfig();
				}
				else if (keys)
				{
					showKeyConfig();
				}
				
				joysticks = keys = cheats = 0;

				engine.setWidgetVariable("fullscreen", &engine.fullScreen);
				engine.setWidgetVariable("soundvol", &game.soundVol);
				engine.setWidgetVariable("musicvol", &game.musicVol);
				engine.setWidgetVariable("output", &game.output);
				engine.setWidgetVariable("autosave", &game.autoSave);
				engine.setWidgetVariable("gamma", &game.brightness);
				engine.setWidgetVariable("gore", &game.gore);
				engine.setWidgetVariable("keys", &keys);
				engine.setWidgetVariable("joysticks", &joysticks);
				engine.setWidgetVariable("cheats", &cheats);
				engine.setWidgetVariable("confirm", &done);

				if (!engine.useAudio)
				{
					engine.enableWidget("soundvol", false);
					engine.enableWidget("musicvol", false);
					engine.enableWidget("output", false);
				}
				
				if (SDL_NumJoysticks() == 0)
				{
					engine.enableWidget("joysticks", false);
				}
				
				engine.showWidget("cheats", engine.cheats);
			}

			graphics.blit(optionsBackground, 0, 0, graphics.screen, false);
			graphics.blit(header, 320, 25, graphics.screen, true);
			drawWidgets();
		}

		if (engine.keyState[SDL_SCANCODE_ESCAPE])
		{
			engine.clearInput();
			engine.flushInput();
			done = 1;
		}

		SDL_Delay(16);
	}
	
	
	if (audio.output  != game.output)
	{
		audio.output = game.output;
		
		if (!audio.output)
		{
			audio.stopMusic();
			audio.stopAmbiance();
		}
		else
		{
			audio.playMusic();
			audio.playAmbiance();
		}
	}

	audio.playMenuSound(2);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(500);
}

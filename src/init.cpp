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

#include "init.h"

static bool displayLicense;

void checkForLicense()
{
	if (!engine.loadData(_("data/license")))
	{
		graphics.showLicenseErrorAndExit();
	}
}

/*
Show the GNU Public License the first time the game is played. Waits 4 seconds
and then proceeds. THIS MUST NOT BE REMOVED!!!!!
*/
void showLicense()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(1000);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	SDL_Surface *pic = graphics.loadImage("gfx/main/licensePic.png");
	graphics.blit(pic, 0, 0, graphics.screen, false);
	SDL_FreeSurface(pic);

	checkForLicense();

	char line[255];
	int y = 0;

	char *token = strtok((char*)engine.dataBuffer, "\n");

	while (true)
	{
		sscanf(token, "%d %[^\n\r]", &y, line);
		
		if (y == -1)
		{
			break;
		}

		graphics.drawString(line, 320, y, true, graphics.screen);

		token = strtok(NULL, "\n");

		if (token == NULL)
		{
			break;
		}
	}

	graphics.delay(4000);

	graphics.drawString(_("Press Space to Continue..."), 320, 440, true, graphics.screen);

	engine.flushInput();
	engine.clearInput();

	while (true)
	{
		graphics.updateScreen();
		engine.getInput();
		config.populate();
		if (engine.userAccepts())
			break;
		SDL_Delay(16);
	}
	
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(1000);
}

#if UNIX
void setupUserHomeDirectory()
{
	char *userHome = getenv("HOME");
	
	if ((!userHome) || (userHome == NULL))
	{
		printf("Couldn't determine user home directory! Exitting.\n");
		exit(1);
	}

	debug(("User Home = %s\n", userHome));
	
	char dir[PATH_MAX];
	dir[0] = 0;

	snprintf(dir, sizeof dir, "%s/.parallelrealities", userHome);
	if ((mkdir(dir, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) != 0) && (errno != EEXIST))
	{
		printf("Couldn't create required directory '%s'", dir);
		exit(1);
	}

	snprintf(dir, sizeof dir, "%s/.parallelrealities/blobwars", userHome);
	if ((mkdir(dir, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) != 0) && (errno != EEXIST))
	{
		printf("Couldn't create required directory '%s'", dir);
		exit(1);
	}

	char gameSavePath[PATH_MAX];
	snprintf(gameSavePath, sizeof gameSavePath, "%s/.parallelrealities/blobwars/", userHome);
	engine.setUserHome(gameSavePath);
}
#endif

bool loadConfig()
{
	float version = 0;
	int release = 0;
	bool rtn = false;

	char configPath[PATH_MAX];

	snprintf(configPath, sizeof configPath, "%sconfig", engine.userHomeDirectory);

	debug(("Loading Config from %s\n", configPath));

	FILE *fp = fopen(configPath, "rb");

	if (!fp)
	{
		return true;
	}

	if (fscanf(fp, "%10f %10d", &version, &release) != 2)
	{
		rtn = true;
	}

	debug(("Version = %.2f - Expected %.2f\n", version, VERSION));
	debug(("Release = %d - Expected %d\n", release, RELEASE));

	if ((version != VERSION) && (release != RELEASE))
	{
		rtn = true;
	}

	if (fscanf(fp, "%10d %10d %10d %10d %10d %10d %10d", &engine.fullScreen, &game.musicVol, &game.soundVol, &game.output, &game.brightness, &engine.extremeAvailable, &game.gore) != 7)
	{
		rtn = true;
	}

	fclose(fp);

	debug(("Extreme Mode = %d\n", engine.extremeAvailable));
	debug(("Output Type = %d\n", game.output));

	// Override audio if there is no sound available
	if ((engine.useAudio) && (game.output))
	{
		engine.useAudio = game.output;
	}
		
	config.loadKeyConfig();
	config.loadJoystickConfig();

	return rtn;
}

void saveConfig()
{
	char configPath[PATH_MAX];

	snprintf(configPath, sizeof configPath, "%sconfig", engine.userHomeDirectory);

	FILE *fp = fopen(configPath, "wb");

	if (!fp)
	{
		printf("Error Saving Config to %s\n", configPath);
		return;
	}

	fprintf(fp, "%f %d\n", VERSION, RELEASE);
	fprintf(fp, "%d %d %d %d %d %d %d\n", engine.fullScreen, game.musicVol, game.soundVol, game.output, game.brightness, engine.extremeAvailable, game.gore);

	fclose(fp);
	
	debug(("Output Type = %d\n", game.output));
}

//
// see if we can load the private keyState
//
int initMedalService(void *data)
{
	(void)data;

	SDL_mutexP(medalServer.lock);
	
	char connectMessage[1024];
	snprintf(connectMessage, sizeof connectMessage, "Contacting Medal Server - %s:%d", MEDAL_SERVER_HOST, MEDAL_SERVER_PORT);
	
	graphics.showMedalMessage(-1, connectMessage);
	
	char keyPath[PATH_MAX];
	char privateKey[20];

	snprintf(keyPath, sizeof keyPath, "%smedalKey", engine.userHomeDirectory);
	
	debug(("Loading private key from %s\n", keyPath));
	
	FILE *fp = fopen(keyPath, "rb");
	
	if (!fp)
	{
		graphics.showMedalMessage(-1, "No Medal Key found - Online functions disabled");
		SDL_mutexV(medalServer.lock);
		return 0;
	}
	
	if (fscanf(fp, "%19s", privateKey) != 1)
	{
		graphics.showMedalMessage(-1, "Medal Key file corrupt - Online functions disabled");
		SDL_mutexV(medalServer.lock);
		fclose(fp);
		return 0;
	}
	
	fclose(fp);
		
	if (!medalServer.connect(privateKey))
	{
		graphics.showMedalMessage(-1, "Medal Server Connection Failed - Online functions disabled");
		return 0;
	}
	
	graphics.showMedalMessage(-1, "Connected to Medal Server");
	
	SDL_mutexV(medalServer.lock);
	
	return 1;
}

void initConfig()
{
	#if UNIX
	setupUserHomeDirectory();
	#endif

	displayLicense = loadConfig();
}

/*
Chugg chugg chugg.... brrr... chugg chugg chugg...brrrrrr... chugg ch..
BRRRRRRRRRRRRRRRRRMMMMMMMMMMMMMMMMMMM!! Well, hopefully anyway! ;)
*/
void initSystem()
{
	long flags = SDL_INIT_VIDEO|SDL_INIT_JOYSTICK;
			
	if (engine.useAudio)
	{
		flags |= SDL_INIT_AUDIO;
	}

	/* Initialize the SDL library */
	if (SDL_Init(flags) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	graphics.screen = SDL_CreateRGBSurface(0, 640, 480, 32, 0xff0000, 0xff00, 0xff, 0xff000000);

	if (graphics.screen == NULL)
	{
		printf("Couldn't set 640x480 video mode: %s\n", SDL_GetError());
		exit(1);
	}

	// Increase the size of the window if we have large desktop resolutions
	SDL_DisplayMode displayMode = {};
	SDL_GetDesktopDisplayMode(0, &displayMode);
	int w = graphics.screen->w;
	int h = graphics.screen->h;
	while (displayMode.w > w * 2 && displayMode.h > h * 2)
	{
		w *= 2;
		h *= 2;
	}

	graphics.window = SDL_CreateWindow("Blobwars: Metal Blob Solid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE);

	if (graphics.window == NULL)
	{
		printf("Couldn't create %dx%d window: %s\n", w, h, SDL_GetError());
		exit(1);
	}

	graphics.renderer = SDL_CreateRenderer(graphics.window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (graphics.renderer == NULL)
	{
		printf("Couldn't create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_RenderSetLogicalSize(graphics.renderer, graphics.screen->w, graphics.screen->h);
	graphics.texture = SDL_CreateTexture(graphics.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, graphics.screen->w, graphics.screen->h);

	if (graphics.texture == NULL)
	{
		printf("Could not create %dx%d texture: %s\n", graphics.screen->w, graphics.screen->h, SDL_GetError());
		exit(1);
	}

	SDL_SetWindowFullscreen(graphics.window, engine.fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

	// This (attempts to) set the gamma correction. We attempt to catch an error here
	// in case someone has done something really stupid in the config file(!!)
	if (game.brightness != -1) {
		Math::limitInt(&game.brightness, 1, 20);
		float brightness = game.brightness;
		brightness /= 10;
		uint16_t ramp[256];
		SDL_CalculateGammaRamp(brightness, ramp);
		SDL_SetWindowGammaRamp(graphics.window, ramp, ramp, ramp);
	}

	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}

	if (engine.useAudio)
	{
		if (Mix_OpenAudio(22050, AUDIO_S16, engine.useAudio, 1024) < 0)
		{
			printf("Warning: Couldn't set 44100 Hz 16-bit audio - Reason: %s\n", Mix_GetError());
			printf("Sound and Music will be disabled\n");
			engine.useAudio = 0;
		}
	}

	debug(("Found %d Joysticks...\n", SDL_NumJoysticks()));

	if (SDL_NumJoysticks() > 0)
	{
		debug(("Opening Joystick - %s...\n", SDL_JoystickName(0)));
		SDL_JoystickEventState(SDL_ENABLE);
		config.sdlJoystick = SDL_JoystickOpen(0);
	}

	SDL_ShowCursor(SDL_DISABLE);

	graphics.registerEngine(&engine);
	graphics.mapColors();

 	audio.registerEngine(&engine);
	audio.setSoundVolume(game.soundVol);
	audio.setMusicVolume(game.musicVol);
	audio.output = game.output;

	debug(("Sound Volume = %d\n", game.soundVol));
	debug(("Music Volume = %d\n", game.musicVol));
	debug(("Output Type = %d\n", game.output));
	debug(("Brightness = %d\n", game.brightness));
	debug(("tmp dir = %s\n", engine.userHomeDirectory));
	debug(("Pack Dir = %s\n", PAKLOCATION));
	debug(("Loading Fonts...\n"));

	#if USEPAK
		if (!engine.unpack("data/vera.ttf", PAK_FONT))
		{
			engine.reportFontFailure();
		}
	#endif

	graphics.loadFont(0, "data/vera.ttf", 12);
	graphics.loadFont(1, "data/vera.ttf", 15);
	graphics.loadFont(2, "data/vera.ttf", 19);
	graphics.loadFont(3, "data/vera.ttf", 23);
	graphics.loadFont(4, "data/vera.ttf", 24);

	debug(("Font sizes all loaded!!\n"));

	audio.loadSound(SND_CHEAT, "sound/Lock And Load!!!");
	audio.loadSound(SND_HIGHLIGHT, "sound/menu");
	audio.loadSound(SND_SELECT, "sound/select");
	
	graphics.medal[0] = graphics.loadImage("gfx/main/medal_bronze_1.png");
	graphics.medal[1] = graphics.loadImage("gfx/main/award_star_silver_3.png");
	graphics.medal[2] = graphics.loadImage("gfx/main/shield.png");
	graphics.medal[3] = graphics.loadImage("gfx/main/ruby.png");

	graphics.license[0] = graphics.loadImage("gfx/main/cc-by.png", true);
	graphics.license[1] = graphics.loadImage("gfx/main/cc-by-sa.png", true);

	SDL_Surface *device = graphics.loadImage("gfx/main/alienDevice.png");
	SDL_SetWindowIcon(graphics.window, device);
	SDL_FreeSurface(device);
	
	if (strstr(engine.userHomeDirectory, "/root"))
	{
		graphics.showRootWarning();
	}

	if (displayLicense)
	{
		showLicense();
	}
	else
	{
		checkForLicense();
	}
	
	if (SDLNet_Init() < 0)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
	}
	else
	{
		SDL_Thread *thread = SDL_CreateThread(initMedalService, "MedalService", NULL);
	
		if (thread == NULL)
		{
			printf("Unable to create thread: %s\n", SDL_GetError());
			printf("Calling medal server directly\n");
			initMedalService(NULL);
			return;
		}
	}
	
	engine.saveConfig = true;

	debug(("Init Complete...\n"));
}

/*
Removes [hopefully] all the resources that has been
loaded and created during the game. This is called by
atexit();
*/
void cleanup()
{
	char tempPath[PATH_MAX];
	
	debug(("Cleaning Up...\n"));
	
	debug(("Updating Replay Data\n"));
	replayData.header.score = game.score;

	debug(("Freeing Audio...\n"));
	audio.destroy();

	debug(("Removing Music...\n"));
	snprintf(tempPath, sizeof tempPath, "%smusic.mod", engine.userHomeDirectory);
	remove(tempPath);

	debug(("Freeing Game Info...\n"));
	game.destroy();

	debug(("Freeing Map Data...\n"));
	map.destroy();

	debug(("Freeing Engine Data...\n"));
	engine.destroy();

	debug(("Freeing Graphics...\n"));
	graphics.destroy();

	debug(("Saving Config...\n"));
	if (engine.saveConfig)
	{
		saveConfig();
	}

	debug(("Removing Font File...\n"));
	snprintf(tempPath, sizeof tempPath, "%sfont.ttf", engine.userHomeDirectory);
	remove(tempPath);
	
	if (SDL_NumJoysticks() > 0)
	{
		SDL_JoystickEventState(SDL_DISABLE);
		for (int i = 0 ; i < SDL_NumJoysticks() ; i++)
		{
			debug(("Closing Joystick #%d\n", i));
			SDL_JoystickClose(config.sdlJoystick);
		}
	}

	debug(("Closing Audio...\n"));
	if (engine.useAudio)
	{
		Mix_CloseAudio();
	}

	debug(("Closing TTF...\n"));
	TTF_Quit();
	
	debug(("Closing NET...\n"));
	SDLNet_Quit();

	debug(("Closing SDL Sub System...\n"));
	SDL_Quit();

	debug(("All Done.\n"));
	
	if (replayData.replayMode == REPLAY_MODE::PLAYBACK)
	{
		printf("\n==== ACTUAL PLAYBACK DATA ====\n");
		replayData.printReplayInformation();
	}
}


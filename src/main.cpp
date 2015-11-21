/*
Copyright (C) 2004-2011 Parallel Realities
Copyright (C) 2011-2015 Perpendicular Dimensions
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

#include "main.h"

#include <locale.h>

Audio audio;
Config config;
Engine engine;
Game game;
GameData gameData;
Graphics graphics;
Map map;
ReplayData replayData;
MedalServer medalServer;

Entity defEnemy[MAX_ENEMIES];
Entity defItem[MAX_ITEMS];
Entity player;
Weapon weapon[MAX_WEAPONS];

void showVersion()
{
	printf(_(
		"\n"
		"Blob Wars, Episode I - Metal Blob Solid (Version %.2f, Release %d)\n"
		"Copyright (C) 2004-2011 Parallel Realities\n"
		"Copyright (C) 2011-2015 Perpendicular Dimensions\n"
		"Licensed under the GNU General Public License (GPL)\n"
		"\n"),
		VERSION, RELEASE);
}

void showHelp()
{
	showVersion();

	printf(_(
		"The Metal Blob Solid gameplay manual can be found in,\n"
		"\t%s\n"
		"\n"
		"Replay Commands\n"
		"\t-map <filename>       Play the specified map (use -listmaps to see maps)\n"
		"\t-record <filename>    Record a game and output to the specified file\n"
		"\t-playback <filename>  Playback the specified recording\n"
		"\t-listmaps             List the available maps for playing\n"
		"\n"
		"Replay Examples\n"
		"\tblobwars -map data/grasslands1 -record replay.dat\n"
		"\tblobwars -playback replay.dat\n"
		"\n"
		"Additional Commands\n"
		"\t-fullscreen         Start the game in Full Screen mode\n"
		"\t-window             Start the game in Window mode\n"
		"\t-mono               Use mono sound output instead of stereo\n"
		"\t-noaudio            Disables audio\n"
		"\t-version            Display version number\n"
		"\t--help              This help\n"
		"\n"),
		GAMEPLAYMANUAL);

	exit(0);
}

void listMaps()
{
	showVersion();
	
	printf(_(
		"Available Maps\n"
		"\tdata/arcticWastes\n"
		"\tdata/assimilator\n"
		"\tdata/caves1\n"
		"\tdata/caves2\n"
		"\tdata/caves3\n"
		"\tdata/caves4\n"
		"\tdata/comm\n"
		"\tdata/finalBattle\n"
		"\tdata/floodedTunnel1\n"
		"\tdata/floodedTunnel2\n"
		"\tdata/floodedTunnel3\n"
		"\tdata/floodedTunnel4\n"
		"\tdata/grasslands1\n"
		"\tdata/grasslands2\n"
		"\tdata/grasslands3\n"
		"\tdata/hq\n"
		"\tdata/icecave1\n"
		"\tdata/icecave2\n"
		"\tdata/spaceStation\n"
		"\tdata/supply\n"
		"\tdata/tomb1\n"
		"\tdata/tomb2\n"
		"\tdata/tomb3\n"
		"\tdata/tomb4\n"
		"\n"));
	
	exit(0);
}

int main(int argc, char *argv[])
{
	#if !USEPAK
	debug(("Not Using PAK...\n"));
	#endif

	#if RELEASE
	#ifdef PAKLOCATION
	if (PAKLOCATION[0] && chdir(PAKLOCATION))
	{
		perror("Could not chdir to '" PAKLOCATION "'");
		return 1;
	}
	#endif
	#endif
	
	config.engine = &engine;
	
	replayData.reset();

	bindtextdomain("blobwars", LOCALEDIR);
	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "C");
	textdomain("blobwars");

	atexit(cleanup);

	bool showSprites = false;
	bool hub = false;
	
	int recordMode = REPLAY_MODE::NONE;
	int requiredSection = SECTION_INTRO;

	initConfig();

	for (int i = 1 ; i < argc ; i++)
	{
		if (strcmp(argv[i], "-fullscreen") == 0) engine.fullScreen = true;
		else if (strcmp(argv[i], "-window") == 0) engine.fullScreen = false;
		else if (strcmp(argv[i], "-noaudio") == 0) engine.useAudio = 0;
		else if (strcmp(argv[i], "-mono") == 0) engine.useAudio = 1;
		else if (strcmp(argv[i], "-version") == 0) {showVersion(); exit(0);}
		else if (strcmp(argv[i], "--help") == 0) showHelp();
		else if (strcmp(argv[i], "-record") == 0) {recordMode = REPLAY_MODE::RECORD; strlcpy(replayData.filename, argv[++i], sizeof replayData.filename);}
		else if (strcmp(argv[i], "-playback") == 0) {recordMode = REPLAY_MODE::PLAYBACK; strlcpy(replayData.filename, argv[++i], sizeof replayData.filename);}
		else if (strcmp(argv[i], "-map") == 0) {if (argc > i + 1) {game.setMapName(argv[++i]); requiredSection = SECTION_GAME;}}
		else if (strcmp(argv[i], "-listmaps") == 0) listMaps();
		else if (strcmp(argv[i], "-credits") == 0) requiredSection = SECTION_CREDITS;
		
		#if DEBUG
		else if (strcmp(argv[i], "-showsprites") == 0) showSprites = true;
		else if (strcmp(argv[i], "-hub") == 0) hub = true;
		else if (strcmp(argv[i], "-randomscreens") == 0) graphics.takeRandomScreenShots = true;
		else if (strcmp(argv[i], "-nomonsters") == 0) engine.devNoMonsters = true;
		#endif

		else {fprintf(stderr, "Unknown argument '%s'\n", argv[i]); showHelp();}
	}
	
	switch (recordMode)
	{
		case REPLAY_MODE::NONE:
			// nothing to do...
			break;
			
		case REPLAY_MODE::RECORD:
			requiredSection = SECTION_GAME;
			strlcpy(replayData.header.map, game.mapName, sizeof replayData.header.map);
			replayData.header.skill = game.skill = 3;
			replayData.setMode(REPLAY_MODE::RECORD);
			break;
			
		case REPLAY_MODE::PLAYBACK:
			requiredSection = SECTION_GAME;
			replayData.setMode(REPLAY_MODE::PLAYBACK);
			replayData.header.time = 0;
			game.setMapName(replayData.header.map);
			game.skill = replayData.header.skill;
			break;
	}

	initSystem();
	
	// seed the random using the one generated
	// via the replay data
	Math::pSeed = replayData.header.randomSeed;
	
	player.setName("Player");

	engine.flushInput();
	
	engine.allowQuit = true;

	if (hub)
	{
		requiredSection = SECTION_HUB;
		loadGame(0);
		loadResources();
	}
	
	if (game.skill == 3)
	{
		game.hasAquaLung = game.hasJetPack = true;
	}

	if (showSprites)
	{
		showAllSprites();
	}

	while (true)
	{
		switch (requiredSection)
		{
			case SECTION_INTRO:
				requiredSection = doIntro();
				break;

			case SECTION_TITLE:
				requiredSection = title();
				break;

			case SECTION_HUB:
				requiredSection = doHub();
				break;

			case SECTION_GAME:
				if (!game.continueFromCheckPoint)
				{
					checkStartCutscene();
					loadResources();
				}
				requiredSection = doGame();
				break;

			case SECTION_GAMEOVER:
				requiredSection = gameover();
				break;
				
			case SECTION_EASYOVER:
				map.clear();
				game.clear();
				easyGameFinished();
				requiredSection = SECTION_TITLE;
				break;
				
			case SECTION_CREDITS:
				doCredits();
				requiredSection = SECTION_TITLE;
				break;
		}
	}

	return 0;
}

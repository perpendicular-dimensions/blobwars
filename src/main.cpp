/*
Copyright (C) 2004 Parallel Realities

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

void showHelp()
{
	printf("\n");
	printf("Blob Wars, Episode I - Metal Blob Solid (Version %.2f, Release %d)\n", VERSION, RELEASE);
	printf("Copyright (C) 2004 Parallel Realities\n");
	printf("Licensed under the GNU General Public License (GPL)\n\n");

	printf("The Metal Blob Solid gameplay manual can be found in,\n");
	printf("\t%s\n\n", GAMEPLAYMANUAL);
	
	printf("Replay Commands\n");
	printf("\t-map <filename>       Play the specified map (use -listmaps to see maps)\n");
	printf("\t-record <filename>    Record a game and output to the specified file\n");
	printf("\t-playback <filename>  Playback the specified recording\n");
	printf("\t-listmaps             List the available maps for playing\n\n");
	
	printf("Replay Examples\n");
	printf("\tblobwars -map data/grasslands1 -record replay.dat\n");
	printf("\tblobwars -playback replay.dat\n\n");

	printf("Additional Commands\n");
	printf("\t-fullscreen         Start the game in Full Screen mode\n");
	printf("\t-mono               Use mono sound output instead of stereo\n");
	printf("\t-noaudio            Disables audio\n");
	printf("\t-version            Display version number\n");
	printf("\t--help              This help\n\n");

	exit(0);
}

void listMaps()
{
	printf("\n");
	printf("Blob Wars, Episode I - Metal Blob Solid (Version %.2f, Release %d)\n", VERSION, RELEASE);
	printf("Copyright (C) 2004 Parallel Realities\n");
	printf("Licensed under the GNU General Public License (GPL)\n\n");
	
	printf("Available Maps\n");
	printf("\tdata/arcticWastes\n");
	printf("\tdata/assimilator\n");
	printf("\tdata/caves1\n");
	printf("\tdata/caves2\n");
	printf("\tdata/caves3\n");
	printf("\tdata/caves4\n");
	printf("\tdata/comm\n");
	printf("\tdata/finalBattle\n");
	printf("\tdata/floodedTunnel1\n");
	printf("\tdata/floodedTunnel2\n");
	printf("\tdata/floodedTunnel3\n");
	printf("\tdata/floodedTunnel4\n");
	printf("\tdata/grasslands1\n");
	printf("\tdata/grasslands2\n");
	printf("\tdata/grasslands3\n");
	printf("\tdata/hq\n");
	printf("\tdata/icecave1\n");
	printf("\tdata/icecave2\n");
	printf("\tdata/spaceStation\n");
	printf("\tdata/supply\n");
	printf("\tdata/tomb1\n");
	printf("\tdata/tomb2\n");
	printf("\tdata/tomb3\n");
	printf("\tdata/tomb4\n\n");
	
	exit(0);
}

void showVersion()
{
	printf("\n");
	printf("Blob Wars, Episode I - Metal Blob Solid (Version %.2f, Release %d)\n", VERSION, RELEASE);
	printf("Copyright (C) 2004 Parallel Realities\n");
	printf("Licensed under the GNU General Public License (GPL)\n\n");
// 	exit(0);
}

int main(int argc, char *argv[])
{
	#if !USEPAK
	debug(("Not Using PAK...\n"));
	#endif
	
	config.engine = &engine;
	
	replayData.reset();

	bindtextdomain("blobwars", LOCALEDIR);
	setlocale(LC_ALL, "");
	textdomain("blobwars");

	atexit(cleanup);

	bool showSprites = false;
	bool hub = false;
	
	int recordMode = REPLAY_MODE::NONE;
	int requiredSection = SECTION_INTRO;

	for (int i = 1 ; i < argc ; i++)
	{
		if (strcmp(argv[i], "-fullscreen") == 0) engine.fullScreen = true;
		else if (strcmp(argv[i], "-noaudio") == 0) engine.useAudio = 0;
		else if (strcmp(argv[i], "-mono") == 0) engine.useAudio = 1;
		else if (strcmp(argv[i], "-version") == 0) showVersion();
		else if (strcmp(argv[i], "--help") == 0) showHelp();
		else if (strcmp(argv[i], "-record") == 0) {recordMode = REPLAY_MODE::RECORD; strcpy(replayData.filename, argv[++i]);}
		else if (strcmp(argv[i], "-playback") == 0) {recordMode = REPLAY_MODE::PLAYBACK; strcpy(replayData.filename, argv[++i]);}
		else if (strcmp(argv[i], "-map") == 0) {game.setMapName(argv[++i]); requiredSection = SECTION_GAME;}
		else if (strcmp(argv[i], "-listmaps") == 0) listMaps();
		
		#if !USEPAK
		else if (strcmp(argv[i], "-showsprites") == 0) showSprites = true;
		else if (strcmp(argv[i], "-hub") == 0) hub = true;
		else if (strcmp(argv[i], "-randomscreens") == 0) graphics.takeRandomScreenShots = true;
		else if (strcmp(argv[i], "-nomonsters") == 0) engine.devNoMonsters = true;
		else if (strcmp(argv[i], "-credits") == 0) requiredSection = SECTION_CREDITS;
		#endif
	}
	
	switch (recordMode)
	{
		case REPLAY_MODE::NONE:
			// nothing to do...
			break;
			
		case REPLAY_MODE::RECORD:
			requiredSection = SECTION_GAME;
			strcpy(replayData.header.map, game.mapName);
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

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

#include "game.h"

void newGame()
{
	game.clear();
	gameData.clear();
	map.persistantList.clear();
	engine.practice = false;

	game.skill = engine.skill;
}

void showInGameOptions()
{
	if (!engine.loadWidgets(_("data/inGameWidgets")))
	{
		return graphics.showErrorAndExit(ERR_FILE, _("data/inGameWidgets"));
	}

	graphics.drawRect(120, 100, 400, 300, graphics.black, graphics.white, graphics.screen);

	int cont, options, escape, quit, escapeyes, escapeno, warnno, warnyes, quitno, quityes, train, trainno, trainyes, restart, restartno, restartyes;
	cont = options = escape = quit = escapeyes = escapeno = warnno = warnyes = quitno = quityes = train = trainno = trainyes = restart = restartno = restartyes = 0;

	engine.setWidgetVariable("continue", &cont);
	engine.setWidgetVariable("options", &options);
	engine.setWidgetVariable("escape", &escape);
	engine.setWidgetVariable("restart", &restart);
	engine.setWidgetVariable("quit", &quit);
	engine.setWidgetVariable("train", &train);

	engine.setWidgetVariable("warnno", &warnno);
	engine.setWidgetVariable("warnyes", &warnyes);
	
	engine.setWidgetVariable("restartno", &restartno);
	engine.setWidgetVariable("restartyes", &restartyes);

	engine.setWidgetVariable("quitno", &quitno);
	engine.setWidgetVariable("quityes", &quityes);
	
	engine.setWidgetVariable("trainno", &quitno);
	engine.setWidgetVariable("trainyes", &quityes);
	
	engine.setWidgetVariable("escapeno", &escapeno);
	engine.setWidgetVariable("escapeyes", &escapeyes);

	engine.showWidgetGroup("warning", false);
	engine.showWidgetGroup("restartconf", false);
	engine.showWidgetGroup("escapeconf", false);
	engine.showWidgetGroup("quitconf", false);
	engine.showWidgetGroup("trainconf", false);
	
	if ((map.isBossMission) || (engine.practice) || (strcmp(map.name, "Space Station") == 0))
	{
		engine.enableWidget("escape", false);
		engine.enableWidget("restart", false);
	}
		
	if (!engine.practice)
	{
		engine.showWidget("train", false);
	}
	else
	{
		engine.showWidget("quit", false);
	}

	engine.flushInput();
	engine.clearInput();

	drawWidgets();
	audio.playMenuSound(2);
	
	int menuSound = -1;

	while (true)
	{
		graphics.updateScreen();		
		engine.getInput();
		config.populate();

		if ((config.isControl(CONTROL::PAUSE)) || (engine.keyState[SDL_SCANCODE_ESCAPE]))
		{
			engine.keyState[SDL_SCANCODE_ESCAPE] = 0;
			config.resetControl(CONTROL::PAUSE);
			break;
		}
		
		menuSound = engine.processWidgets();

		if (menuSound)
		{
			graphics.drawRect(120, 100, 400, 300, graphics.black, graphics.white, graphics.screen);
			drawWidgets();
			audio.playMenuSound(menuSound);
		}

		if (cont)
		{
			break;
		}

		if (escape)
		{
			engine.showWidgetGroup("options", false);

			if (!gameData.stagePreviouslyCleared(game.stageName))
			{
				engine.showWidgetGroup("warning", true);
				engine.highlightWidget("warnno");
			}
			else
			{
				engine.showWidgetGroup("escapeconf", true);
				engine.highlightWidget("escapeno");
			}

			graphics.drawRect(120, 100, 400, 300, graphics.black, graphics.white, graphics.screen);
			drawWidgets();
			escape = 0;
		}

		if ((escapeyes) || (warnyes))
		{
			audio.stopMusic();
			audio.stopAmbiance();
			addTeleportParticles(player.x, player.y, 50, SND_TELEPORT3);
			dropCarriedItems();
			game.setMissionOver(MIS_PLAYERESCAPE);
			break;
		}

		if (options)
		{
			showOptions();
			break;
		}

		if ((warnno) || (quitno) || (escapeno) || (trainno) || (restartno))
		{
			engine.highlightWidget("continue");
			engine.showWidgetGroup("options", true);
			engine.showWidgetGroup("warning", false);
			engine.showWidgetGroup("trainconf", false);
			engine.showWidgetGroup("escapeconf", false);
			engine.showWidgetGroup("quitconf", false);
			engine.showWidgetGroup("restartconf", false);
			
			if (!engine.practice)
			{
				engine.showWidget("train", false);
			}
			else
			{
				engine.showWidget("quit", false);
			}
			
			graphics.drawRect(120, 100, 400, 300, graphics.black, graphics.white, graphics.screen);
			drawWidgets();
			quitno = trainno = warnno = escapeno = restartno = 0;
		}

		if (quit)
		{
			engine.showWidgetGroup("options", false);
			engine.showWidgetGroup("quitconf", true);
			engine.highlightWidget("quitno");

			graphics.drawRect(120, 100, 400, 300, graphics.black, graphics.white, graphics.screen);
			drawWidgets();
			quit = 0;
		}
		
		if (train)
		{
			engine.showWidgetGroup("options", false);
			engine.showWidgetGroup("trainconf", true);
			engine.highlightWidget("trainno");

			graphics.drawRect(120, 100, 400, 300, graphics.black, graphics.white, graphics.screen);
			drawWidgets();
			train = 0;
		}
		
		if (restart)
		{
			engine.showWidgetGroup("options", false);
			engine.showWidgetGroup("restartconf", true);
			engine.highlightWidget("restartno");

			graphics.drawRect(120, 100, 400, 300, graphics.black, graphics.white, graphics.screen);
			drawWidgets();
			restart = 0;
		}

		if ((quityes) || (trainyes))
		{
			game.setMissionOver(MIS_PLAYERQUIT);
			break;
		}
		
		if (restartyes)
		{
			game.setMissionOver(MIS_PLAYERRESTART);
			SDL_FillRect(graphics.screen, NULL, graphics.black);
			audio.stopMusic();
			audio.stopAmbiance();
			graphics.delay(500);
			break;
		}

		SDL_Delay(16);
	}
}

void doGameStuff()
{
	engine.getInput();
	config.populate();
	replayData.read(config.command);
	
	if (game.missionOverReason == MIS_INPROGRESS)
	{
		config.doPause();
	}

	engine.doFrameLoop();

	graphics.updateScreen();
	graphics.animateSprites();
	graphics.drawBackground();

	doEffects();
	doTrains();
	doTraps();
	drawMap();
	doLineDefs();
	doSwitches();
	doItems();
	doBullets();
	doMIAs();
	
	if (map.isBossMission)
	{
		doBosses();
	}
	
	doEnemies();
	doObstacles();
	doTeleporters();
	
	if (map.isBlizzardLevel)
	{
		doWind();
	}
	
	doParticles();
	
	replayData.set(config.command);	
	replayData.commit();
}

int gameover()
{
	audio.stopMusic();
	audio.stopAmbiance();
	
	if (!engine.loadWidgets(_("data/gameOverWidgets")))
	{
		return graphics.showErrorAndExit(ERR_FILE, _("data/gameOverWidgets")), SECTION_GAME;
	}

	SDL_Surface *gameover = graphics.quickSprite("Game Over", graphics.loadImage("gfx/main/gameover.png"));
	graphics.setTransparent(gameover);

	audio.loadGameOverMusic();
	audio.playMusic();

	engine.flushInput();
	engine.clearInput();

	unsigned int frameLimit = SDL_GetTicks() + 16;

	bool showGameOverOptions = false;
	int cont, quit, menuSound;
	cont = quit = menuSound = 0;

	engine.setWidgetVariable("gameOverNo", &cont);
	engine.setWidgetVariable("gameOverYes", &quit);
	
	if (game.canContinue > 1)
	{
		Widget *widget = engine.getWidgetByName("gameOverNo");
		char postfix[100];
		snprintf(postfix, sizeof postfix, " (%d)", game.canContinue);
		strlcat(widget->label, postfix, sizeof widget->label);
	}

	while (true)
	{
		if (menuSound)
			audio.playMenuSound(menuSound);

		doGameStuff();
		drawMapTopLayer();

		graphics.blit(gameover, 320, 240, graphics.screen, true);

		if (engine.userAccepts())
		{
			if (!showGameOverOptions)
			{
				showGameOverOptions = true;
				engine.showWidgetGroup("gameover", true);
				engine.highlightWidget("gameOverNo");
				engine.flushInput();
				engine.clearInput();
			}

			// Can't continue on a boss mission or if no checkpoints reached!
			if ((map.isBossMission) || (!game.canContinue))
			{
				engine.showWidgetGroup("gameover", false);
				quit = 1;
			}
		}
		
		if (showGameOverOptions)
		{
			drawWidgets();
			menuSound = engine.processWidgets();
		}

		if ((cont) || (quit))
			break;

		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}

	if (quit)
	{
		audio.fadeMusic();
		graphics.fadeToBlack();
		map.clear();
		
		return SECTION_TITLE;
	}

	game.continueFromCheckPoint = true;
	audio.stopMusic();
	audio.reloadLevelMusic();

	return SECTION_GAME;
}

void showMissionInformation()
{
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.updateScreen();

	SDL_Surface *panel = graphics.createSurface(400, 300);
	SDL_Surface *panelBack = graphics.alphaRect(400, 300, 0x00, 0x00, 0x00);
	SDL_SetColorKey(panel, SDL_TRUE, SDL_MapRGB(panel->format, 0, 0, 0));

	graphics.drawRect(1, 1, 398, 298, graphics.black, graphics.white, panelBack);

	char message[256];
	int col1 = 25;
	int col2 = 375;
	int y = 30;

	Objective *objective;

	graphics.setFontSize(0);

	graphics.setFontSize(3);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	graphics.drawString(_(game.stageName), 200, 20, TXT_CENTERED, panel);

	graphics.setFontSize(0);

	y += 20;

	if (map.totalMIAs > 0)
	{
		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		snprintf(message, sizeof message, _("Rescue %d MIAs"), map.requiredMIAs);
		graphics.drawString(message, col1, y, TXT_LEFT, panel);

		if (map.foundMIAs < map.requiredMIAs)
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			snprintf(message, sizeof message, "%d / %d", map.foundMIAs, map.requiredMIAs);
			graphics.drawString(message, col2, y, TXT_RIGHT, panel);
		}
		else
		{
			graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Completed"), col2, y, TXT_RIGHT, panel);
		}
	}

	objective = (Objective*)map.objectiveList.getHead();

	while (objective->next != NULL)
	{
		objective = (Objective*)objective->next;

		y += 20;

		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		
		if ((game.skill < 3) &&  (strstr(objective->description, "L.R.T.S.")) && (!gameData.completedWorld))
		{
			graphics.drawString(_("???? ???????? ????"), col1, y, TXT_LEFT, panel);
		}
		else
		{
			graphics.drawString(_(objective->description), col1, y, TXT_LEFT, panel);
		}
		
		// this is a fake objective (for the 4th Ancient Tomb)
		if (objective->targetValue == -1)
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Incomplete"), col2, y, TXT_RIGHT, panel);
		}
		else if (objective->currentValue < objective->targetValue)
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			if (objective->targetValue == 1)
			{
				graphics.drawString(_("Incomplete"), col2, y, TXT_RIGHT, panel);
			}
			else
			{
				snprintf(message, sizeof message, "%d / %d", objective->currentValue, objective->targetValue);
				graphics.drawString(message, col2, y, TXT_RIGHT, panel);
			}
		}
		else
		{
			graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Completed"), col2, y, TXT_RIGHT, panel);
		}
	}
	
	if (game.skill == 3)
	{
		graphics.setFontColor(0xff, 0xff, 0x00, 0x00, 0x00, 0x00);
		snprintf(message, sizeof message, _("Time Limit - %d:%.2d Minutes"), map.remainingMinutes, map.remainingSeconds);
		graphics.drawString(message, 200, 260, TXT_CENTERED, panel);
	}
	
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	graphics.drawString(_("Press Fire to Continue"), 200, 280, TXT_CENTERED, panel);

	engine.flushInput();
	engine.clearInput();

	int px, py;

	map.getRandomEntityPosition(&px, &py);
	map.getRandomEntityPosition(&player.tx, &player.ty);

	player.x = px;
	player.y = py;

	unsigned int frameLimit = SDL_GetTicks() + 16;
		
	while (true)
	{
		if ((int)player.x < player.tx) player.x += 2;
		if ((int)player.x > player.tx) player.x -= 2;
		if ((int)player.y < player.ty) player.y += 2;
		if ((int)player.y > player.ty) player.y -= 2;

		if (Collision::collision(player.x, player.y, 5, 5, player.tx, player.ty, 5, 5))
			map.getRandomEntityPosition(&player.tx, &player.ty);

		engine.setPlayerPosition((int)player.x, (int)player.y, map.limitLeft, map.limitRight, map.limitUp, map.limitDown);

		doGameStuff();
		drawMapTopLayer();
		graphics.blit(panelBack, 320, 220, graphics.screen, true);
		graphics.blit(panel, 320, 220, graphics.screen, true);

		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;

		if (engine.userAccepts())
			break;
	}

	SDL_FreeSurface(panel);
	SDL_FreeSurface(panelBack);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(1000);
}

void beamInPlayer()
{
	game.getCheckPoint(&player.x, &player.y);
	
	int beamInTime = 180;
	
	unsigned int frameLimit = SDL_GetTicks() + 16;
	
	audio.playSound(SND_TELEPORT1, CH_ANY, player.x);
	
	engine.setPlayerPosition((int)player.x, (int)player.y, map.limitLeft, map.limitRight, map.limitUp, map.limitDown);
	
	while (beamInTime > 0)
	{
		engine.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
		
		doGameStuff();
		drawMapTopLayer();
		
		if ((beamInTime % 10) == 0)
			addTeleportParticles(player.x + 10, player.y + 10, 50, -1);
		
		beamInTime--;
	}
	
	game.getCheckPoint(&player.x, &player.y);
	
	player.dx = 0;
	player.dy = 0;
	player.immune = 120;
	player.environment = ENV_AIR;
	player.oxygen = 7;
	player.fuel = 7;
	Math::removeBit(&player.flags, ENT_FLIES);
}

int doGame()
{
	graphics.setFontSize(0);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.delay(1000);

	Uint32 frames, frameLimit, millis;
	Uint32 start, cur;

	#if DEBUG
	Uint32 now, then, frameCounter;
	char fps[10];
	strlcpy(fps, "fps", sizeof fps);
	#endif

	engine.messageTime = -1;
	engine.messagePriority = -1;

	audio.playMusic();
	audio.playAmbiance();

	if (!game.continueFromCheckPoint)
	{
		player.health = -99;

		if ((!map.isBossMission) && (replayData.replayMode == REPLAY_MODE::NONE))
		{
			showMissionInformation();
		}

		game.levelsStarted++;
	}
	else
	{
		game.useObjectiveCheckPoint();
	}

	player.setVelocity(0, 1);
	player.baseThink = 60;
	player.health = MAX_HEALTH;

	if (game.continueFromCheckPoint)
	{
		player.health = (MAX_HEALTH / 2);
		player.setSprites(graphics.getSprite("BobRight", true), graphics.getSprite("BobLeft", true), graphics.getSprite("BobSpin", true));
	}
	else
	{
		player.currentWeapon = &weapon[WP_PISTOL];
	}

	game.resetMissionOver();

	frameLimit = SDL_GetTicks() + 16;
	frames = millis = 0;
	start = SDL_GetTicks();
#ifdef DEBUG
	then = frameCounter = start;
#endif

	if ((strcmp(map.name, "Space Station") == 0) && (!game.continueFromCheckPoint))
	{
		beamInPlayer();
	}
	else
	{
		resetPlayer();
	}

	engine.flushInput();
	engine.clearInput();

	debug(("Map Clipping is %d %d %d %d\n", map.limitLeft, map.limitRight, map.limitUp, map.limitDown));

	game.continueFromCheckPoint = false;

	engine.paused = false;

	while (true)
	{
		++frames;
		++millis;
		cur = SDL_GetTicks();

		if (game.missionOverReason != MIS_PLAYEROUT)
		{
  			engine.setPlayerPosition((int)player.x, (int)player.y, map.limitLeft, map.limitRight, map.limitUp, map.limitDown);
		}

		doSpawnPoints();
		doGameStuff();

		doPlayer();
		raiseWaterLevel();

		if ((player.environment == ENV_SLIME) || (player.environment == ENV_LAVA) || (player.health < 1))
		{
			if (player.health < 1)
			{
				game.setMissionOver(MIS_PLAYERDEAD);
				audio.fadeMusic();
				audio.stopAmbiance();
			}
			else
			{
				game.setMissionOver(MIS_PLAYEROUT);
				player.immune = 130;
			}
		}

		if (config.isControl(CONTROL::MAP))
		{
			config.resetControl(CONTROL::MAP);
			if (!map.isBossMission)
			{
				showMap((int)(player.x / 32), (int)(player.y / 32));
			}
			else
			{
				engine.setInfoMessage("Automap is not available!", 1, INFO_HINT);
			}
		}

		drawMapTopLayer();
		doStatusBar();
		doMusicInfo(cur - start);

		if ((engine.keyState[SDL_SCANCODE_ESCAPE]) && (game.missionOver == 0))
		{
			if (replayData.replayMode == REPLAY_MODE::NONE)
			{
				showInGameOptions();
			}
			else
			{
				exit(0);
			}
		}

		if (allObjectivesCompleted())
		{
			if (game.missionOver == 0)
			{
				if (map.isBossMission)
				{
					map.killAllEnemies();
				}
				
				audio.stopMusic();
				audio.stopAmbiance();

				game.setMissionOver(MIS_COMPLETE);
				engine.setInfoMessage("All Required Objectives Met - Mission Complete", 10, INFO_OBJECTIVE);
			}
		}

		if (game.missionOver > 0)
		{
			game.missionOver--;
		
			if (game.missionOver == 0)
			{
				if (game.missionOverReason == MIS_PLAYEROUT)
				{
					SDL_FillRect(graphics.screen, NULL, graphics.black);
					graphics.updateScreen();
					graphics.delay(1000);
					engine.flushInput();
					engine.clearInput();
					resetPlayer();
					game.resetMissionOver();
				}
				else
				{
					if (game.missionOverReason == MIS_COMPLETE)
					{
						game.missionOver = MAX_FPS * 2;
						
						if (strcmp(map.name, "Space Station") != 0)
						{
							addTeleportParticles(player.x, player.y, 50, SND_TELEPORT3);
							dropCarriedItems();
							game.missionOverReason = MIS_PLAYERESCAPE;
						}
						else
						{
							break;
						}
					}
					else if (game.missionOverReason == MIS_GAMECOMPLETE)
					{
						addTeleportParticles(player.x, player.y, 50, SND_TELEPORT3);
						game.missionOverReason = MIS_PLAYERESCAPE;
						game.missionOver = MAX_FPS * 4;
					}
					else
					{
						if (replayData.replayMode != REPLAY_MODE::NONE)
						{
							exit(0);
						}
					
						break;
					}
				}
			}
		}

		Math::limitInt(&(--game.lastComboTime), 0, 60);

		if (millis >= 60)
		{
			millis = 0;
			
			if ((game.missionOverReason == MIS_INPROGRESS) || (game.missionOverReason == MIS_PLAYEROUT))
			{
				game.incrementMissionTime();
				
				if (game.skill == 3)
				{
					doTimeRemaining();
				}
			}
		}

		if (engine.paused)
		{
			doPauseInfo();
			audio.pause();
		}

		while (engine.paused)
		{
			engine.getInput();
			config.populate();
			config.doPause();
			graphics.updateScreen();
			#ifdef DEBUG
			then = SDL_GetTicks();
			#endif
			frames = 0;

			if (!engine.paused)
			{
				audio.resume();
			}

			SDL_Delay(16);
		}
		
		if ((engine.keyState[SDL_SCANCODE_F3]) && (engine.cheatSkipLevel))
		{
			autoCompleteAllObjectives(true);
			engine.keyState[SDL_SCANCODE_F3] = 0;
			engine.setInfoMessage("Skipping Mission...", 2, INFO_OBJECTIVE);
		}
		
		#if DEBUG
		if (engine.keyState[SDL_SCANCODE_F1])
		{
			autoCompleteAllObjectives(false);
		}		
		#endif
		
		if (replayData.replayMode != REPLAY_MODE::PLAYBACK)
		{
			engine.delay(frameLimit);
		}
		else if (!replayData.fast)
		{
			engine.delay(frameLimit);
		}
		
		if (engine.keyState[SDL_SCANCODE_F5])
		{
			replayData.fast = !replayData.fast;
			engine.keyState[SDL_SCANCODE_F5] = 0;
		}
		
		frameLimit = SDL_GetTicks()  + 16;
		
		if (game.missionOverReason == MIS_GAMECOMPLETE)
			frameLimit = SDL_GetTicks() + 64;

		#if DEBUG
		static Graphics::SurfaceCache fpsCache;
		graphics.drawString(fps, 600, 30, true, graphics.screen, fpsCache);

		if (SDL_GetTicks() > frameCounter + 500)
		{
			now = SDL_GetTicks();
			snprintf(fps, sizeof fps, "%2.2f fps", ((double)frames*1000)/(now - then));
			then = frameCounter = SDL_GetTicks();
			frames = 0;
		}
		#endif
	}

	if (allObjectivesCompleted())
	{
		if (strcmp(map.name, "Final Battle") == 0)
		{
			game.missionOverReason = MIS_GAMECOMPLETE;
		}
		else
		{
			game.missionOverReason = MIS_COMPLETE;
		}
	}

	switch (game.missionOverReason)
	{
		case MIS_COMPLETE:
			if (strcmp(map.name, "Space Station"))
			{
				graphics.delay(1000);
				audio.loadMusic("music/grasslands");
				audio.playMusic();
				graphics.fadeToBlack();
				
				bool previouslyCompleted = gameData.stagePreviouslyCleared(game.stageName);
				
				showMissionClear();
				
				if (engine.practice)
				{
					return SECTION_TITLE;
				}
				
				if (!previouslyCompleted)
				{
					checkEndCutscene();
				}
				return SECTION_HUB;
			}
			else
			{
				graphics.fadeToBlack();
				processPostMissionData();
				saveGame();
				game.setMapName("data/finalBattle");
				game.setStageName("Final Battle");
				return SECTION_GAME;
			}
			break;
			
		case MIS_GAMECOMPLETE:
			SDL_FillRect(graphics.screen, NULL, graphics.white);
			graphics.updateScreen();
			graphics.fadeToBlack();
			
			// we've finished the game. Extreme mode is now available! :)
			engine.extremeAvailable = true;
			map.clear();
			graphics.free();
			audio.free();
			checkEndCutscene();
			return SECTION_CREDITS;
			break;

		case MIS_TIMEUP:
			game.canContinue = 0;
		case MIS_PLAYERDEAD:
			if (player.health > -60)
			{
				player.health = -99;
				gibPlayer();
			}
			return SECTION_GAMEOVER;
			break;

		case MIS_PLAYERESCAPE:
			game.escapes++;
			if (gameData.stagePreviouslyCleared(game.stageName))
			{
				processPostMissionData();
				saveGame();
			}
			return SECTION_HUB;
			break;
			
		case MIS_PLAYERRESTART:
			clearAllMissionData();
			return SECTION_GAME;
			break;

		default:
			return SECTION_TITLE;
			break;
	}

	return SECTION_TITLE;
}

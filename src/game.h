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

extern void drawMap();
extern void drawMapTopLayer();
extern void showMap(int centerX, int centerY);
extern void raiseWaterLevel();
extern void doWind();

extern void doTimeRemaining();
extern void doStatusBar();
extern void doMusicInfo(unsigned int);
extern void doPauseInfo();

extern void doItems();
extern void doPlayer();
extern void doTrains();
extern void doSwitches();
extern void doBullets();
extern void doEffects();
extern void doParticles();
extern void doMIAs();
extern void doObstacles();
extern void doEnemies();
extern void doBosses();
extern void doSpawnPoints();
extern void doTeleporters();
extern void doLineDefs();
extern void doTraps();

extern void showOptions();
extern void drawWidgets();

extern void dropCarriedItems();

extern void resetPlayer();
extern void gibPlayer();

extern void addTeleportParticles(float x, float y, int amount, int soundToPlay);

extern void showMissionInformation();
extern bool allObjectivesCompleted();
extern void showMissionClear();
extern void autoCompleteAllObjectives(bool allObjectives);
extern void doCredits();

extern void checkEndCutscene();
extern void processPostMissionData();
extern void clearAllMissionData();
extern void saveGame();

extern void presentPlayerMedal(const char *tname);

extern Audio audio;
extern Config config;
extern Engine engine;
extern Graphics graphics;
extern Map map;
extern ReplayData replayData;

extern Entity player;
extern Game game;
extern GameData gameData;
extern Weapon weapon[MAX_WEAPONS];

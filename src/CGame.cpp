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

Game::Game()
{
	musicVol = 100;
	soundVol = 128;
	output = 2;
	brightness = 10;

	gore = 1;
	skill = 1;

	clear();
}

void Game::clear()
{
	hasAquaLung = hasJetPack = continueFromCheckPoint = false;
	score = stagesCleared = 0;
	totalHours = totalMinutes = totalSeconds = 0;
	currentMissionHours = currentMissionMinutes = currentMissionSeconds = 0;
	totalEnemiesDefeated = totalItemsCollected = totalBonusesCollected = 0;
	totalObjectivesCompleted = totalMIAsRescued = 0;
	currentMissionEnemiesDefeated = currentMissionItemsCollected = 0;
	stagesCleared = 0;

	lastComboTime = 0;
	currentComboHits = maxComboHits = 0;
	
	autoSave = 1;
	autoSaveSlot = 0;

	currentWeapon = 0;
	for (int i = 0 ; i < 5 ; i++)
	{
		bulletsHit[i] = bulletsFired[i] = 0;
	}

	strlcpy(mapName, "data/grasslands1", sizeof mapName);
	strlcpy(stageName, "Grasslands", sizeof stageName);

	continuesUsed = 0;
	levelsStarted = 0;
	escapes = 0;

	canContinue = 0;
}

void Game::destroy()
{
	clear();
}

void Game::incrementMissionTime()
{
	currentMissionSeconds++;
	if (currentMissionSeconds == 60)
	{
		currentMissionSeconds = 0;
		currentMissionMinutes++;
		if (currentMissionMinutes == 60)
		{
			currentMissionMinutes = 0;
			currentMissionHours++;
		}
	}
}

void Game::setObjectiveCheckPoint()
{
	objectiveCheckPointX = checkPointX;
	objectiveCheckPointY = checkPointY;
	canContinue = 3;
}

void Game::useObjectiveCheckPoint()
{
	checkPointX = objectiveCheckPointX;
	checkPointY = objectiveCheckPointY;
	continuesUsed++;
	canContinue--;
}

void Game::setCheckPoint(float x, float y)
{
	checkPointX = (int)x;
	checkPointY = (int)y;
}

void Game::getCheckPoint(float *x, float *y) const
{
	*x = checkPointX;
	*y = checkPointY;
}

void Game::doCombo()
{
	if (lastComboTime == 0)
		currentComboHits = 0;

	currentComboHits++;

	if (currentComboHits > maxComboHits)
		maxComboHits = currentComboHits;

	lastComboTime = 25;

	Math::limitChar(&maxComboHits, 0, 99);
	Math::limitChar(&currentComboHits, 0, 99);
}

void Game::incBulletsFired()
{
	bulletsFired[currentWeapon]++;
}

void Game::incBulletsHit()
{
	bulletsHit[currentWeapon]++;
}

int Game::getWeaponAccuracy(int weapon)
{
	if (bulletsHit[weapon])
	{
		return (int)(((0.0 + bulletsHit[weapon]) / (0.0 + bulletsFired[weapon])) * 100.0);
	}

	return 0;
}

int Game::getTotalBulletsFired() const
{
	return bulletsFired[0] + bulletsFired[1] + bulletsFired[2] + bulletsFired[3] + bulletsFired[4];
}

int Game::getTotalAccuracy()
{
	int fired = getTotalBulletsFired();
	int hits = bulletsHit[0] + bulletsHit[1] + bulletsHit[2] + bulletsHit[3] + bulletsHit[4];

	if (hits)
	{
		return (int)(((0.0 + hits) / (0.0 + fired)) * 100.0);
	}

	return 0;
}

int Game::getMostUsedWeapon()
{
	unsigned int mostUsed = 0;
	unsigned int mostFired = 0;

	for (int i = 0 ; i < 5 ; i++)
	{
		if (bulletsFired[i] > mostFired)
		{
			mostUsed = i;
			mostFired = bulletsFired[i];
		}
	}

	return mostUsed;
}

void Game::totalUpStats()
{
	totalEnemiesDefeated += currentMissionEnemiesDefeated;
	totalItemsCollected += currentMissionItemsCollected;

	totalSeconds += currentMissionSeconds;
	totalMinutes += currentMissionMinutes;
	totalHours += currentMissionHours;

	while (totalSeconds > 59)
	{
		totalSeconds -= 60;
		totalMinutes++;
	}

	while (totalMinutes > 59)
	{
		totalMinutes -= 60;
		totalHours++;
	}

	currentMissionEnemiesDefeated = currentMissionItemsCollected = 0;
	currentMissionSeconds = currentMissionMinutes = currentMissionHours = 0;
}

void Game::setStageName(const char *name)
{
	strlcpy(stageName, name, sizeof stageName);
}

void Game::setMapName(const char *name)
{
	strlcpy(mapName, name, sizeof mapName);
}

void Game::setMissionOver(int reason)
{
	if ((missionOver == 0) || (reason == MIS_TIMEUP))
	{
		switch (reason)
		{
			case MIS_COMPLETE:
				missionOver = MAX_FPS;
				break;
			case MIS_PLAYEROUT:
				missionOver = (int)(MAX_FPS * 2.5);
				break;
			case MIS_TIMEUP:
			case MIS_PLAYERDEAD:
				missionOver = MAX_FPS * 5;
				break;
			case MIS_PLAYERESCAPE:
				missionOver = MAX_FPS * 2;
				break;
			case MIS_GAMECOMPLETE:
				missionOver = MAX_FPS * 8;
				break;
			default:
				missionOver = MAX_FPS;
				break;
		}
	}

	missionOverReason = reason;
}

void Game::resetMissionOver()
{
	missionOver = missionOverReason = 0;
}

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

class Game {

	private:
	
		int objectiveCheckPointX, objectiveCheckPointY;

	public:

		char mapName[50];
		char stageName[50];

		int gore, skill, soundVol, musicVol, output, brightness, autoSaveSlot, autoSave;

		int score, stagesCleared;
		int totalHours, totalMinutes, totalSeconds;
		int currentMissionHours, currentMissionMinutes, currentMissionSeconds;
		int totalEnemiesDefeated, totalItemsCollected, totalBonusesCollected;
		int currentMissionEnemiesDefeated, currentMissionItemsCollected;
		int totalObjectivesCompleted, totalMIAsRescued;

		unsigned char currentWeapon;
		unsigned int bulletsFired[5], bulletsHit[5];

		int checkPointX, checkPointY;
		int teleportPointX, teleportPointY;

		bool hasAquaLung, hasJetPack, continueFromCheckPoint;

		int lastComboTime;
		unsigned char currentComboHits, maxComboHits;

		unsigned int missionOverReason;
		unsigned long missionOver;
		
		unsigned int continuesUsed;
		unsigned int levelsStarted;
		unsigned int escapes;
		
		int canContinue;

		Game();
		void clear();
		void destroy();
		void incrementMissionTime();
		void setCheckPoint(float x, float y);
		void getCheckPoint(float *x, float *y) const;
		void setObjectiveCheckPoint();
		void useObjectiveCheckPoint();
		void doCombo();
		void incBulletsFired();
		void incBulletsHit();
		int getWeaponAccuracy(int weapon);
		int getTotalBulletsFired() const;
		int getTotalAccuracy();
		int getMostUsedWeapon();
		void totalUpStats();
		void setStageName(const char *name);
		void setMapName(const char *name);
		void setMissionOver(int reason);
		void resetMissionOver();

};

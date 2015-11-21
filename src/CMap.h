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

class Map {

	private:
		
		Entity *allowableEnemy[10];
		int maxAllowableEnemies;

	public:

		char name[255];

		unsigned char data[MAPWIDTH][MAPHEIGHT];

		int offsetX, offsetY;
		int limitLeft, limitRight, limitUp, limitDown;

		int foundItems, totalItems;
		int foundMIAs, totalMIAs;
		int requiredMIAs;
		
		// time limit for extreme setting
		int remainingMinutes;
		int remainingSeconds;
		
		// special for ice levels
		bool isIceLevel;
		
		// special for flooded tunnel #4
		float waterLevel;
		int requiredWaterLevel;
		
		// special for arctic wastes
		bool isBlizzardLevel;
		int windChangeTime;
		float windPower;
		
		// air/water decoration
		bool isCavesTileset;
		bool isGrasslandsTileset;

		// Boss Stuff
		bool fightingGaldov;
		bool isBossMission;
		unsigned int bossNextThink;
		void (*doBossLevelAction) (void);
		Boss *boss[10];
		Boss *mainBossPart;
		float bossEnergyMeterBit;
		
		List persistantList;

		List trainList;
		List itemList;
		List bulletList;
		List enemyList;
		List miaList;
		List obstacleList;
		List particleList;
		List switchList;
		List spawnList;
		List effectList;
		List objectiveList;
		List teleportList;
		List lineList;
		List trapList;

	Map();
	
	void clear();
	void destroy();
	
	bool isPracticeMission();
	bool isValid(int x, int y);
	bool isSolid(int x, int y);
	bool isBreakable(int x, int y);
	bool isNoReset(int x, int y);
	bool isLiquid(int x, int y);
	bool isTopLayer(int x, int y);
	
	Persistant *getPersistant(const char *name);
	Persistant *createPersistant(const char *name);
	void destroyPersistant(const char *name);
	
	void setName(const char *name);
	void setClipping(int limitLeft, int limitRight, int limitUp, int limitDown);
	void addTrain(const char *name, int startX, int startY, int endX, int endY, int pause, bool atStart, bool active);
	void addDoor(const char *name, int type, int startX, int startY, int endX, int endY, bool active);
	void addSwitch(const char *name, const char *linkName, const char *requiredObjectName, const char *activateMessage, int type, int x, int y, bool activated);
	void addItem(Entity *item);
	void addBullet(Entity *bullet);
	void addParticle(float x, float y, float dx, float dy, int health, int color, Sprite *sprite, int flags);
	void addEnemy(Entity *enemy);
	void addMIA(Entity *mia);
	void addObstacle(Entity *obstacle);
	void addSpawnPoint(const char *name, int x, int y, int type, int subtype, int min, int max, bool active);
	void addEffect(Effect *effect);
	void addObjective(const char *description, const char *target, int targetValue, bool required);
	void addTeleporter(Teleporter *teleporter);
	void addLineDef(LineDef *lineDef);
	void addTrap(Trap *trap);
	void evalTileset(const char *baseDir);
	
	void killAllEnemies();
	
	void setAllowableEnemy(Entity *enemy);
	char *getSpawnableEnemy();
	char *getSpawnableEnemy(int i);
	
	void getRandomEntityPosition(int *x, int *y);
	void setMainBossPart(Boss *boss);
};

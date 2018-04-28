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

		std::string name;

		uint8_t data[MAPWIDTH][MAPHEIGHT];

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
		
		std::map<std::string, std::vector<std::string>> persistants;

		std::vector<std::unique_ptr<Train>> trains;
		std::list<std::unique_ptr<Entity>> items;
		std::list<std::unique_ptr<Entity>> bullets;
		std::list<std::unique_ptr<Entity>> enemies;
		std::vector<std::unique_ptr<Entity>> mias;
		std::vector<std::unique_ptr<Entity>> obstacles;
		std::list<std::unique_ptr<Particle>> particles;
		std::vector<std::unique_ptr<Switch>> switches;
		std::vector<std::unique_ptr<SpawnPoint>> spawns;
		std::list<std::unique_ptr<Effect>> effects;
		std::vector<std::unique_ptr<Objective>> objectives;
		std::vector<std::unique_ptr<Teleporter>> teleports;
		std::vector<std::unique_ptr<LineDef>> lines;
		std::list<std::unique_ptr<Trap>> traps;

	Map();
	
	void clear();
	
	bool isPracticeMission();
	bool isValid(int x, int y);
	bool isSolid(int x, int y);
	bool isBreakable(int x, int y);
	bool isNoReset(int x, int y);
	bool isLiquid(int x, int y);
	bool isTopLayer(int x, int y);
	
	std::vector<std::string> &getPersistant(const std::string &name);
	std::vector<std::string> &createPersistant(const std::string &name);
	void destroyPersistant(const std::string &name);
	
	void setName(const std::string &name);
	void setClipping(int limitLeft, int limitRight, int limitUp, int limitDown);
	void addTrain(const std::string &name, int startX, int startY, int endX, int endY, int pause, bool atStart, bool active);
	void addDoor(const std::string &name, int type, int startX, int startY, int endX, int endY, bool active);
	void addSwitch(const std::string &name, const std::string &linkName, const std::string &requiredObjectName, const std::string &activateMessage, int type, int x, int y, bool activated);
	void addItem(Entity *item);
	void addBullet(Entity *bullet);
	void addParticle(float x, float y, float dx, float dy, int health, int color, Sprite *sprite, int flags);
	void addEnemy(Entity *enemy);
	void addMIA(Entity *mia);
	void addObstacle(Entity *obstacle);
	void addSpawnPoint(const std::string &name, int x, int y, int type, int subtype, int min, int max, bool active);
	void addEffect(Effect *effect);
	void addObjective(const std::string &description, const std::string &target, int targetValue, bool required);
	void addTeleporter(Teleporter *teleporter);
	void addLineDef(LineDef *lineDef);
	void addTrap(Trap *trap);
	void evalTileset(const std::string &baseDir);
	
	void killAllEnemies();
	
	void setAllowableEnemy(Entity *enemy);
	std::string getSpawnableEnemy();
	std::string getSpawnableEnemy(int i);
	
	void getRandomEntityPosition(int *x, int *y);
	void setMainBossPart(Boss *boss);
};

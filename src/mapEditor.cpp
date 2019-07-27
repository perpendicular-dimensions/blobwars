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

Audio audio;
Engine engine;
Game game;
GameData gameData;
Graphics graphics;
Map map;

Entity defEnemy[MAX_ENEMIES];
Entity defItem[MAX_ITEMS];
Entity player;
Weapon weapon[MAX_WEAPONS];

MedalServer medalServer;
Config config;
ReplayData replayData;

static void drawMap(int mapX, int mapY)
{
	SDL_Rect r;
	int tile = 0;

	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			r.x = x * BRICKSIZE;
			r.y = y * BRICKSIZE;
			r.w = r.h = BRICKSIZE - 1;

			tile = map.data[x + mapX][y + mapY];

			if (tile == MAP_WATERANIM)
			{
				tile = graphics.getWaterAnim();
			}

			if (tile > 0)
			{
				graphics.blit(graphics.tile[tile], r.x, r.y, graphics.screen, false);

				if ((tile >= MAP_NORESET) && (tile < MAP_DECORATION))
				{
					graphics.drawRect(r.x, r.y, BRICKSIZE, 4, graphics.yellow, graphics.screen);
				}
			}
		}
	}
}

static void showMap(int *mapX, int *mapY)
{
	SDL_FillRect(graphics.screen, nullptr, graphics.black);

	engine.keyState[SDL_SCANCODE_SPACE] = 0;
	int moveTimer = 0;

	while (true)
	{
		graphics.updateScreen();
		engine.getInput();
		config.populate();

		if (engine.keyState[SDL_SCANCODE_SPACE])
			break;

		for (int x = 0; x < MAPWIDTH; x++)
		{
			for (int y = 0; y < MAPHEIGHT; y++)
			{
				switch (map.data[x][y])
				{
				case 0:
					graphics.putPixel(x, y, 9, graphics.screen);
					break;
				case 1:
					graphics.putPixel(x, y, graphics.blue, graphics.screen);
					break;
				case 2:
					graphics.putPixel(x, y, graphics.green, graphics.screen);
					break;
				case 3:
					graphics.putPixel(x, y, graphics.red, graphics.screen);
					break;
				default:
					graphics.putPixel(x, y, graphics.white, graphics.screen);
					break;
				}
			}
		}

		if (moveTimer == 0)
		{
			if (engine.keyState[SDL_SCANCODE_LEFT])
				*mapX -= 1;

			if (engine.keyState[SDL_SCANCODE_RIGHT])
				*mapX += 1;

			if (engine.keyState[SDL_SCANCODE_UP])
				*mapY -= 1;

			if (engine.keyState[SDL_SCANCODE_DOWN])
				*mapY += 1;
		}

		moveTimer--;
		Math::limitInt(&moveTimer, 0, 60);

		graphics.drawRect(*mapX, *mapY, 20, 1, graphics.green, graphics.screen);
		graphics.drawRect(*mapX, *mapY, 1, 15, graphics.green, graphics.screen);
		graphics.drawRect(*mapX, *mapY + 15, 20, 1, graphics.green, graphics.screen);
		graphics.drawRect(*mapX + 20, *mapY, 1, 15, graphics.green, graphics.screen);
	}

	engine.keyState[SDL_SCANCODE_SPACE] = 0;
}

static int nextBlock(int current, int dir)
{
	if ((current + dir) > 255)
		return current;

	if ((current + dir) < 0)
		return current;

	int wanted = current;

	while (true)
	{
		wanted += dir;

		if (wanted < 0)
			return current;
		if (wanted > 255)
			return current;

		if (graphics.tile[wanted])
			return wanted;
	}
}

static void drawEnemies()
{
	for (auto &&enemy: map.enemies)
	{
		if (enemy.owner != &enemy)
		{
			enemy.face = enemy.owner->face;
			(enemy.face == 0) ? enemy.x = enemy.owner->x + enemy.tx : enemy.x = enemy.owner->x + -enemy.tx;
			enemy.y = enemy.owner->y + enemy.ty;
		}

		int x = (int)(enemy.x - engine.playerPosX);
		int y = (int)(enemy.y - engine.playerPosY);

		int absX = abs(x);
		int absY = abs(y);

		if ((absX < 800) && (absY < 600))
		{
			graphics.blit(enemy.getFaceImage(), x, y, graphics.screen, false);
			enemy.animate();

			if ((!(enemy.flags & ENT_WEIGHTLESS)) && (!(enemy.flags & ENT_FLIES)) && (!(enemy.flags & ENT_SWIMS)))
				enemy.applyGravity();
		}
	}
}

static void deleteEnemy(int x, int y)
{
	x *= BRICKSIZE;
	y *= BRICKSIZE;

	map.enemies.remove_if([&](auto &&enemy) {
		return enemy.x == x && enemy.y == y;
	});
}

static void saveMap(const std::string &name)
{
	using namespace YAML;

	// Load the original file and modify the YAML structure in place.
	// This should preserve any manual additions to the map file.
	auto data = engine.loadYAML(name);
	if (!data)
		exit(1);

	// Update enemies
	Node enemies(NodeType::Sequence);

	for (auto &&enemy: map.enemies)
	{
		Node node(NodeType::Map);
		node.SetStyle(EmitterStyle::Flow);
		node["name"] = enemy.name;
		node["x"] = enemy.x;
		node["y"] = enemy.y;
		enemies.push_back(node);
	}

	data["enemies"] = enemies;

	// Determine the map crop area
	int minx = MAPWIDTH;
	int miny = MAPHEIGHT;
	int maxx = 0;
	int maxy = 0;

	for (int y = 0; y < MAPHEIGHT; y++)
	{
		for (int x = 0; x < MAPWIDTH; x++)
		{
			if (map.data[x][y])
			{
				minx = std::min(minx, x);
				miny = std::min(miny, y);
				maxx = std::max(maxx, x);
				maxy = std::max(maxy, y);
			}
		}
	}

	if (minx > maxx || miny > maxy)
	{
		minx = 0;
		miny = 0;
		maxx = -1;
		maxy = -1;
	}

	int w = maxx - minx + 1;
	int h = maxy - miny + 1;

	data["crop"] = Node(NodeType::Map);
	data["crop"].SetStyle(EmitterStyle::Flow);
	data["crop"]["x"] = minx;
	data["crop"]["y"] = miny;
	data["crop"]["w"] = w;
	data["crop"]["h"] = h;

	// Update the map data
	std::string mapdata;
	mapdata.reserve(h * (2 * w + 1));

	for (int y = miny; y <= maxy; y++)
	{
		for (int x = minx; x <= maxx; x++)
		{
			mapdata += fmt::format("{:02x}", map.data[x][y]);
		}
		mapdata += '\n';
	}

	data["data"] = mapdata;

	// Write out the resulting YAML document
	std::ofstream file(name);
	Emitter yaml(file);
	yaml << data;
	yaml << Newline;

	if (file.bad())
	{
		fmt::print("Could not save {}: {}\n", name, strerror(errno));
	}
	else
	{
		fmt::print("Saved {} ({})\n", name, SDL_GetTicks());
	}
}

static void newMap(const std::string &name)
{
	using namespace YAML;
	std::ofstream file(name);
	Emitter yaml(file);

	yaml << BeginMap;
	{
		yaml << Key << "stageName" << Value << "unnamed";
		yaml << Key << "tileset" << Value << "gfx/grasslands";
		yaml << Key << "background" << Value << "gfx/grasslands/stone.jpg";
		yaml << Key << "music" << Value << "music/tunnel";
		yaml << Key << "alphaTiles" << Value << Flow;
		{
			yaml << BeginSeq;
			yaml << 1 << 2 << 3 << 200 << 201 << 202 << 203 << 204 << 244 << 245 << 246;
			yaml << EndSeq;
		}
		yaml << Key << "start" << Value << Flow;
		{
			yaml << BeginMap;
			yaml << Key << "x" << Value << 0;
			yaml << Key << "y" << Value << 0;
			yaml << EndMap;
		}
		yaml << Key << "crop" << Value << Flow;
		{
			yaml << BeginMap;
			yaml << Key << "x" << Value << 0;
			yaml << Key << "y" << Value << 0;
			yaml << Key << "w" << Value << 0;
			yaml << Key << "h" << Value << 0;
			yaml << EndMap;
		}
		yaml << Newline << Newline;
		yaml << Key << "data" << Value << Literal << "";
	}
	yaml << EndMap;
	yaml << Newline;

	if (file.bad())
		exit(1);
}

static void addTileDecoration()
{
	printf("Adding Tile Decoration...\n");

	for (int y = 1; y < MAPHEIGHT; y++)
	{
		for (int x = 0; x < MAPWIDTH; x++)
		{
			if ((map.data[x][y] == 9) && (map.data[x][y - 1] == MAP_AIR))
			{
				if (Math::prand() % 4)
					map.data[x][y - 1] = 241;
			}
		}
	}

	for (int y = 0; y < MAPHEIGHT; y++)
	{
		for (int x = 0; x < MAPWIDTH; x++)
		{
			if (map.data[x][y] == 241)
			{
				if ((Math::prand() % 3) == 0)
					map.data[x][y] = 242;
			}
		}
	}

	engine.keyState[SDL_SCANCODE_F1] = 0;
}

static void fillHorizontal(int block, int x, int y)
{
	bool moveLeft = true;
	bool moveRight = true;
	bool ok = true;

	int left = x;
	int right = x;

	if (map.data[x][y] == 0)
	{
		map.data[x][y] = block;

		while (ok)
		{
			if (moveLeft)
			{
				left--;
				if (left < 0)
				{
					left = 0;
				}
			}

			if (map.data[left][y] == 0)
			{
				map.data[left][y] = block;
			}
			else
			{
				moveLeft = false;
			}

			if (moveRight)
			{
				right++;

				if (right >= MAPWIDTH)
				{
					right = MAPWIDTH - 1;
				}
			}

			if (map.data[right][y] == 0)
			{
				map.data[right][y] = block;
			}
			else
			{
				moveRight = false;
			}

			if ((!moveLeft) && (!moveRight))
			{
				ok = false;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: mapeditor <filename>\n\n");
		exit(1);
	}

	config.engine = &engine;

	replayData.reset();

	atexit(cleanup);

	engine.useAudio = 0;
	engine.allowQuit = true;

	initSystem();

	FILE *fp = fopen(argv[1], "rb");
	if (!fp)
		newMap(argv[1]);
	else
		fclose(fp);

	game.setMapName(argv[1]);

	loadResources();

	int mapX = map.limitLeft / BRICKSIZE;
	int mapY = map.limitUp / BRICKSIZE;
	int x = 0;
	int y = 0;
	int allowMove = 0;

	int editing = 0;
	int currentMonster = 0;
	int currentItem = 0;
	int currentBlock = 1;

	SDL_Rect r;

	int MOVESPEED = 5;

	std::string statusline;

	while (true)
	{
		engine.getInput();
		config.populate();
		config.doPause();
		engine.doFrameLoop();

		graphics.updateScreen();
		graphics.animateSprites();
		graphics.drawBackground();

		engine.setPlayerPosition((mapX * BRICKSIZE) + 320, (mapY * BRICKSIZE) + 240, INT_MIN, INT_MAX, INT_MIN, INT_MAX);

		drawMap(mapX, mapY);
		doTrains();
		doSwitches();
		doItems();
		doBullets();
		doMIAs();
		drawEnemies();
		doObstacles();
		doEffects();
		doParticles();
		doTeleporters();
		doLineDefs();
		doTraps();

		x = engine.getMouseX();
		y = engine.getMouseY();

		x /= BRICKSIZE;
		y /= BRICKSIZE;

		r.x = x * BRICKSIZE;
		r.y = y * BRICKSIZE;
		r.w = BRICKSIZE;
		r.h = BRICKSIZE;

		switch (editing)
		{
		case 0:
			graphics.drawRect(r.x - 1, r.y - 1, 34, 1, graphics.yellow, graphics.screen);
			graphics.drawRect(r.x - 1, r.y - 1, 1, 34, graphics.yellow, graphics.screen);
			graphics.drawRect(r.x + BRICKSIZE, r.y - 1, 1, 34, graphics.yellow, graphics.screen);
			graphics.drawRect(r.x - 1, r.y + BRICKSIZE, 34, 1, graphics.yellow, graphics.screen);
			graphics.blit(graphics.tile[currentBlock], r.x, r.y, graphics.screen, false);
			if (engine.mouseLeft)
				map.data[mapX + x][mapY + y] = currentBlock;
			break;
		case 1:
			if (defEnemy[currentMonster].sprite[0])
				graphics.blit(defEnemy[currentMonster].getFaceImage(), r.x, r.y, graphics.screen, false);
			if (engine.mouseLeft)
			{
				addEnemy(defEnemy[currentMonster].name, (mapX + x) * BRICKSIZE, (mapY + y) * BRICKSIZE, 0);
				engine.mouseLeft = 0;
			}
			break;
		case 2:
			graphics.blit(defItem[currentItem].getFaceImage(), r.x, r.y, graphics.screen, false);
			if (engine.mouseLeft)
			{
				addItem(defItem[currentItem].id, defItem[currentItem].name, (mapX + x) * BRICKSIZE, (mapY + y) * BRICKSIZE, defItem[currentItem].sprite[0]->name, 0, defItem[currentItem].value, 0, true);
				engine.mouseLeft = 0;
			}
			break;
		}

		if (engine.mouseRight)
		{
			if (editing == 0)
				map.data[mapX + x][mapY + y] = MAP_AIR;
			if (editing == 1)
				deleteEnemy(mapX + x, mapY + y);
		}

		allowMove--;
		if (allowMove < 1)
			allowMove = 0;

		if (allowMove == 0)
		{
			if (engine.keyState[SDL_SCANCODE_UP])
			{
				mapY--;
				allowMove = MOVESPEED;
			}
			if (engine.keyState[SDL_SCANCODE_DOWN])
			{
				mapY++;
				allowMove = MOVESPEED;
			}
			if (engine.keyState[SDL_SCANCODE_LEFT])
			{
				mapX--;
				allowMove = MOVESPEED;
			}
			if (engine.keyState[SDL_SCANCODE_RIGHT])
			{
				mapX++;
				allowMove = MOVESPEED;
			}

			if (engine.keyState[SDL_SCANCODE_PAGEDOWN])
			{
				mapY += 10;
				allowMove = MOVESPEED;
			}
			if (engine.keyState[SDL_SCANCODE_PAGEUP])
			{
				mapY -= 10;
				allowMove = MOVESPEED;
			}

			if (engine.keyState[SDL_SCANCODE_1])
				editing = 0;
			if (engine.keyState[SDL_SCANCODE_2])
				editing = 1;
			if (engine.keyState[SDL_SCANCODE_3])
				editing = 2;

			if (engine.keyState[SDL_SCANCODE_0])
				fillHorizontal(currentBlock, mapX + x, mapY + y);

			if (engine.keyState[SDL_SCANCODE_F1])
				addTileDecoration();

			if (engine.keyState[SDL_SCANCODE_ESCAPE])
				break;
		}

		if (engine.keyState[SDL_SCANCODE_PERIOD])
		{
			switch (editing)
			{
			case 0:
				currentBlock = nextBlock(currentBlock, 1);
				break;
			case 1:
				currentMonster++;
				break;
			case 2:
				currentItem++;
				break;
			}

			engine.keyState[SDL_SCANCODE_PERIOD] = 0;
		}

		if (engine.keyState[SDL_SCANCODE_COMMA])
		{
			switch (editing)
			{
			case 0:
				currentBlock = nextBlock(currentBlock, -1);
				break;
			case 1:
				currentMonster--;
				break;
			case 2:
				currentItem--;
				break;
			}

			engine.keyState[SDL_SCANCODE_COMMA] = 0;
		}

		Math::limitInt(&currentMonster, 0, MAX_ENEMIES - 1);
		Math::limitInt(&currentItem, 0, MAX_ITEMS - 1);

		if (defEnemy[currentMonster].sprite[0] == nullptr)
			currentMonster = 0;

		if (defItem[currentItem].sprite[0] == nullptr)
			currentItem = 0;

		if (engine.keyState[SDL_SCANCODE_SPACE])
		{
			showMap(&mapX, &mapY);
		}

		if (engine.keyState[SDL_SCANCODE_S])
		{
			saveMap(game.mapName);
			engine.keyState[SDL_SCANCODE_S] = 0;
		}

		if (mapX < 0)
			mapX = 0;
		if (mapY < 0)
			mapY = 0;
		if (mapX > MAPWIDTH - 40)
			mapX = MAPWIDTH - 40;
		if (mapY > MAPHEIGHT - 30)
			mapY = MAPHEIGHT - 30;

		if (editing == 0)
			statusline = fmt::format("Index : {}:{} ; Screen {}:{} ; Tile {}", mapX + x, mapY + y, (mapX + x) * BRICKSIZE, (mapY + y) * BRICKSIZE, currentBlock);
		else if (editing == 1)
			statusline = fmt::format("Index : {}:{} ; Screen {}:{} ; {}", mapX + x, mapY + y, (mapX + x) * BRICKSIZE, (mapY + y) * BRICKSIZE, defEnemy[currentMonster].name);
		else if (editing == 2)
			statusline = fmt::format("Index : {}:{} ; Screen {}:{} ; {}", mapX + x, mapY + y, (mapX + x) * BRICKSIZE, (mapY + y) * BRICKSIZE, defItem[currentItem].name);

		r.x = 0;
		r.w = 640;
		r.h = 20;

		if (mapY < MAPHEIGHT - 30)
			r.y = 460;
		else
			r.y = 0;

		SDL_FillRect(graphics.screen, &r, graphics.black);

		graphics.drawString(statusline, 320, r.y + 5, true, graphics.screen);
	}

	return 0;
}

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

#include "map.h"

void drawMap()
{
	SDL_Rect r;

	int offsetX = (int)(engine.playerPosX);
	int offsetY = (int)(engine.playerPosY);

	map.offsetX = offsetX;
	map.offsetY = offsetY;

	Math::limitInt(&offsetX, 0, ((MAPWIDTH - 40) * BRICKSIZE));
	Math::limitInt(&offsetY, 0, ((MAPHEIGHT - 30) * BRICKSIZE));

	int mapx = offsetX >> BRICKSHIFT;
	int mapy = offsetY >> BRICKSHIFT;
	int brick = 0;

	for (int x = 0 ; x < 21 ; x++)
	{
		for (int y = 0 ; y < 16 ; y++)
		{
			brick = map.data[mapx + x][mapy + y];

			r.x = ((x * BRICKSIZE) - (offsetX & (BRICKSIZE - 1)));
			r.y = ((y * BRICKSIZE) - (offsetY & (BRICKSIZE - 1)));
			r.w = r.h = BRICKSIZE;

   			if ((brick >= MAP_BREAKABLE) && (brick < MAP_WATERANIM))
			{
				graphics.blit(graphics.tile[brick], r.x, r.y, graphics.screen, false);
				
				#if DEBUG
				if ((brick >= MAP_NORESET) && (brick < MAP_DECORATION))
				{
					graphics.drawRect(r.x, r.y, 32, 4, graphics.yellow, graphics.screen);
				}
				#endif
			}
		}
	}
}

void drawMapTopLayer()
{
	SDL_Rect r;

	int offsetX = map.offsetX;
	int offsetY = map.offsetY;

	Math::limitInt(&offsetX, 0, ((MAPWIDTH - 40) * BRICKSIZE));
	Math::limitInt(&offsetY, 0, ((MAPHEIGHT - 30) * BRICKSIZE));

	int mapx = offsetX >> BRICKSHIFT;
	int mapy = offsetY >> BRICKSHIFT;

	int brick;

	for (int x = 0 ; x < 21 ; x++)
	{
		for (int y = 0 ; y < 16 ; y++)
		{
			r.x = ((x * BRICKSIZE) - (offsetX & (BRICKSIZE - 1)));
			r.y = ((y * BRICKSIZE) - (offsetY & (BRICKSIZE - 1)));
			r.w = r.h = BRICKSIZE;

			brick = map.data[mapx + x][mapy + y];

			if (brick == 0)
			{
				continue;
			}

			if ((brick < MAP_BREAKABLE) || (brick >= MAP_WATERANIM))
			{
				if (brick == MAP_WATER)
				{
					if (map.data[mapx + x][mapy + y + 1] >= MAP_BREAKABLE)
					{
						addBubble((mapx + x) * BRICKSIZE, (mapy + y) * BRICKSIZE);
					}
				}

				if (brick == MAP_WATERANIM)
				{
					brick = graphics.getWaterAnim();
				}
				else if (brick == MAP_SLIME)
				{
					brick = graphics.getSlimeAnim();
				}
				else if ((brick >= MAP_LAVAANIM) && (brick < MAP_TOPLAYER))
				{
					map.data[mapx + x][mapy + y] = graphics.getLavaAnim(brick);
					brick = map.data[mapx + x][mapy + y];
				}

				graphics.blit(graphics.tile[brick], r.x, r.y, graphics.screen, false);
			}
		}
	}
}

void addBlips(List *blipList, int mapX, int mapY, int type)
{
	Sprite *blipType;
	Entity *ent;

	switch (type)
	{
		case 1:
			blipType = graphics.getSprite("MIAArrow", true);
			ent = (Entity*)map.miaList.getHead();
			break;
		case 2:
			blipType = graphics.getSprite("ItemArrow", true);
			ent = (Entity*)map.itemList.getHead();
			break;
		case 3:
			blipType = graphics.getSprite("EnemyArrow", true);
			ent = (Entity*)map.enemyList.getHead();
			break;
		default:
			return;
	}

	RadarBlip *blip;
	int x, y;

	while (ent->next != NULL)
	{
		ent = (Entity*)ent->next;

		if (ent->health <= 0)
			continue;

		if (type == 3)
		{
			if (!requiredEnemy(ent->name))
			{
				continue;
			}
		}

		// Items
		if (type == 2)
		{
			if ((ent->id < ITEM_MISC) || (ent->id == ITEM_MISC_INVISIBLE))
			{
				continue;
			}
		}

		x = (int)(ent->x + ent->width) >> BRICKSHIFT;
		y = (int)(ent->y + ent->height) >> BRICKSHIFT;

		x -= mapX;
		y -= mapY;

		x = (160) + (x * 5);
		y = (120) + (y * 5);

		if ((x >= 165) && (y >= 125) && (x <= 475) && (y <= 355))
		{
			blip = new RadarBlip();
			blip->set(x, y, type);
			blipList->add (blip);
		}
		else
		{
			if (y < 125)
				graphics.blit(blipType->image[0], 220 + (type * 50), 100, graphics.screen, true);
			if (x > 475)
				graphics.blit(blipType->image[1], 510, 140 + (type * 50), graphics.screen, true);
			if (y > 355)
				graphics.blit(blipType->image[2], 220 + (type * 50), 380, graphics.screen, true);
			if (x < 165)
				graphics.blit(blipType->image[3], 125, 140 + (type * 50), graphics.screen, true);
		}
	}
}

void addMiniMapDoors(SDL_Surface *panel, int mapX, int mapY)
{
	Train *train = (Train*)map.trainList.getHead();

	int x, y;
	int width, height, color;

	while (train->next != NULL)
	{
		train = (Train*)train->next;
		
		width = 5;
		height = 5;
		color = graphics.white;
		
		x = (int)train->x >> BRICKSHIFT;
		
		y = (int)train->y >> BRICKSHIFT;
		
		if ((x >= mapX) && (x <= mapX + 64) && (y >= mapY) && (y <= mapY + 48))
		{
			x -= mapX;
			y -= mapY;
			
			if (train->type == TR_TRAIN)
			{
				width = 10;
			}
			else if ((train->type >= TR_SLIDEDOOR) && (train->type <= TR_BRONZE_SLIDEDOOR))
			{
				width = 10;
			}
			else
			{
				height = 10;
			}
			
			switch (train->type)
			{
				case TR_GOLD_DOOR:
				case TR_GOLD_SLIDEDOOR:
					color = graphics.yellow;
					break;
					
				case TR_SILVER_DOOR:
				case TR_SILVER_SLIDEDOOR:
					color = graphics.grey;
					break;
					
				case TR_BRONZE_DOOR:
				case TR_BRONZE_SLIDEDOOR:
					color = SDL_MapRGB(graphics.screen->format, 0xff, 0x99, 0x00);
					break;
					
				case TR_DOOR:
				case TR_LOCKED_DOOR:
				case TR_LOCKED_SLIDEDOOR:
					color = SDL_MapRGB(graphics.screen->format, 0xff, 0x00, 0xff);
					break;
			}
			
			graphics.drawRect(x * 5, y * 5, width, height, color, panel);
		}
	}
}

void showMap(int centerX, int centerY)
{
	char string[100];
	int x1, y1, x2, y2;

	x1 = centerX - 32;
	x2 = centerX + 32;
	y1 = centerY - 24;
	y2 = centerY + 24;
	
	int minX = (map.limitLeft >> BRICKSHIFT);
	int maxX = (map.limitRight >> BRICKSHIFT) - 44;
	int minY = (map.limitUp >> BRICKSHIFT);
	int maxY = (map.limitDown >> BRICKSHIFT) - 33;

	Math::limitInt(&x1, minX, maxX);
	Math::limitInt(&x2, minX, maxX);
	Math::limitInt(&y1, minY, maxY);
	Math::limitInt(&y2, minY, maxY);

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.updateScreen();
	SDL_Delay(500);
	
	SDL_Surface *panel = graphics.createSurface(320, 240);
	SDL_Surface *background = graphics.loadImage("gfx/main/mapBackground.png");
	SDL_SetAlpha(background, 130);
	
	graphics.blit(background, 0, 0, panel, false);
	
	for (int y = 0 ; y < 48 ; y++)
	{
		for (int x = 0 ; x < 64 ; x++)
		{
			if (map.data[x1 + x][y1 + y] == MAP_WATER)
			{
				graphics.drawRect(x * 5, y * 5, 5, 5, graphics.blue, panel);
			}
		}
	}
	
	addMiniMapDoors(panel, x1, y1);

	for (int y = 0 ; y < 48 ; y++)
	{
		for (int x = 0 ; x < 64 ; x++)
		{
			int color = graphics.black;

			if (map.data[x1 + x][y1 + y] == MAP_AIR)
			{
				color = graphics.black;
			}
			else if (map.data[x1 + x][y1 + y] == MAP_WATER)
			{
				color = graphics.black;
			}
			else if (map.data[x1 + x][y1 + y] == MAP_SLIME)
			{
				color = graphics.green;
			}
			else if (map.data[x1 + x][y1 + y] == MAP_LAVA)
			{
				color = graphics.red;
			}
			else if (map.data[x1 + x][y1 + y] >= MAP_DECORATION)
			{
				color = graphics.black;
				
				if (map.data[x1 + x][y1 + y - 1] == MAP_WATER)
				{
					color = graphics.blue;
				}
				else if (map.data[x1 + x][y1 + y + 1] == MAP_WATER)
				{
					color = graphics.blue;
				}
			}
			else if (map.data[x1 + x][y1 + y] < MAP_WATERANIM)
			{
				color = graphics.darkGreen;
			}

			if (color != graphics.black)
			{
				graphics.drawRect(x * 5, y * 5, 5, 5, color, panel);
			}
		}
	}

	graphics.blit(background, 0, 0, graphics.screen, false);
	SDL_FreeSurface(background);

	List blipList;

	RadarBlip *blip = new RadarBlip();
	blip->set(160 + ((centerX - x1) * 5), 120 + ((centerY - y1) * 5), 0);
	blipList.add(blip);

	addBlips(&blipList, x1, y1, 1);
	addBlips(&blipList, x1, y1, 2);
	addBlips(&blipList, x1, y1, 3);
	
	Sprite *enemySignal = graphics.getSprite("EnemySignal", true);
	Sprite *miaSignal = graphics.getSprite("MIASignal", true);
	Sprite *bobSignal = graphics.getSprite("BobSignal", true);
	Sprite *itemSignal = graphics.getSprite("ItemSignal", true);

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	graphics.setFontSize(3);
	graphics.drawString(_(game.stageName), 320, 30, TXT_CENTERED, graphics.screen);

	graphics.setFontSize(0);
	
	graphics.drawRect(160, 414, 7, 7, graphics.yellow, graphics.white, graphics.screen);
	graphics.drawString(_("MIAs"), 175, 410, TXT_LEFT, graphics.screen);

	graphics.drawRect(290, 414, 7, 7, graphics.blue, graphics.white, graphics.screen);
	graphics.drawString(_("Items"), 305, 410, TXT_LEFT, graphics.screen);

	graphics.drawRect(415, 414, 7, 7, graphics.red, graphics.white, graphics.screen);
	graphics.drawString(_("Enemies"), 430, 410, TXT_LEFT, graphics.screen);

	graphics.setFontSize(1);
	snprintf(string, sizeof string, "%s - %.2d:%.2d:%.2d", _("Mission Time"), game.currentMissionHours, game.currentMissionMinutes, game.currentMissionSeconds);
	graphics.drawString(string, 320, 60, TXT_CENTERED, graphics.screen);
	graphics.drawString(_("Press Button to Continue..."), 320, 450, TXT_CENTERED, graphics.screen);

	engine.flushInput();
	engine.clearInput();

	doMusicInfo(-1);

	while (true)
	{
		engine.getInput();
		config.populate();
		graphics.updateScreen();
		graphics.animateSprites();

		graphics.drawRect(160, 120, 320, 240, graphics.black, graphics.white, graphics.screen);
		graphics.blit(panel, 160, 120, graphics.screen, false);

		if ((config.isControl(CONTROL::MAP)) || (config.isControl(CONTROL::PAUSE)) || (engine.keyState[SDL_SCANCODE_ESCAPE]))
		{
			engine.keyState[SDL_SCANCODE_ESCAPE] = 0;
			config.resetControl(CONTROL::MAP);
			config.resetControl(CONTROL::PAUSE);
			break;
		}

		blip = (RadarBlip*)blipList.getHead();

		while (blip->next != NULL)
		{
			blip = (RadarBlip*)blip->next;

			switch (blip->type)
			{
				case 0:
					graphics.blit(bobSignal->getCurrentFrame(), blip->x, blip->y, graphics.screen, true);
					break;
				case 1:
					graphics.blit(miaSignal->getCurrentFrame(), blip->x, blip->y, graphics.screen, true);
					break;
				case 2:
					graphics.blit(itemSignal->getCurrentFrame(), blip->x, blip->y, graphics.screen, true);
					break;
				case 3:
					graphics.blit(enemySignal->getCurrentFrame(), blip->x, blip->y, graphics.screen, true);
					break;
			}
		}

		SDL_Delay(16);
	}

	blipList.clear();

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.updateScreen();
	SDL_Delay(500);

	engine.flushInput();
	engine.clearInput();
}

void evaluateMapAttribute(Entity *ent, int mapAttribute)
{
	switch (mapAttribute)
	{
		case MAP_AIR_WALL_1:
		case MAP_AIR_WALL_2:
		case MAP_AIR_WALL_3:
		case MAP_AIR_CEILING_1:
			mapAttribute = MAP_AIR;
			break;
		case MAP_AIR_WALL_4:
			if (map.isCavesTileset)
			{
				mapAttribute = MAP_AIR;
			}
			break;
		case MAP_AIR_CEILING_2:
			if (map.isGrasslandsTileset)
			{
				mapAttribute = MAP_AIR;
			}
			else if (map.isCavesTileset)
			{
				mapAttribute = MAP_WATER;
			}
			break;
		case MAP_WATER_WALL:
			if (map.isCavesTileset)
			{
				mapAttribute = MAP_WATER;
			}
			break;
	}

	switch (mapAttribute)
	{
		case MAP_AIR:
			if ((ent->environment != ENV_AIR) && (!(ent->flags & ENT_INANIMATE)))
			{
				if (!(ent->flags & ENT_SWIMS))
				{
					if (ent->dy < 0)
					{
						ent->dy = PLAYER_JUMP_SPEED;
					}

					if (ent == &player)
					{
						if ((ent == &player) && ((game.hasAquaLung) || (engine.cheatExtras)))
						{
							player.setSprites(graphics.getSprite("BobRight", true), graphics.getSprite("BobLeft", true), graphics.getSprite("BobSpin", true));
						}
					}

					ent->environment = ENV_AIR;
					
					if (ent->dy < 0)
					{
						audio.playSound(SND_WATEROUT, CH_TOUCH, ent->x);
					}

					ent->checkEnvironment();
				}
			}
			ent->falling = true;
		case MAP_SOLID:
			break;
		case MAP_WATER:
		case MAP_SLIME:
		case MAP_LAVA:

			ent->falling = false;

			if (ent->environment == ENV_AIR)
			{
				audio.playSound(SND_WATERIN, CH_TOUCH, ent->x);
				if ((mapAttribute == MAP_SLIME) || (mapAttribute == MAP_LAVA))
					ent->thinktime = 1;
			}
			
			// On ice levels water is harmful (because it's very very cold!)
			if ((map.isIceLevel) && (mapAttribute == MAP_WATER))
			{
				mapAttribute = MAP_LAVA;
				ent->thinktime = 1;
			}

			if (mapAttribute == MAP_WATER)
			{
				ent->environment = ENV_WATER;
				if ((ent == &player) && ((game.hasAquaLung) || (engine.cheatExtras)))
				{
					player.setSprites(graphics.getSprite("AquaBobRight", true), graphics.getSprite("AquaBobLeft", true), graphics.getSprite("AquaBobSpin", true));
				}
			}
			else if (mapAttribute == MAP_SLIME)
			{
				ent->environment = ENV_SLIME;
			}
			else if (mapAttribute == MAP_LAVA)
			{
				ent->environment = ENV_LAVA;
			}
			break;
	}
}

void raiseWaterLevel()
{
	if (map.waterLevel == map.requiredWaterLevel)
	{
		return;
	}

	int y = (int)map.waterLevel;

	if ((int)map.waterLevel != map.requiredWaterLevel)
	{
		for (int x = 0 ; x < MAPWIDTH ; x++)
		{
			if ((map.data[x][y] == MAP_AIR) || (map.isLiquid(x, y)))
			{
				map.data[x][y] = MAP_WATER;
			}
			
			if ((map.data[x][y] >= MAP_DECORATION) && (map.data[x][y] <= MAP_WATERANIM))
			{
				map.data[x][y] = MAP_WATER;
			}
			
			if (map.data[x][y] >= MAP_TOPLAYER)
			{
				map.data[x][y] = MAP_WATER;
			}
			
			if (map.data[x][y - 1] >= MAP_TOPLAYER)
			{
				map.data[x][y] = MAP_WATER;
			}

			if ((map.data[x][y - 1] == MAP_AIR) || (map.isLiquid(x, y - 1)))
			{
				map.data[x][y - 1] = MAP_WATERANIM;
			}
			
			if ((map.data[x][y - 1] >= MAP_DECORATION) && (map.data[x][y - 1] <= MAP_WATERANIM))
			{
				map.data[x][y - 1] = MAP_WATERANIM;
			}
		}

		map.waterLevel -= 0.1;

		int x = (int)(player.x + player.dx) >> BRICKSHIFT;
		int y = (int)(player.y + player.height - 1) >> BRICKSHIFT;

		int mapAttribute = map.data[x][y];

		if ((mapAttribute == MAP_WATER) && (player.environment == MAP_AIR))
		{
			evaluateMapAttribute(&player, mapAttribute);
		}
	}
}

void doWind()
{
	map.windChangeTime--;
	
	if (map.windChangeTime <= 0)
	{
		map.windPower = 0;
		
		if (Math::rrand(0, 1) == 0)
			map.windPower = Math::rrand(-3, 3);
			
		// change wind time
		map.windChangeTime = Math::rrand(60, 600);
	}
	
	addWindParticles();
}

void parseMapDataLine(const char *line, int y)
{
	int tileIndex = 0;
	int x = 0;

	while (true)
	{
		sscanf(line, "%d", &tileIndex);

		map.data[x][y] = tileIndex;

		while (true)
		{
			line++;

			if (*line == ' ')
				break;
		}

		x++;

		if (x == MAPWIDTH)
			break;
	}
}

bool loadMapData(const char *filename)
{
	map.clear();

	if (!engine.loadData(filename))
		graphics.showErrorAndExit("The requested map '%s' was not found.", filename);

	char *token = strtok((char*)engine.dataBuffer, "\n");
	parseMapDataLine(token, 0);

	int y = 1;

	while (true)
	{
		token = strtok(NULL, "\n");

		parseMapDataLine(token, y);

		y++;
		if (y == MAPHEIGHT)
			break;
	}

	getMapTokens();

	adjustObjectives();
	initMIAPhrases();

	return true;
}

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

static bool okayToSpawnEnemy(std::string name, int x, int y)
{
	// Don't summon other monsters!!
	if (map.fightingGaldov)
	{
		return false;
	}

	if (engine.devNoMonsters)
	{
		return false;
	}

	// stop enemies from appearing in the middile of doors
	for (auto &&train: map.trains)
	{
		// assume enemy is 20 x 20 pixels (most are at least) and trains are 64 x 64
		if (Collision::collision(x * BRICKSHIFT, y * BRICKSHIFT, 20, 20, train.x, train.y, 64, 64))
		{
			debug("Couldn't add enemy '{}' - Collided with train\n", name);
			return false;
		}
	}

	Entity *enemy = getDefinedEnemy(name);

	if (map.isLiquid(x, y))
	{
		if (enemy->flags & ENT_SWIMS)
		{
			return true;
		}

		debug("Couldn't add enemy '{}' - Would drown\n", name);

		return false;
	}
	else
	{
		if (enemy->flags & ENT_SWIMS)
		{
			debug("Couldn't add enemy '{}' - Not in water\n", name);

			return false;
		}
	}

	if (enemy->flags & ENT_FLIES)
	{
		return true;
	}

	for (int i = 0; i < 30; i++)
	{
		y++;

		if (y > map.limitDown)
		{
			debug("Couldn't add enemy '{}' - Outside map limits\n", name);

			return false;
		}

		if (map.isLiquid(x, y))
		{
			debug("Couldn't add enemy '{}' - Would drown after free fall\n", name);

			return false;
		}

		if (map.isSolid(x, y))
		{
			return true;
		}
	}

	debug("Couldn't add enemy '{}' - Just couldn't!\n", name);

	return false;
}

void doSpawnPoints()
{
	int x, y;

	std::string enemy;

	for (auto &&sp: map.spawns)
	{
		sp.think();

		if (!sp.active)
		{
			continue;
		}

		if (sp.spawnType == SPW_HAZARD)
		{
			if (sp.spawnSubType == HAZARD_ROCKFALL)
			{
				x = (int)fabs(sp.x - player.x);
				y = (int)fabs(sp.y - player.y);

				if ((x <= 640) && (y <= 480))
				{
					engine.setPlayerPosition((int)player.x + Math::rrand(-MAP_SHAKEAMOUNT, MAP_SHAKEAMOUNT), (int)player.y + Math::rrand(-MAP_SHAKEAMOUNT, MAP_SHAKEAMOUNT), map.limitLeft, map.limitRight, map.limitUp, map.limitDown);
				}
			}
			else if (sp.spawnSubType == HAZARD_STALAGTITES)
			{
				x = (int)fabs(sp.x - player.x);
				y = (int)fabs(sp.y - player.y);

				if ((x <= 320) && (y <= 480))
				{
					engine.setPlayerPosition((int)player.x + Math::rrand(-MAP_SHAKEAMOUNT, MAP_SHAKEAMOUNT), (int)player.y + Math::rrand(-MAP_SHAKEAMOUNT, MAP_SHAKEAMOUNT), map.limitLeft, map.limitRight, map.limitUp, map.limitDown);
				}
			}
		}

		if (sp.readyToSpawn())
		{
			if ((sp.spawnType != SPW_ENEMY) && (sp.spawnType != SPW_ITEM))
			{
				// If the player is too far away, don't spawn (unless it's random enemy / item spawning)
				x = (int)fabs(sp.x - player.x);
				y = (int)fabs(sp.y - player.y);

				if ((x > 700) || (y > 500))
				{
					sp.reset();
					continue;
				}
			}

			switch (sp.spawnType)
			{
			case SPW_HAZARD:
				switch (sp.spawnSubType)
				{
				case HAZARD_LAVABALL:
					engine.world.place(sp.x, sp.y);
					engine.world.currentWeapon = &weapon[WP_LAVABALL1];
					addBullet(engine.world, Math::rrand(-5, 5), Math::rrand(-(5 + game.skill), -(2 + game.skill)));
					break;
				case HAZARD_ROCKFALL:
					engine.world.place(sp.x, sp.y);
					engine.world.currentWeapon = &weapon[WP_ROCK1];
					addBullet(engine.world, Math::rrand(-2, 2), Math::rrand(0, 2));
					break;
				case HAZARD_BOMBS:
					engine.world.place(sp.x, sp.y);
					engine.world.currentWeapon = &weapon[WP_BOMB];
					addBullet(engine.world, Math::rrand(-2, 2), Math::rrand(0, 2));
					break;
				case HAZARD_EXPLOSION:
					x = sp.x + Math::rrand(-128, 128);
					y = sp.y + Math::rrand(-128, 128);
					addExplosion(x, y, 50, engine.world);
					x = x >> BRICKSHIFT;
					y = y >> BRICKSHIFT;
					if (map.isSolid(x, y))
					{
						int waterLevel = (int)map.waterLevel;
						if (waterLevel == -1 || y < waterLevel)
						{
							map.data[x][y] = MAP_AIR;
						}
						else if (y == waterLevel)
						{
							map.data[x][y] = MAP_WATERANIM;
						}
						else
						{
							map.data[x][y] = MAP_WATER;
						}
					}
					break;
				case HAZARD_POWERBULLETS:
					engine.world.place(sp.x, sp.y);
					engine.world.currentWeapon = &weapon[WP_SHELLS];
					x = engine.world.currentWeapon->dx;
					if (player.x < sp.x)
						x = -x;
					addBullet(engine.world, x, 0);
					break;
				case HAZARD_STALAGTITES:
					engine.world.place(sp.x, sp.y);
					engine.world.currentWeapon = &weapon[WP_STALAGTITE];
					addBullet(engine.world, 0, 2);
					break;
				default:
					debug("Spawn Type is unknown!\n");
					break;
				}
				break;

			case SPW_ENEMY:
				if (game.missionOverReason != MIS_INPROGRESS)
					break;

				enemy = map.getSpawnableEnemy();

				x = (int)(player.x) >> BRICKSHIFT;
				y = (int)(player.y) >> BRICKSHIFT;

				x += Math::rrand(-10, 10);
				y += Math::rrand(-10, 10);

				if ((x >= 0) && (y >= 0) && (x < MAPWIDTH) && (y < MAPHEIGHT))
				{
					if ((map.data[x][y] == MAP_AIR) || (map.data[x][y] == MAP_WATER))
					{
						if (okayToSpawnEnemy(enemy, x, y))
						{
							x = x << BRICKSHIFT;
							y = y << BRICKSHIFT;
							addEnemy(enemy, x, y, ENT_SPAWNED);
							addTeleportParticles(x, y, 25, SND_TELEPORT2);
						}
					}
				}

				break;

			case SPW_ITEM:
				if (game.missionOverReason != MIS_INPROGRESS)
					break;

				x = (int)(player.x) >> BRICKSHIFT;
				y = (int)(player.y) >> BRICKSHIFT;

				x += Math::rrand(-10, 10);
				y += Math::rrand(-10, 10);

				if ((x >= 0) && (y >= 0))
				{
					if (map.data[x][y] == MAP_AIR)
					{
						x = x << BRICKSHIFT;
						y = y << BRICKSHIFT;
						dropHelperItems(x, y);
						addTeleportParticles(x, y, 5, SND_TELEPORT2);
					}
				}

				break;

			case SPW_BOSSBULLET:

				if (game.missionOverReason != MIS_INPROGRESS)
					break;

				if (map.boss[sp.spawnSubType]->health > 0)
				{
					map.boss[sp.spawnSubType]->active = true;
				}

				break;

			default:
				debug("Spawn Type is unknown!\n");
				break;
			}

			sp.reset();

			if (sp.spawnType == SPW_ENEMY)
			{
				if ((Math::prand() % (game.skill + 2)) > 0)
				{
					sp.requiredInterval = Math::rrand(1, 30);
				}
			}
		}
	}
}

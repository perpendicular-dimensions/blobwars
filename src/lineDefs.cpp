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

#include "lineDefs.h"

void addLineDef(const char *name, const char *linkName, const char *message, int x, int y, int width, int height, bool active)
{
	LineDef *lineDef = new LineDef();

	lineDef->set(name, linkName, message, x, y, width, height);
	lineDef->activated = active;

	map.addLineDef(lineDef);
}

void showMessageLineDef(const char *linkName, bool show)
{
	LineDef *lineDef = (LineDef*)map.lineList.getHead();

	while (lineDef->next != NULL)
	{
		lineDef = (LineDef*)lineDef->next;

		if (strcmp(lineDef->linkName, linkName) == 0)
			lineDef->activated = show;
	}
}

void doLineDefs()
{
	LineDef *lineDef = (LineDef*)map.lineList.getHead();

	int x, y, absX, absY;

	while (lineDef->next != NULL)
	{
		lineDef = (LineDef*)lineDef->next;

		if (lineDef->activated)
			continue;

		x = (int)(lineDef->x - engine.playerPosX);
		y = (int)(lineDef->y - engine.playerPosY);

		absX = abs(x);
		absY = abs(y);

		if ((absX < 800) && (absY < 600))
		{
			#if DEBUG
				graphics.drawRect(x, y, lineDef->width, lineDef->height, graphics.red, graphics.screen);
			#endif

			if (player.health < 1)
			{
				continue;
			}

			if (Collision::collision(player.x + player.dx, player.y + player.dy, player.width, player.height, lineDef->x, lineDef->y, lineDef->width, lineDef->height))
			{
				if ((strcmp(lineDef->name, "Exit") == 0) && (!requiredObjectivesCompleted()))
				{
					config.resetControl(CONTROL::LEFT);
					config.resetControl(CONTROL::RIGHT);

					if (player.dx > 0) player.x = (lineDef->x - player.width) - 2;
					if (player.dx < 0) player.x = (lineDef->x + lineDef->width + 2);
					
					player.dx = 0;
					
					if (!map.isBossMission)
					{
						engine.setInfoMessage("Can't Exit Yet - Objectives Not Met", 1, INFO_HINT);
					}
					
					continue;
				}

				if (strcmp(lineDef->name, "Message") == 0)
				{
					engine.setInfoMessage(lineDef->activateMessage, 1, INFO_HINT);
					if (strcmp(lineDef->linkName, "@none@") == 0)
					{
						lineDef->activated = true;
					}
				}
				else if (strcmp(lineDef->name, "PlayerOut") == 0)
				{
					if (game.missionOver == 0)
					{
						player.health--;
					}

					if (player.health > 0)
					{
						game.setMissionOver(MIS_PLAYEROUT);
					}
					else
					{
						game.setMissionOver(MIS_PLAYERDEAD);
					}
				}
				else if (strcmp(lineDef->name, "CheckPoint") == 0)
				{
					game.setObjectiveCheckPoint();
					engine.setInfoMessage("Checkpoint Reached", 9, INFO_OBJECTIVE);
					lineDef->activated = true;
				}
				else if (strcmp(lineDef->name, "ActivateBoss") == 0)
				{
					map.mainBossPart->active = true;
					lineDef->activated = true;
				}
				else
				{
					checkObjectives(lineDef->name, true);
					activateTrigger(lineDef->linkName, lineDef->activateMessage, true);
					lineDef->activated = true;
					
					if (strcmp(lineDef->name, "StealCrystal") == 0)
					{
						stealCrystal();
						lineDef->activated = true;
					}
				}
			}
		}

	}
}

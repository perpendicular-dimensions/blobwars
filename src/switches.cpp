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

#include "switches.h"

void checkSwitchContact(Entity *ent)
{
	Switch *swt = (Switch*)map.switchList.getHead();
	bool okayToActivate = false;

	char message[256];

	while (swt->next != NULL)
	{
		swt = (Switch*)swt->next;
		
		if (swt->type == SWT_USED)
			continue;

		if (swt->type == SWT_NORMAL)
		{
			if ((ent != &player) && (!(ent->flags & ENT_BULLET)))
				continue;

			if ((ent->flags & ENT_BULLET) && (ent->id != WP_GRENADES))
				continue;
		}
		
		if ((swt->type == SWT_RESET) && (ent != &player))
			continue;
			
		if ((swt->type == SWT_WATERLEVEL) && (ent != &player))
			continue;

		if (Collision::collision(ent, swt))
		{
			okayToActivate = false;

			if (strcmp(swt->requiredObjectName, "@none@") == 0)
			{
				okayToActivate = true;
			}
			else if (ent == &player)
			{
				if (carryingItem(swt->requiredObjectName))
				{
					okayToActivate = true;
					
					if (swt->type == SWT_PRESSURE)
					{
						strlcpy(swt->requiredObjectName, "@none@", sizeof swt->requiredObjectName);
					}
					
					checkObjectives(swt->name, true);
					strlcpy(swt->name, "Switch", sizeof swt->name);
				}
				else
				{
					snprintf(message, sizeof message, "%s required", swt->requiredObjectName);
					engine.setInfoMessage(message, 1, INFO_HINT);
				}
			}

			if (okayToActivate)
			{	
				if (swt->health == 0)
				{
					if ((swt->type == SWT_NORMAL) || (swt->type == SWT_WATERLEVEL))
					{
						audio.playSound(SND_SWITCH1, CH_TOUCH, swt->x);
						swt->activated = !swt->activated;
						activateTrigger(swt->linkName, swt->activateMessage, swt->activated);
						swt->health = 1;
						swt->type = SWT_USED;
					}
					else if (swt->type == SWT_TOGGLE)
					{
						audio.playSound(SND_SWITCH1, CH_TOUCH, swt->x);
						activateTrigger(swt->linkName, swt->activateMessage, !swt->activated);
						swt->activated = !swt->activated;
					}
					else if (swt->type == SWT_PRESSURE)
					{
						audio.playSound(SND_SWITCH1, CH_TOUCH, swt->x);
						swt->activated = true;
						activateTrigger(swt->linkName, swt->activateMessage, true);
						swt->health = 2;
					}
					else if ((swt->type == SWT_TIMED) || (swt->type == SWT_RESET))
					{
						audio.playSound(SND_SWITCH1, CH_TOUCH, swt->x);
						activateTrigger(swt->linkName, swt->activateMessage, true);
						swt->activated = !swt->activated;
						swt->health = 240;
					}
				}

				if ((swt->type == SWT_TOGGLE) || (swt->type == SWT_PRESSURE))
					swt->health = 2;
			}
		}
	}
}

void doSwitches()
{
	Switch *swt = (Switch*)map.switchList.getHead();

	int x, y, absX, absY;

	while (swt->next != NULL)
	{
		swt = (Switch*)swt->next;

		x = (int)(swt->x - engine.playerPosX);
		y = (int)(swt->y - engine.playerPosY);

		absX = abs(x);
		absY = abs(y);

		if ((absX < 800) && (absY < 600))
		{
			if (!swt->activated)
				graphics.blit(graphics.getSprite("SwitchOff", true)->getCurrentFrame(), x, y, graphics.screen, false);
			else
				graphics.blit(graphics.getSprite("SwitchOn", true)->getCurrentFrame(), x, y, graphics.screen, false);
		}

		if ((swt->type != SWT_NORMAL) && (swt->type != SWT_WATERLEVEL) && (swt->type != SWT_USED))
		{
			if (swt->health > 0)
			{
				swt->health--;
				if (swt->health == 0)
				{
					if (swt->type != SWT_TOGGLE)
					{
						swt->activated = false;
						activateTrigger(swt->linkName, swt->activateMessage, false);
					}
				}
			}
		}
	}
}

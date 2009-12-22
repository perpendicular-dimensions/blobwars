/*
Copyright (C) 2004 Parallel Realities

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

#include "info.h"

void doTimeRemaining()
{		
	map.remainingSeconds--;
	
	if ((map.remainingMinutes == 0) && (map.remainingSeconds <= 10) && (map.remainingSeconds > 0))
	{
		audio.playSound(SND_CLOCK, CH_TOUCH);
	}
	
	if (map.remainingSeconds < 0)
	{
		if (map.remainingMinutes > 0)
		{
			map.remainingSeconds = 59;
			map.remainingMinutes--;
		}
	}
	
	if ((map.remainingSeconds == 0) && (map.remainingMinutes == 0))
	{
		player.health = 0;
		player.immune = 0;
		Math::removeBit(&player.flags, ENT_FLIES);
		game.setMissionOver(MIS_TIMEUP);
	}
	
	replayData.header.time++;
}

void doStatusBar()
{
	graphics.setFontSize(0);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	char string[1024];

	graphics.blit(graphics.infoBar, 0, 0, graphics.screen, false);

	graphics.drawString(_("Health"), 50, 5, TXT_RIGHT, graphics.screen);

	for (int i = 0 ; i < MAX_HEALTH ; i++)
	{
		if (i < player.health)
			graphics.blit(graphics.getSprite("HealthBlock", true)->getCurrentFrame(), 60 + (i * 15), 7, graphics.screen, false);
		else
			graphics.blit(graphics.getSprite("HealthBlockEmpty", true)->getCurrentFrame(), 60 + (i * 15), 7, graphics.screen, false);
	}

	if (player.health <= 3)
		if (engine.getFrameLoop() < 30)
			for (int i = 0 ; i < player.health ; i++)
				graphics.blit(graphics.getSprite("WarningBlock", true)->getCurrentFrame(), 60 + (i * 15), 7, graphics.screen, false);

	if ((!game.hasAquaLung) && (!engine.cheatExtras))
	{
		graphics.drawString(_("Oxygen"), 305, 5, TXT_RIGHT, graphics.screen);

		for (int i = 0 ; i < 7 ; i++)
		{
			if (i < player.oxygen)
				graphics.blit(graphics.getSprite("OxygenBlock", true)->getCurrentFrame(), 315 + (i * 15), 7, graphics.screen, false);
			else
				graphics.blit(graphics.getSprite("OxygenBlockEmpty", true)->getCurrentFrame(), 315 + (i * 15), 7, graphics.screen, false);
		}

		if (player.oxygen <= 3)
			if (engine.getFrameLoop() < 30)
				for (int i = 0 ; i < player.oxygen ; i++)
					graphics.blit(graphics.getSprite("WarningBlock", true)->getCurrentFrame(), 315 + (i * 15), 7, graphics.screen, false);
	}
	else if ((game.hasJetPack) || (engine.cheatExtras))
	{
		graphics.drawString(_("Jetpack"), 305, 5, TXT_RIGHT, graphics.screen);

		for (int i = 0 ; i < 7 ; i++)
		{
			if (i < player.fuel)
				graphics.blit(graphics.getSprite("OxygenBlock", true)->getCurrentFrame(), 315 + (i * 15), 7, graphics.screen, false);
			else
				graphics.blit(graphics.getSprite("OxygenBlockEmpty", true)->getCurrentFrame(), 315 + (i * 15), 7, graphics.screen, false);
		}

		if ((player.fuel < 3) && (!(player.flags & ENT_FLIES)))
			if (engine.getFrameLoop() < 30)
				for (int i = 0 ; i < player.fuel ; i++)
					graphics.blit(graphics.getSprite("WarningBlock", true)->getCurrentFrame(), 315 + (i * 15), 7, graphics.screen, false);
	}

	if ((map.mainBossPart == NULL || strstr(engine.message, "Aqua") || strstr(engine.message, "Jet")) && (game.missionOverReason != MIS_GAMECOMPLETE))
	{
		if (engine.messageTime > -1)
		{
			switch (engine.messageType)
			{
				case INFO_NORMAL:
					graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
					break;
				case INFO_OBJECTIVE:
					graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
					break;
				case INFO_HINT:
					graphics.setFontColor(0xff, 0xaa, 0x00, 0x00, 0x00, 0x00);
					break;
				case INFO_ACTIVATE:
					graphics.setFontColor(0x00, 0xff, 0xff, 0x00, 0x00, 0x00);
					break;
				case INFO_BAD:
					graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
					break;
			}
	
			graphics.blit(graphics.infoBar, 0, 455, graphics.screen, false);
			graphics.drawString(_(engine.message), 320, 466, true, graphics.screen);
			
			engine.messageTime--;
			if (engine.messageTime == -1)
			{
				engine.messagePriority = -1;
				strcpy(engine.message, "");
			}
	
			graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		}
	}
	else
	{
		if (map.mainBossPart != NULL)
		{
			graphics.blit(graphics.infoBar, 0, 455, graphics.screen, false);
			
			graphics.drawString(_(map.mainBossPart->name), 255, 460, TXT_RIGHT, graphics.screen);
			graphics.drawRect(265 - 1, 463 - 1, 200 + 2, 10 + 2, graphics.white, graphics.screen);
			graphics.drawRect(265, 463, 200, 10, graphics.black, graphics.screen);
			
			if (map.mainBossPart->health > 0)
			{
				graphics.drawRect(265, 463, (int)(map.mainBossPart->health * map.bossEnergyMeterBit), 10, graphics.red, graphics.screen);
			}
		}
	}

	sprintf(string, "%s %s", _("Weapon:"), _(player.currentWeapon->name));
	graphics.drawString(string, 630, 5, TXT_RIGHT, graphics.screen);
	
	if (game.skill == 3)
	{
		sprintf(string, _("Time Remaining: %.2d:%.2d"), map.remainingMinutes, map.remainingSeconds);
		graphics.blit(graphics.infoBar, 0, 25, graphics.screen, false);
		
		if ((map.remainingMinutes > 0) || (map.remainingSeconds > 0))
		{
			if (map.remainingMinutes == 0)
			{
				if (map.remainingSeconds > 0)
				{
					if (map.remainingSeconds <= 10)
					{
						graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
					}
					else if (map.remainingSeconds <= 30)
					{
						graphics.setFontColor(0xff, 0xff, 0x00, 0x00, 0x00, 0x00);
					}
				}
			}
			graphics.drawString(string, 320, 35, TXT_CENTERED, graphics.screen);
		}
		else
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			graphics.setFontSize(3);
			graphics.drawString(_("Mission Failed! Time Up!"), 320, 220, TXT_CENTERED, graphics.screen);
			graphics.setFontSize(0);
			game.canContinue = 0;
		}
	}
}

void doPauseInfo()
{
	int col1, col2, y;

	col1 = 310;
	col2 = 330;
	y = 60;

	graphics.fade(130);

	char string[1024];
	strcpy(string, "");
	
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	//#if !USEPAK
	sprintf(string, _("Position = %d:%d"), (int)player.x, (int)player.y);
	graphics.drawString(string, 5, 25, false, graphics.screen);
	//#endif

	graphics.drawString(_("*** PAUSED ***"), 320, y, TXT_CENTERED, graphics.screen);

	graphics.drawString(_("MIAs in Area"), col1, y += 30, TXT_RIGHT, graphics.screen);
	sprintf(string, "%d", map.totalMIAs - map.foundMIAs);
	graphics.drawString(string, col2, y, TXT_LEFT, graphics.screen);

	graphics.drawString(_("Enemies Defeated"), col1, y += 20, TXT_RIGHT, graphics.screen);
	sprintf(string, "%d", game.currentMissionEnemiesDefeated);
	graphics.drawString(string, col2, y, TXT_LEFT, graphics.screen);

	graphics.drawString(_("Items Collected"), col1, y += 20, TXT_RIGHT, graphics.screen);
	sprintf(string, "%d / %d", map.foundItems, map.totalItems);
	graphics.drawString(string, col2, y, TXT_LEFT, graphics.screen);

	graphics.drawString(_("Best Combo"), col1, y += 20, TXT_RIGHT, graphics.screen);
	sprintf(string, _("%d Hits"), game.maxComboHits);
	graphics.drawString(string, col2, y, TXT_LEFT, graphics.screen);

	graphics.drawString(_("++ Inventory ++"), 320, y += 40, TXT_CENTERED, graphics.screen);
	showCarriedItems();

	// Do the objectives list
	Objective *objective = (Objective*)map.objectiveList.getHead();
	char message[100];

	y += 60;

	if (map.totalMIAs > 0)
	{
		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		sprintf(message, _("Rescue %d MIAs"), map.requiredMIAs);
		graphics.drawString(message, col1, y, TXT_RIGHT, graphics.screen);

		if (map.foundMIAs < map.requiredMIAs)
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			sprintf(message, "%d / %d", map.foundMIAs, map.requiredMIAs);
			graphics.drawString(message, col2, y, TXT_LEFT, graphics.screen);
		}
		else
		{
			graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Completed"), col2, y, TXT_LEFT, graphics.screen);
		}
	}

	while (objective->next != NULL)
	{
		objective = (Objective*)objective->next;

		y += 20;
		
		graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		
		if ((game.skill < 3) &&  (strstr(objective->description, "L.R.T.S.")) && (!gameData.completedWorld))
		{
			graphics.drawString(_("???? ???????? ????"), col1, y, TXT_RIGHT, graphics.screen);
		}
		else
		{
			graphics.drawString(_(objective->description), col1, y, TXT_RIGHT, graphics.screen);
		}
		
		// this is a fake objective (for the 4th Ancient Tomb)
		if (objective->targetValue == -1)
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Incomplete"), col2, y, TXT_LEFT, graphics.screen);
		}
		else if (objective->currentValue < objective->targetValue)
		{
			graphics.setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
			if (objective->targetValue == 1)
			{
				graphics.drawString(_("Incomplete"), col2, y, TXT_LEFT, graphics.screen);
			}
			else
			{
				sprintf(message, "%d / %d", objective->currentValue, objective->targetValue);
    			graphics.drawString(message, col2, y, TXT_LEFT, graphics.screen);
			}

			if (!objective->required)
			{
				graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
				graphics.drawString(_("(optional)"), 450, y, TXT_LEFT, graphics.screen);
			}
		}
		else
		{
			graphics.setFontColor(0x00, 0xff, 0x00, 0x00, 0x00, 0x00);
			graphics.drawString(_("Completed"), col2, y, TXT_LEFT, graphics.screen);
		}
	}

	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	y += 10;

	sprintf(string, "%s - %.2d:%.2d:%.2d", _("Mission Time"), game.currentMissionHours, game.currentMissionMinutes, game.currentMissionSeconds);
	graphics.drawString(string, 320, 430, TXT_CENTERED, graphics.screen);
}

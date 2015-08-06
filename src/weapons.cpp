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

#include "weapons.h"

/**
* Loads all the predefined weapons for the game
*/
void loadDefWeapons()
{
	if (!engine.loadData("data/weapons"))
	{
		graphics.showErrorAndExit("Couldn't load weapon definitions file (%s)", "data/weapons");
	}

	char *token = strtok((char*)engine.dataBuffer, "\n");

	char name[50];
	char sprite[2][100];
	char flags[100];
	int param[8];
	int id;

	while (true)
	{
		if (strcmp(token, "@EOF@") == 0)
			break;

		sscanf(token, "%d %*c %[^\"] %*c %d %d %d %d %d %d %s %s %d %s", &id, name, &param[0], &param[1], &param[2], &param[3], &param[4], &param[5], sprite[0], sprite[1], &param[6], flags);

		weapon[id].setName(name);
		weapon[id].id = param[0];
		weapon[id].damage = param[1];
		weapon[id].health = param[2];
		weapon[id].dx = param[3];
		weapon[id].dy = param[4];
		weapon[id].reload = param[5];
		weapon[id].sprite[0] = graphics.getSprite(sprite[0], true);
		weapon[id].sprite[1] = graphics.getSprite(sprite[1], true);
		weapon[id].fireSound = param[6];
		weapon[id].flags = engine.getValueOfFlagTokens(flags);

		token = strtok(NULL, "\n");
	}
}

/**
* Chooses a random non aimed weapon. Selected either WP_MACHINEGUN, WP_ALIENLASER
* WP_PLASMARIFLE or WP_ALIENGRENADE
* @return The randomly selected Weapon
*/
Weapon *getRandomStraightWeapon()
{
	switch (Math::prand() % 4)
	{
		case 0:
			return &weapon[WP_MACHINEGUN];
			break;
		case 1:
			return &weapon[WP_ALIENLASER];
			break;
		case 2:
			return &weapon[WP_PLASMARIFLE];
			break;
		case 3:
			return &weapon[WP_ALIENGRENADE];
			break;
	}

	return &weapon[WP_PISTOL];
}

/**
* Chooses a random aimed weapon. Selected either WP_AIMEDPISTOL, WP_ALIENSPREAD
* or WP_ALIENGRENADE
* @return The randomly selected Weapon
*/
Weapon *getRandomAimedWeapon()
{
	switch (Math::prand() % 3)
	{
		case 0:
			return &weapon[WP_AIMEDPISTOL];
			break;
		case 1:
			return &weapon[WP_ALIENSPREAD];
			break;
		case 2:
			return &weapon[WP_ALIENGRENADE];
			break;
	}

	return &weapon[WP_AIMEDPISTOL];
}

/**
* Chooses a random weapon for use by Galdov. Selects either WP_AIMEDPISTOL, WP_ALIENSPREAD
* WP_ALIENGRENADE or WP_PLASMARIFLE
* @return The randomly selected Weapon
*/
Weapon *getRandomGaldovWeapon()
{
	switch (Math::prand() % 4)
	{
		case 0:
			return &weapon[WP_AIMEDPISTOL];
			break;
		case 1:
			return &weapon[WP_ALIENSPREAD];
			break;
		case 2:
			return &weapon[WP_ALIENGRENADE];
			break;
		case 3:
			return &weapon[WP_PLASMARIFLE];
			break;
	}

	return &weapon[WP_AIMEDPISTOL];
}

/**
* Searches the Weapon array for the named Weapon. Once it is found
* it is returned. If it is not found WP_PISTOL is returned
* @param name
* @return The randomly selected Weapon
*/
Weapon *getWeaponByName(const char *name)
{
	if (strcmp("randomStraight", name) == 0)
	{
		return getRandomStraightWeapon();
	}

	if (strcmp("randomAimed", name) == 0)
	{
		return getRandomAimedWeapon();
	}

	for (int i = 0 ; i < MAX_WEAPONS ; i++)
	{
		if (strcmp(weapon[i].name, name) == 0)
		{
			return &weapon[i];
		}
	}

	debug(("WARNING - COULD NOT FIND WEAPON '%s'\n", name));

	return &weapon[WP_PISTOL];
}

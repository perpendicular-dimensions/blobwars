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

/**
* Loads all the predefined weapons for the game
*/
void loadDefWeapons()
{
	auto data = engine.loadYAML("data/weapons.yaml");
	if (!data)
		graphics.showErrorAndExit("Couldn't load weapon definitions file (%s)", "data/weapons.yaml");

	for (auto &&it: data)
	{
		auto id = it.first.as<int>();
		if (id < 0 || id >= MAX_WEAPONS)
			graphics.showErrorAndExit("Invalid weapon definitions file (%s)", "data/weapons.yaml");

		weapon[id].setName(it.second["name"].as<std::string>());
		weapon[id].id = it.second["id"].as<int>();
		weapon[id].damage = it.second["damage"].as<int>();
		weapon[id].health = it.second["health"].as<int>();
		weapon[id].dx = it.second["dx"].as<int>();
		weapon[id].dy = it.second["dy"].as<int>();
		weapon[id].reload = it.second["reload"].as<int>();
		weapon[id].sprite[0] = graphics.getSprite(it.second["spriteRight"].as<std::string>(), true);
		weapon[id].sprite[1] = graphics.getSprite(it.second["spriteLeft"].as<std::string>(), true);
		weapon[id].fireSound = it.second["fireSound"].as<int>();
		weapon[id].flags = 0;
		for (auto &&flag: it.second["flags"])
			weapon[id].flags |= engine.getValueOfFlagTokens(flag.as<std::string>());
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
Weapon *getWeaponByName(const std::string &name)
{
	if ("randomStraight" == name)
	{
		return getRandomStraightWeapon();
	}

	if ("randomAimed" == name)
	{
		return getRandomAimedWeapon();
	}

	for (int i = 0; i < MAX_WEAPONS; i++)
	{
		if (weapon[i].name == name)
		{
			return &weapon[i];
		}
	}

	debug(("WARNING - COULD NOT FIND WEAPON '%s'\n", name));

	return &weapon[WP_PISTOL];
}

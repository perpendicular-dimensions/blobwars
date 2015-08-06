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

#include "resources.h"

void loadSound(int index, const char *filename)
{
	audio.loadSound(index, filename);

	graphics.showLoading(1, 30);
}

void loadSprite(const char *token)
{
	Sprite *sprite;

	char name[50];
	char filename[8][100];
	int frameTime[8];

	int i;
	int hue, sat, val;

	sscanf(token, "%s %d %d %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d", name, &hue, &sat, &val, filename[0], &frameTime[0], filename[1], &frameTime[1], filename[2], &frameTime[2], filename[3], &frameTime[3], filename[4], &frameTime[4], filename[5], &frameTime[5], filename[6], &frameTime[6], filename[7], &frameTime[7]);

	sprite = graphics.addSprite(name);

	i = 0;

	while (true)
	{
		if (strcmp(filename[i], "@none@") == 0)
			break;

		sprite->setFrame(i, graphics.loadImage(filename[i], hue, sat, val), frameTime[i]);

		i++;
		
		if (i == 8)
			break;
	}
}

void loadResources()
{
	graphics.resetLoading();

	audio.free();
	graphics.free();
	
	graphics.quickSprite("cheatHeader", graphics.loadImage("gfx/main/cheats.png"));
	graphics.quickSprite("optionsHeader", graphics.loadImage("gfx/main/options.png"));
	graphics.quickSprite("joystickHeader", graphics.loadImage("gfx/main/joystickConfig.png"));
	graphics.quickSprite("keyHeader", graphics.loadImage("gfx/main/keyConfig.png"));
	graphics.quickSprite("optionsBackground", graphics.loadImage("gfx/main/optionsBackground.png"));

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.drawString(_("Loading..."), 320, 440, TXT_CENTERED, graphics.screen);
	graphics.showLoading(0, 0);
	graphics.updateScreen();
	
	if (!engine.loadDefines())
		graphics.showErrorAndExit("Could not load map define list '%s'", "data/defines.h");

	if (!engine.loadData("data/mainSprites"))
		graphics.showErrorAndExit(ERR_FILE, "data/mainSprites");

	char *token = strtok((char*)engine.dataBuffer, "\n");

	while (true)
	{
		if (strcmp(token, "@EOF@") == 0)
		{
			break;
		}

		loadSprite(token);

		token = strtok(NULL, "\n");

		graphics.showLoading(1, 20);
	}

	loadSound(SND_WATERIN, "sound/waterIn");
	loadSound(SND_WATEROUT, "sound/waterOut");
	loadSound(SND_ITEM, "sound/item");
	loadSound(SND_PISTOL, "sound/pistol");
	loadSound(SND_RICO1, "sound/rico1");
	loadSound(SND_RICO2, "sound/rico2");
	loadSound(SND_MACHINEGUN, "sound/machinegun");
	loadSound(SND_DEATH1, "sound/death1");
	loadSound(SND_DEATH2, "sound/death2");
	loadSound(SND_DEATH3, "sound/death3");
	loadSound(SND_GRENADE, "sound/grenade");
	loadSound(SND_ROCKET, "sound/rocket");
	loadSound(SND_FIREBALL, "sound/fireball");
	loadSound(SND_HIT, "sound/punch");
	loadSound(SND_SPLAT, "sound/splat");
	loadSound(SND_POP, "sound/pop1");
	loadSound(SND_TELEPORT1, "sound/teleport1");
	loadSound(SND_TELEPORT2, "sound/teleport2");
	loadSound(SND_TELEPORT3, "sound/teleport3");
	loadSound(SND_GETWEAPON, "sound/weaponPickup");
	loadSound(SND_LASER, "sound/laser");
	loadSound(SND_ELECDEATH1, "sound/elecDeath1");
	loadSound(SND_ELECDEATH2, "sound/elecDeath2");
	loadSound(SND_ELECDEATH3, "sound/elecDeath3");
	loadSound(SND_LOCKEDDOOR, "sound/doorLocked");
	loadSound(SND_OPENDOOR, "sound/doorOpen");
	loadSound(SND_DOOROPENED, "sound/doorOpened");
	loadSound(SND_SWITCH1, "sound/switch");
	loadSound(SND_SWITCH2, "sound/switch");
	loadSound(SND_DROWNING, "sound/drowning");
	loadSound(SND_GULP, "sound/gulp");
	loadSound(SND_EAT, "sound/eat");
	loadSound(SND_THROW, "sound/throw");
	loadSound(SND_GRBOUNCE, "sound/grenadeBounce");
	loadSound(SND_SPREADGUN, "sound/plasma");
	loadSound(SND_STONEBREAK, "sound/stoneBreak");
	loadSound(SND_ELECTRICITY1, "sound/electricity1");
	loadSound(SND_ELECTRICITY2, "sound/electricity2");
	loadSound(SND_ELECTRICITY3, "sound/electricity3");
	loadSound(SND_CLANG, "sound/clang");
	loadSound(SND_FIRECRACKLE, "sound/fireCrackle");
	loadSound(SND_CLOCK, "sound/clock");

	loadDefWeapons();

	graphics.showLoading(35, 35);

	loadDefEnemies();

	graphics.showLoading(40, 40);

	loadDefItems();
	
	graphics.showLoading(45, 45);

	graphics.loadMapTiles("gfx/common");

	player.setSprites(graphics.getSprite("BobRight", true), graphics.getSprite("BobLeft", true), graphics.getSprite("BobSpin", true));

	graphics.showLoading(50, 50);

	loadMapData(game.mapName);

	graphics.showLoading(100, 100);

	#if USEPAK
	graphics.showLoading(100, 100);
	graphics.delay(100);
	#endif

	createBoss(game.stageName);
	
	game.canContinue = 0;

	// We don't need this anymore. Remove it to free up some memory...
	engine.defineList.clear();
}

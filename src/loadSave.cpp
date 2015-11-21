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

#include "loadSave.h"

void initSaveSlots()
{
	char filename[PATH_MAX];
	char string[100];
	struct stat fileInfo;
	int modTime = 0;

	Game tempGame;

	engine.continueSaveSlot = 0;

	FILE *fp;

	//READ SAVE GAME DATA
	for (int i = 0 ; i < 5 ; i++)
	{
		snprintf(filename, sizeof filename, "%ssave%d.dat", engine.userHomeDirectory, i);

		fp = fopen(filename, "rb");

		if (!fp)
		{
			strlcpy(string, "%.2d - %s", sizeof string);
			snprintf(engine.saveSlot[i], sizeof engine.saveSlot[i], string, (i + 1), _("Empty"));
		}
		else
		{
			if (fread(&tempGame, sizeof(Game), 1, fp) != 1)
			{
				strlcpy(string, "%.2d - %s", sizeof string);
				snprintf(engine.saveSlot[i], sizeof engine.saveSlot[i], string, (i + 1), _("Corrupt Save Data"));
			}
			else
			{
				snprintf(engine.saveSlot[i], sizeof engine.saveSlot[i], "%.2d - %s (%.2d:%.2d:%.2d)", (i + 1), _(tempGame.stageName), tempGame.totalHours, tempGame.totalMinutes, tempGame.totalSeconds);
			}

			if (stat(filename, &fileInfo) != -1)
			{
				if (fileInfo.st_mtime > modTime)
				{
					modTime = fileInfo.st_mtime;
					engine.continueSaveSlot = (i + 1);
				}
			}

			fclose(fp);
		}
	}
	
	debug(("Continue Save Slot = %d\n", engine.continueSaveSlot));
}

/*
Fill in later...
*/
bool loadGame(int slot)
{
	debug(("Loading Game #%d...\n", slot));
	game.clear();

	SDL_Delay(500);
	char filename[PATH_MAX];
	
	char line[1024];
	char string[2][1024];
	int param[2];
	
	Data *data;

	FILE *fp;
	
	int sanity = 0;

	snprintf(filename, sizeof filename, "%ssave%d.dat", engine.userHomeDirectory, slot);

	fp = fopen(filename, "rb");
	
	if (!fp)
	{
		return false;
	}

	if (fread(&game, sizeof(Game), 1, fp) != 1)
	{
		fclose(fp);
		return graphics.showErrorAndExit("The save data loaded was not in the format expected", ""), false;
	}
	
	fclose(fp);
	
	snprintf(filename, sizeof filename, "%spersistant%d.dat", engine.userHomeDirectory, slot);

	fp = fopen(filename, "rb");
	
	if (!fp)
	{
		return false;
	}
	
	while (true)
	{
		if (!fgets(line, 1024, fp)) {
			fclose(fp);
			return graphics.showErrorAndExit("Unexpected end of file reading save data", ""), false;
		}

		sscanf(line, "%*c %[^\"] %*c %*c %[^\"] %*c %d %d", string[0], string[1], &param[0], &param[1]);
		
		data = new Data();
		
		data->set(string[0], string[1], param[0], param[1]);
		
		debug(("Read %s %s %d %d\n", data->key, data->value, data->current, data->target));
		
		if ((data->current == -1) && (data->target == -1))
		{
			delete data;
			break;
		}

		gameData.addCompletedObjective(data);
		
		sanity++;
		
		if (sanity == 10000)
		{
			debug(("Sanity Check #1 > 10000!\n"));
			fclose(fp);
			exit(1);
		}
	}
	
	sanity = 0;
	
	char stageName[50];
	int numberOfLines = 0;
	
	Persistant *persistant;
	PersistData *persistData;
	
	while (true)
	{
		if (!fgets(line, 1024, fp)) {
			fclose(fp);
			graphics.showErrorAndExit("Unexpected end of file reading save data", "");
		}

		sscanf(line, "%[^\n\r]", string[0]);
		strlcpy(stageName, string[0], sizeof stageName);
		
		if (strcmp(stageName, "@EOF@") == 0)
		{
			break;
		}

		if (!fgets(line, 1024, fp)) {
			fclose(fp);
			graphics.showErrorAndExit("Unexpected end of file reading save data", "");
		}
		sscanf(line, "%d", &numberOfLines);
		
		debug(("Read %s with %d lines.\n", stageName, numberOfLines));
		
		persistant = map.createPersistant(stageName);
		
		for (int i = 0 ; i < numberOfLines ; i++)
		{
			persistData = new PersistData();

			if (!fgets(line, 1024, fp)) {
				fclose(fp);
				graphics.showErrorAndExit("Unexpected end of file reading save data", "");
			}

			strlcpy(persistData->data, line, sizeof persistData->data);
			
			//debug(("Read %d: %s", i, persistData->data));
			
			persistant->addLine(persistData->data);
			
			sanity++;
		
			if (sanity == 100000)
			{
				debug(("Sanity Check #2 > 100000!\n"));
				fclose(fp);
				exit(1);
			}
		}
	}

	fclose(fp);
	
	debug(("Loaded Successfully\n"));

	return true;
}

int confirmSave()
{
	if (game.autoSave)
	{
		return game.autoSaveSlot;
	}
	
	if (!engine.loadWidgets("data/saveWidgets"))
		graphics.showErrorAndExit(ERR_FILE, "data/saveWidgets");
	
	int slot[6], quitYes, quitNo;
	slot[0] = slot[1] = slot[2] = slot[3] = slot[4] = slot[5] = 0;
	quitYes = quitNo = 0;
	
	engine.setWidgetVariable("slot1", &slot[0]);
	engine.setWidgetVariable("slot2", &slot[1]);
	engine.setWidgetVariable("slot3", &slot[2]);
	engine.setWidgetVariable("slot4", &slot[3]);
	engine.setWidgetVariable("slot5", &slot[4]);
	engine.setWidgetVariable("slot6", &slot[5]);
	
	engine.setWidgetVariable("contyes", &quitYes);
	engine.setWidgetVariable("contno", &quitNo);
	
	char widgetName[10];
	widgetName[0] = 0;
	
	for (int i = 0 ; i < 5 ; i++)
	{
		snprintf(widgetName, sizeof widgetName, "slot%d", i + 1);
		strlcpy(engine.getWidgetByName(widgetName)->label, engine.saveSlot[i], sizeof engine.getWidgetByName(widgetName)->label);
	}
	
	engine.highlightWidget("slot1");
	
	int menuSound = 0;
	
	int rtn = -1;
	
	engine.showWidgetGroup("gameSlots", true);
	engine.showWidgetGroup("continueconf", false);
	
	graphics.setFontSize(4);
	SDL_Surface *title = graphics.quickSprite("savetitle", graphics.getString("Save Game", true));
	
	while (true)
	{
		graphics.updateScreen();
		SDL_FillRect(graphics.screen, NULL, graphics.black);
		engine.getInput();
		config.populate();

		menuSound = engine.processWidgets();

		if (menuSound)
			audio.playMenuSound(menuSound);
		
		graphics.blit(title, 320, 100, graphics.screen, true);
		
		drawWidgets();
		
		if (slot[5])
		{
			engine.showWidgetGroup("gameSlots", false);
			engine.showWidgetGroup("continueconf", true);
			engine.highlightWidget("contno");
			drawWidgets();
			slot[5] = 0;
		}
		
		if (quitYes)
		{
			break;
		}
		
		if (quitNo)
		{
			engine.showWidgetGroup("gameSlots", true);
			engine.showWidgetGroup("continueconf", false);
			engine.highlightWidget("slot1");
			drawWidgets();
			quitNo = 0;
		}
		
		for (int i = 0 ; i < 5 ; i++)
		{
			if (slot[i])
			{
				rtn = i;
			}
		}
		
		if ((slot[0]) || (slot[1]) || (slot[2]) || (slot[3]) || (slot[4]) || (slot[5]))
		{
			break;
		}

		SDL_Delay(16);
	}
	
	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.updateScreen();
	SDL_Delay(250);
	
	return rtn;
}

void saveGame()
{
	char message[256];

	SDL_FillRect(graphics.screen, NULL, graphics.black);
	graphics.updateScreen();
	SDL_Delay(500);
	
	int slot = confirmSave();
	
	if (slot == -1)
		return;

	graphics.setFontSize(1);
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	snprintf(message, sizeof message, _("Saving Game to Save Slot #%d. Please Wait..."), slot + 1);
	graphics.drawString(message, 320, 220, true, graphics.screen);
	graphics.updateScreen();

	char filename[PATH_MAX];

	FILE *fp;

	snprintf(filename, sizeof filename, "%ssave%d.dat", engine.userHomeDirectory, slot);

	fp = fopen(filename, "wb");
	
	if (!fp)
	{
		return graphics.showErrorAndExit("File write error whilst saving game", "");
	}

	if (fwrite(&game, sizeof(Game), 1, fp) != 1)
	{
		fclose(fp);
		return graphics.showErrorAndExit("File write error whilst saving game", strerror(errno));
	}
	
	fclose(fp);
	
	snprintf(filename, sizeof filename, "%spersistant%d.dat", engine.userHomeDirectory, slot);

	fp = fopen(filename, "wt");
	
	if (!fp)
	{
		return graphics.showErrorAndExit("File write error whilst saving game", "");
	}
	
	createPersistantMapData();
	
	Data *data = (Data*)gameData.dataList.getHead();

	while (data->next != NULL)
	{
		data = (Data*)data->next;
		
		fprintf(fp, "\"%s\" \"%s\" %d %d\n", data->key, data->value, data->current, data->target);
	}
	
	fprintf(fp, "\"@EOF@\" \"@EOF@\" -1 -1\n");
	
	Persistant *persistant = (Persistant*)map.persistantList.getHead();
	PersistData *persistData;
	
	while (persistant->next != NULL)
	{
		persistant = (Persistant*)persistant->next;
		
		if (strcmp(persistant->stageName, "@none@") == 0)
		{
			continue;
		}
		
		fprintf(fp, "%s\n", persistant->stageName);
		fprintf(fp, "%d\n", persistant->numberOfLines);
	
		persistData = (PersistData*)persistant->dataList.getHead();
		
		while (persistData->next != NULL)
		{
			persistData = (PersistData*)persistData->next;
			
			fprintf(fp, "%s", persistData->data);
		}
	}
	
	fprintf(fp, "@EOF@\n");
	
	fclose(fp);
	
	map.clear();
	
	SDL_Delay(500);

	graphics.drawString(_("Save Complete"), 320, 260, true, graphics.screen);
	graphics.updateScreen();

	SDL_Delay(500);
}
